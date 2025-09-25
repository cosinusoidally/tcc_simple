extern int stdout;

int regs_data[8];

int PROT_READ=1;
int PROT_WRITE=2;
int PROT_EXEC=4;
int MAP_ANONYMOUS=32;
int MAP_PRIVATE=2;
int MAP_FIXED=0x10;

int wi8(int o,int v) {
        char *h = 0;
        h[o]=v;
        return 0;
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
  trap_syscalls_off();
  r = syscall(3, regs_data[1],regs_data[2],regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
  trap_syscalls_on();
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
  r = syscall(5, filename, flags, mode, 0, 0, 0);
  printf("open: fd %d\n", r);
  trap_syscalls_on();
  return r;
}

int vm_close() {
  int r;
  trap_syscalls_off();
  r = syscall(6, regs_data[1],regs_data[2],regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
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

  trap_syscalls_on();
  asm("mov $0x8047F80,%esp");
  asm("mov $0x8048054,%eax");
  asm("jmp %eax");
  trap_syscalls_off();
  return 0;
}
