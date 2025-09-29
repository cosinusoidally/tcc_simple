extern int stdout;

int regs_data[8];

int PROT_READ=1;
int PROT_WRITE=2;
int PROT_EXEC=4;
int MAP_ANONYMOUS=32;
int MAP_PRIVATE=2;
int MAP_FIXED=0x10;

int file_addr = 256*1024*1024;

int next_filenum = 4;
int next_fd;

int filename_array = 0x0200000;
int filename_size = 1024;

int gfds = 0x01000010;
int sizeof_gfd = 8;


int file_descriptors = 0x20000+0x200;
int sizeof_file_descriptor = 8;

char *heap = 0;

int elf_base = 0x8048000;
int args_base = 0x8047B80;

int command_num = 0;

char *commands[] = {
//  "../artifacts/pnut-exe hello.c",
  "/hex0-orig hex0_x86.hex0 /hex0",
  "/hex0 hex1_x86.hex0 /hex1",
  "/hex1 hex2_x86.hex1 /hex2-0",
  "/hex2-0 catm_x86.hex2 /catm",
  "/catm M0.hex2 ELF-i386.hex2 M0_x86.hex2",
  "/hex2-0 M0.hex2 /M0",
  "/M0 cc_x86_min.M1 cc_x86_min-0.hex2",
  "/catm cc_x86_min-1.hex2 ELF-i386.hex2 cc_x86_min-0.hex2",
  "/hex2-0 cc_x86_min-1.hex2 /cc_x86_min.exe",

  "/catm js_to_c_simple_all.c js_to_c_m2_support.c js_to_c_simple.c",
  "/cc_x86_min.exe js_to_c_simple_all.c js_to_c.exe.M1",
  "/catm js_to_c.exe-0.M1 simple_asm_defs.M1 x86_defs.M1 libc-core.M1 js_to_c.exe.M1",
  "/M0 js_to_c.exe-0.M1 js_to_c.exe.hex2",
  "/catm js_to_c.exe-0.hex2 ELF-i386.hex2 js_to_c.exe.hex2",
  "/hex2-0 js_to_c.exe-0.hex2 /js_to_c.exe",
  "/js_to_c.exe cjsawk_full.c cjsawk_full_js_to_c.c",

  "/cc_x86_min.exe cjsawk_full_js_to_c.c cjsawk_boot.exe.M1",
  "/catm cjsawk_boot.exe-0.M1 simple_asm_defs.M1 x86_defs.M1 libc-core.M1 cjsawk_boot.exe.M1",
  "/M0 cjsawk_boot.exe-0.M1 cjsawk_boot.exe.hex2",
  "/catm cjsawk_boot.exe-0.hex2 ELF-i386.hex2 cjsawk_boot.exe.hex2",
  "/hex2-0 cjsawk_boot.exe-0.hex2 /cjsawk_boot.exe",

  "/cjsawk_boot.exe hello.c out_dummy.M1",
  "/cjsawk_boot.exe cjsawk_full.c cjsawk.M1",
  "/catm cjsawk-0.M1 simple_asm_defs.M1 x86_defs.M1 libc-core.M1 cjsawk.M1",
  "/M0 cjsawk-0.M1 cjsawk.hex2",
  "/catm cjsawk-0.hex2 ELF-i386.hex2 cjsawk.hex2",
  "/hex2-0 cjsawk-0.hex2 /cjsawk.exe",
  0,
};

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
  int t = 0;

  if(fd_get_file_offset(fd) == gfd_get_file_length(fd_get_filenum(fd))) {
    r = 0;
  } else {
    while(t < count) {
      if(fd_get_file_offset(fd) == gfd_get_file_length(fd_get_filenum(fd))) {
        break;
      }
      c = ri8(gfd_get_file_addr(fd_get_filenum(fd))+fd_get_file_offset(fd));
      fd_set_file_offset(fd, fd_get_file_offset(fd) + 1);
      wi8(buf, c);
      buf = buf + 1;
      t = t + 1;
      if(t > 1) {
/*
         trap_syscalls_off();
         printf("vm_read %d\n",t);
         trap_syscalls_on();
*/
      }
    }
    r = t;
  }
  return r;
}

int vm_write() {
  int r;
  int t;
  int fd = regs_data[1];
  int buf = regs_data[2];
  int count = regs_data[3];
  int c2 = count;
  if(fd < 3) {
    trap_syscalls_off();
    syscall(4, fd, buf, count);
    trap_syscalls_on();
    return count;
  }
  t = fd_get_file_offset(fd);
  while(c2>0) {
    wi8(gfd_get_file_addr(fd_get_filenum(fd))+t, ri8(buf));
    t = t + 1;
    fd_set_file_offset(fd, t);
    gfd_set_file_length(fd_get_filenum(fd), t);
    c2 = c2 - 1;
    buf = buf + 1;
  }
  return count;
}

int vm_open() {
  int r;
  int t;
  int filename = regs_data[1];
  int flags = regs_data[2];
  int mode = regs_data[3];
  trap_syscalls_off();
  flags = flags & 0xFFFF;
  mode = mode & 0xFFFF;
  printf("open: %s %d %d\n", filename, flags, mode);
  if((flags==577) && ((mode == 384) || (mode == 448))) {
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

int vm_lseek() {
  int fd = regs_data[1];
  int offset = regs_data[2];
  int whence = regs_data[3];
  trap_syscalls_off();
  printf("vm_lseek: %d %d %d\n", fd, offset, whence);
  if(whence !=0) {
    printf("vm_lseek only whence = 0 supported\n");
  }
  fd_set_file_offset(fd, offset);
  trap_syscalls_on();
  return offset;
}

int run_again = 1;

int vm_exit() {
  int error_code = regs_data[1];
  trap_syscalls_off();
  if(error_code != 0) {
    printf("vm_exit: %d\n", error_code);
// swallow error for now
//    exit(error_code);
  }
  command_num = command_num + 1;
  printf("brk_ptr: 0x%x\n", brk_ptr);

  if(commands[command_num]) {
    run_process(commands[command_num]);
    printf("shouldn't get here\n");
    exit(1);
  } else {
    extract_file("out_dummy.M1", "artifacts/out.M1");
    extract_file("cjsawk.M1", "artifacts/cjsawk.exe.M1");
    extract_file("cjsawk.hex2", "artifacts/cjsawk.exe.hex2");
    extract_file("/cjsawk.exe", "artifacts/cjsawk.exe");
    extract_file("/cjsawk_boot.exe", "artifacts/cjsawk_boot.exe");
    extract_file("/hex0", "artifacts/hex0");
    extract_file("/hex2-0", "artifacts/hex2-0");
    extract_file("/M0", "artifacts/M0");
    extract_file("cc_x86_min-0.hex2", "artifacts/cc_x86_min-0.hex2");
    extract_file("cc_x86_min-1.hex2", "artifacts/cc_x86_min-1.hex2");
    extract_file("/cc_x86_min.exe", "artifacts/cc_x86_min.exe");
    extract_file("cjsawk_full_js_to_c.c", "artifacts/cjsawk_full_js_to_c.c");
    exit(error_code);
  }
}

extract_file(vfs_name, real_name) {
  int t;
  t = find_file(vfs_name);
  if(t != 0) {
    int ofile=fopen(real_name, "w");
    fwrite(gfd_get_file_addr(t), 1, gfd_get_file_length(t), ofile);
    fclose(ofile);
  }
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
  } else if(n == 19) {
    r = vm_lseek();
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
  printf("file_length: %d\n", gfd_get_file_length(t));
  fclose(f);
}

reset_process() {
  int base_addr;
  int i;
  base_addr =args_base;
  printf("reset_memory: 0x%x 0x%x\n", base_addr, brk_ptr);
  i = base_addr;
  while(i <=brk_ptr) {
    wi8(i,0);
    i = i + 1;
  }
  printf("reset file descriptors\n");
  next_fd = 4;
}

run_process(cmdline) {
  int o;
  int c;
  int t;
  int args_offset;
  int last_offset;
  int argc;
  int foo;

  o = 0;
  args_offset = 128;
  int *args;
  args = args_base;
  last_offset = args_base+args_offset;

  reset_process();

  while(1) {
    c = ri8(cmdline+o);
    t = c;
    if(t == ' ' || t == 0) {
      t = 0;
      argc= args[0] + 1;
      args[0] = argc;
      args[argc]= last_offset;
      last_offset = args_base + args_offset +o +1;
    }
    wi8(args_base+args_offset+o,t);
    if(c == 0) {
      break;
    }
    o = o + 1;
  }
  args[argc+1] = 0;
  int i = 0;
  while(i < argc){
    printf("run_process arg[%d]: %s\n", i + 1, args[i+1]);
    i = i + 1;
  }

  o = elf_base;

  if(foo = find_file(args[1])) {
    printf("run_process: found %s in vfs\n", args[1]);
    t = 0;
    int p;
    int l;
    p = gfd_get_file_addr(foo);
    l = gfd_get_file_length(foo);
    while(t<l) {
      wi8(o, ri8(p+t));
      t = t + 1;
      o = o + 1;
    }
  } else {
    foo=fopen(args[1], "r");
    while((c=fgetc(foo))!=-1) {
      wi8(o,c);
      o = o + 1;
    }
    fclose(foo);
  }

  printf("o: %x\n", o);

  brk_ptr = 4096+4096*(o/4096);
  printf("brk_ptr: %x\n", brk_ptr);

  trap_syscalls_on();
  asm("mov $0x8047B80,%esp");
  asm("mov $0x8048054,%eax");
  asm("jmp %eax");
  trap_syscalls_off();

  return 0;
}

main(){
/* big mapping for our heap */
  int res = 0;
  res = mmap(0x20000, 512*1024*1024, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, 0, 0);
  printf("res: %d\n", res);
  if(res == -1 ) {
    printf("mmap error\n");
    exit(1);
  }

  brk_ptr = elf_base;

  load_file("../cjsawk/hello.c", "hello.c");
  load_file("../cjsawk/artifacts/deps/cjsawk_full.c", "cjsawk_full.c");
  load_file("../m2min_v3/simple_asm_defs.M1", "simple_asm_defs.M1");
  load_file("../m2min_v3/x86_defs.M1", "x86_defs.M1");
  load_file("../m2min_v3/libc-core.M1", "libc-core.M1");
  load_file("../m2min_v3/ELF-i386.hex2", "ELF-i386.hex2");

  load_file("../m2min_v2/artifacts/hex0", "/hex0-orig");

  load_file("../m2min_v2/hex0_x86.hex0", "hex0_x86.hex0");
  load_file("../m2min_v2/hex1_x86.hex0", "hex1_x86.hex0");
  load_file("../m2min_v2/hex2_x86.hex1", "hex2_x86.hex1");
  load_file("../m2min_v2/catm_x86.hex2", "catm_x86.hex2");
  load_file("../m2min_v2/M0_x86.hex2", "M0_x86.hex2");
  load_file("../m2min_v2/cc_x86_min.M1", "cc_x86_min.M1");

  load_file("../m2min_v3/js_to_c/js_to_c_m2_support.c", "js_to_c_m2_support.c");
  load_file("../m2min_v3/js_to_c/js_to_c_simple.c", "js_to_c_simple.c");

  run_process(commands[0]);

  return 0;
}
