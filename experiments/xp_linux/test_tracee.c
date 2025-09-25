extern int stdout;

int regs_data[8];

int PROT_READ=1;
int PROT_WRITE=2;
int PROT_EXEC=4;
int MAP_ANONYMOUS=32;
int MAP_PRIVATE=2;
int MAP_FIXED=0x10;

int file_addr = 0x54000000;
int file_offset = 0;
int file_length = 0;

char *heap = 0;

int wi8(int o,int v) {
        char *h = 0;
        h[o]=v;
        return 0;
}

int ri8(o) {
  return heap[o] & 255;
}

int wrap_syscall();

int trap_syscalls_on() {
  syscall(65534, wrap_syscall, regs_data);
}

int trap_syscalls_off() {
  syscall(65533);
}

int brk_ptr=0;

int vm_brk() {
  int p=regs_data[1];
  if(p!=0) {
    brk_ptr=p;
  }
  return brk_ptr;
}

int vm_read() {
  int r;
  int c;
  int fd = regs_data[1];
  int buf = regs_data[2];
  int count = regs_data[3];
  if(count != 1) {
    trap_syscalls_off();
    printf("vm_read only supports count 1\n");
    exit(1);
  }
/* disabled direct syscall */
//  r = syscall(3, fd, buf, count, 0, 0, 0);
  if(file_offset == file_length) {
    r = 0;
  } else {
    c = ri8(file_addr+file_offset);
    file_offset = file_offset + 1;
    wi8(buf, c);
    r = 1;
  }
  return r;
}

int vm_write() {
  int r;
  trap_syscalls_off();
  r = syscall(4, regs_data[1],regs_data[2],regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
  trap_syscalls_on();
  return r;
}

int vm_open() {
  int r;
  int filename = regs_data[1];
  int flags = regs_data[2];
  int mode = regs_data[3];
  trap_syscalls_off();
  printf("open: %s %d %d\n", filename, flags, mode);
  if((flags==577) && (mode == 384)) {
    printf("open %s for write\n", filename);
  } else {
    printf("open %s for read\n", filename);
  }
  r = syscall(5, filename, flags, mode, 0, 0, 0);
  printf("open: fd %d\n", r);
  trap_syscalls_on();
  return r;
}

int vm_close() {
  int r;
  int fd = regs_data[1];
  trap_syscalls_off();
  printf("close: %d\n", fd);
  r = syscall(6, fd, 0, 0, 0, 0, 0);
  trap_syscalls_on();
  return r;
}

int vm_exit() {
  int r;
  trap_syscalls_off();
  printf("brk_ptr: %x\n", brk_ptr);
  r = syscall(1, regs_data[1],regs_data[2],regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
  trap_syscalls_on();
  return r;
}

int wrap_syscall() {
  int r;
  int n;
//  printf("wrap_syscall eax: %d ebx: %d ecx: %d edx: %d esi: %d edi: %d ebp: %d\n", regs_data[0], regs_data[1], regs_data[2], regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
  n = regs_data[0];
  if(n == 45) {
    r = vm_brk();
  } else if(n == 3) {
    r = vm_read();
  } else if(n == 4) {
    r = vm_write();
  } else if(n == 5) {
    r = vm_open();
  } else if(n == 6) {
    r = vm_close();
  } else if(n == 1) {
    r = vm_exit();
  } else {
    trap_syscalls_off();
    printf("unsupported syscall: %d\n",n);
    exit(1);
  }
  return r;
}

load_file() {
  int f = fopen("artifacts/hello.c", "r");
  int c;
  while((c = fgetc(f)) != -1) {
    wi8(file_addr+file_offset, c);
    file_offset=file_offset+1;
  }
  file_length = file_offset;
  file_offset = 0;
  printf("file_length: %d\n", file_length);
  fclose(f);
}

main(){
/* big mapping for our heap */
  int res = 0;
  res = mmap(0x20000, 7*512*1024*1024, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, 0, 0);
  printf("res: %d\n", res);
  if(res == -1 ) {
    printf("mmap error\n");
    exit(1);
  }
//  int foo=fopen("../cjsawk/artifacts/builds/hello/hello.exe", "r");
  int foo=fopen("../cjsawk/artifacts/builds/full_cc_x86_min/cjsawk.exe", "r");
  int c;
  int o = 0x8048000;
  while((c=fgetc(foo))!=-1) {
    wi8(o,c);
    o = o + 1;
  }
  printf("o: %x\n", o);

  int *args;
  args = 0x8047F80;
  args[0] = 3;
  args[1] = "cjsawk.exe";
  args[2] = "artifacts/hello.c";
  args[3] = "artifacts/out.M1";

  brk_ptr = 4096+4096*(o/4096);
  printf("brk_ptr: %x\n", brk_ptr);

  load_file();

  trap_syscalls_on();
  asm("mov $0x8047F80,%esp");
  asm("mov $0x8048054,%eax");
  asm("jmp %eax");
  trap_syscalls_off();
  return 0;
}
