extern int stdout;

int regs_data[8];

int PROT_READ=1;
int PROT_WRITE=2;
int PROT_EXEC=4;
int MAP_ANONYMOUS=32;
int MAP_PRIVATE=2;
int MAP_FIXED=0x10;

int file_addr = 0x54000000;
int file_length = 0;

int next_filenum = 4;
int next_fd = 4;

int filename_array = 0x0200000;
int filename_size = 1024;

int gfds = 0x01000010;
int sizeof_gfd = 8;


int file_descriptors = 0x20000+0x200;
int sizeof_file_descriptor = 8;

char *heap = 0;

int wi8(int o,int v) {
        heap[o]=v;
        return 0;
}

int wi32(int o,int v) {
	int *h;
        h = o;
        h[0]=v;
        return 0;
}

int ri8(o) {
  return heap[o] & 255;
}

int ri32(o) {
  int *h;
  h = o;
  return h[0];
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
  if(fd_get_file_offset(fd) == file_length) {
    r = 0;
  } else {
    c = ri8(gfd_get_file_addr(fd_get_filenum(fd))+fd_get_file_offset(fd));
    fd_set_file_offset(fd, fd_get_file_offset(fd) + 1);
    wi8(buf, c);
    r = 1;
  }
  return r;
}

int vm_write() {
  int r;
  int t;
  int fd = regs_data[1];
  int buf = regs_data[2];
  int count = regs_data[3];
  if(fd == 0) {
    trap_syscalls_off();
    printf("vm_write doesn't support stdout yet\n");
    exit(1);
  }
  if(count != 1) {
    trap_syscalls_off();
    printf("vm_read only supports count 1\n");
    exit(1);
  }
  t = fd_get_file_offset(fd);
  wi8(gfd_get_file_addr(fd_get_filenum(fd))+t, ri8(buf));
  t = t + 1;
  fd_set_file_offset(fd, t);
  gfd_set_file_length(fd_get_filenum(fd), t);
  return r;
}

int vm_open() {
  int r;
  int t;
  int filename = regs_data[1];
  int flags = regs_data[2];
  int mode = regs_data[3];
  trap_syscalls_off();
  printf("open: %s %d %d\n", filename, flags, mode);
  if((flags==577) && (mode == 384)) {
    printf("open %s for write\n", filename);
    t = new_file(filename);
    r = new_fd(t);
  } else {
    printf("open %s for read\n", filename);
    t = find_file(filename);
    if(t) {
      r = new_fd(t);
    } else {
      trap_syscalls_off();
      printf("file not found %s\n", filename);
      exit(1);
    }
  }
  printf("open: fd %d\n", r);
  trap_syscalls_on();
  return r;
}

int gfn_get_filename(fn) {
  return filename_array + (fn*filename_size);
}

int vm_close() {
  int r;
  int t;
  int tn;
  int fd = regs_data[1];
  trap_syscalls_off();
  printf("close: %d\n", fd);
  t = fd_get_filenum(fd);
  tn = gfn_get_filename(t);
  printf("t: %d tn: %s\n", t, tn);
  trap_syscalls_on();
  return r;
}

int vm_exit() {
  int error_code = regs_data[1];
  trap_syscalls_off();
  printf("brk_ptr: %x\n", brk_ptr);
  int ofile=fopen("artifacts/out.M1", "w");
  int t;
  t = find_file("artifacts/out_dummy.M1");
  fwrite(gfd_get_file_addr(t), 1, fd_get_file_offset(5), ofile);
  fclose(ofile);
  exit(error_code);
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

int find_file(filename) {
  int t;
  int i = next_filenum - 1;
  printf("find_file: %s\n", filename);
  while(i>3) {
    t = filename_array+(i*filename_size);
    printf("looking at: %s\n", t);
    if(strcmp(t, filename) == 0) {
      printf("found: %s %d\n", filename, i);
      return i;
    }
    i = i - 1;
  }
  printf("not found %s\n", filename);
  return 0;
}

int fd_set_filenum(fd, filenum) {
  wi32(file_descriptors+(fd*sizeof_file_descriptor), filenum);
}

int fd_get_filenum(fd) {
  return ri32(file_descriptors+(fd*sizeof_file_descriptor));
}

int fd_set_file_offset(fd, o) {
  wi32(file_descriptors+(fd*sizeof_file_descriptor)+4, o);
}

int fd_get_file_offset(fd) {
  return ri32(file_descriptors+(fd*sizeof_file_descriptor)+4);
}


int new_fd(filenum) {
  fd_set_filenum(next_fd, filenum);
  fd_set_file_offset(next_fd, 0);
  next_fd = next_fd+1;
  return next_fd - 1;
}

int gfd_set_file_addr(filenum, addr) {
  wi32(gfds+(filenum*sizeof_gfd), addr);
}

int gfd_get_file_addr(filenum) {
  return ri32(gfds+(filenum*sizeof_gfd));
}

int gfd_set_file_length(filenum, len) {
  wi32(gfds+(filenum*sizeof_gfd)+4, len);
}

int gfd_get_file_length(filenum) {
  return ri32(gfds+(filenum*sizeof_gfd)+4);
}

int new_file(int filename) {
  file_addr = file_addr + gfd_get_file_length(next_filenum - 1);
  gfd_set_file_addr(next_filenum, file_addr);
  gfd_set_file_length(next_filenum, 0);
  file_length = 0;
  strcpy(gfn_get_filename(next_filenum), filename);
  next_filenum = next_filenum + 1;
  return next_filenum - 1;
}

load_file(realname, virtualname) {
  printf("load_file: %s virtualname: %s\n", realname, virtualname);
  int f = fopen(realname, "r");
  int c;
  int t;
  t = new_file(virtualname);
  while((c = fgetc(f)) != -1) {
    wi8(file_addr+gfd_get_file_length(t), c);
    gfd_set_file_length(t, gfd_get_file_length(t)+1);
  }
  file_length = gfd_get_file_length(t);
  printf("file_length: %d\n", gfd_get_file_length(t));
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
/* dummy for now to make sure we are really reading via virtual syscalls */
  args[2] = "hello.c";
  args[3] = "artifacts/out_dummy.M1";

  brk_ptr = 4096+4096*(o/4096);
  printf("brk_ptr: %x\n", brk_ptr);

  load_file("../cjsawk/artifacts/builds/full_cc_x86_min/cjsawk.exe", "cjsawk.exe");
  load_file("../cjsawk/hello.c", "hello.c");

  trap_syscalls_on();
  asm("mov $0x8047F80,%esp");
  asm("mov $0x8048054,%eax");
  asm("jmp %eax");
  trap_syscalls_off();
  return 0;
}
