int dbg = 0;

int regs_data[9];

int PROT_READ=1;
int PROT_WRITE=2;
int PROT_EXEC=4;
int MAP_ANONYMOUS=32;
int MAP_PRIVATE=2;
int MAP_FIXED=0x10;

int file_addr;

int next_filenum = 4;
int next_fd;

int base_address;

int filename_array;
int filename_size = 1024;

int gfds;
int sizeof_gfd = 8;


int file_descriptors;
int sizeof_file_descriptor = 8;

char *heap = 0;

int elf_base = 0x8048000;
/* elf_base - (8192 + 2048) 8192 bytes for command line and 2048 for argv */
int args_base = 0x8045800;

int command_num = 0;

int command_file;

char *commands[] = {
//  "../artifacts/tcc-pnut -Dfunction=int -Dvar=int -c foo.c", 0,
  "hex0 hex0_x86.hex0 /hex0",
  "/hex0 hex1_x86.hex0 /hex1",
  "/hex1 hex2_x86.hex1 /hex2-0",
  "/hex2-0 catm_x86.hex2 /catm",
  "/catm M0.hex2 ELF-i386.hex2 M0_x86.hex2",
  "/hex2-0 M0.hex2 /M0",
  "/M0 cc_x86_min.M1 cc_x86_min-0.hex2",
  "/catm cc_x86_min-1.hex2 ELF-i386.hex2 cc_x86_min-0.hex2",
  "/hex2-0 cc_x86_min-1.hex2 /cc_x86_min.exe",

  "/catm js_to_c_simple_all.c js_to_c_m2_support.c js_to_c_simple.c",
  "/catm pnut_js_m2.c pnut_refactor/simple_support_js_m2_prims.c pnut_refactor/support_common.c pnut_refactor/simple_support_js_m2.js pnut_refactor/pnut.js",

  "/cc_x86_min.exe js_to_c_simple_all.c js_to_c.exe.M1",
  "/catm js_to_c.exe-0.M1 simple_asm_defs.M1 x86_defs.M1 libc-core.M1 js_to_c.exe.M1",
  "/M0 js_to_c.exe-0.M1 js_to_c.exe.hex2",
  "/catm js_to_c.exe-0.hex2 ELF-i386.hex2 js_to_c.exe.hex2",
  "/hex2-0 js_to_c.exe-0.hex2 /js_to_c.exe",

  "/catm cjsawk_full.c cjsawk.js support_libc.js simple_support_js_m2_prims.c support_m2.c",
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

  "/cjsawk.exe pnut_js_m2.c pnut_js.exe.M1",
  "/catm pnut_js.exe-0.M1 simple_asm_defs.M1 x86_defs.M1 libc-core.M1 pnut_js.exe.M1",
  "/M0 pnut_js.exe-0.M1 pnut_js.exe.hex2",
  "/catm pnut_js.exe-0.hex2 ELF-i386.hex2 pnut_js.exe.hex2",
  "/hex2-0 pnut_js.exe-0.hex2 /pnut_js.exe",
  0,
};

int command_next;

int wrap_syscall_alt(edi, esi, ebp, esp, ebx, edx, ecx, eax);

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

char command_buffer[8192];

int next_command() {
  int o = 0;
  int c;
  if(command_file) {
    while((c = fgetc(command_file)) != -1) {
      if(c == '\n') {break;}
//      fputc(c,stdout);
      wi8(command_buffer+o, c);
      o = o + 1;
    }
    wi8(command_buffer+o, 0);
    printf("command_buffer %s\n", command_buffer);
    if(o == 0) { return o;}
    return command_buffer;
  } else {
    command_num = command_num + 1;
    return commands[command_num - 1];
  }
}

/* buffer for absolute filenames */
char afb[1024];

int absolute_path(filename) {
  int i = 0;
  int j = 0;
  int t;
  while(j<1024) {
    wi8(afb+j,0);
    j = j +1;
  }
  j = 0;
  wi8(afb,'/');
  j = j + 1;
  if(ri8(filename) == '/') {
    i = i + 1;
  }
  while((t=ri8(filename+i)) !=0) {
    if(t == '.') {
      i = i + 1;
      t = ri8(filename+i);
      if(t == '.') {
        i = i + 1;
        t = ri8(filename+i);
        if(t == '/') {
          i = i + 1;
          t = ri8(filename+i);
          while((ri8(afb+j) != '/')&& (j>=0)){
            j = j - 1;
          }
          j = j - 1;
          if(j < 0) { j = 0;}
          while((ri8(afb+j) != '/')&& (j>=0)){
            j = j - 1;
          }
          j = j + 1;
        } else {
          printf("absolute_path error\n");
          exit(1);
        }
      } else if(t == '/') {
        i = i + 1;
      } else {
        i = i - 1;
        t = ri8(filename+i);
      }
    }
    wi8(afb+j, t);
    i = i + 1;
    j = j + 1;
  }
  wi8(afb+j, 0);
  return afb;
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
  if((flags==577) && ((mode == 384) || (mode == 448) || (mode == 438))) {
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

int vm_exit() {
  int t;
  int error_code = regs_data[1];
  trap_syscalls_off();
  if(error_code != 0) {
    printf("vm_exit: %d\n", error_code);
// swallow error for now
//    exit(error_code);
  }
  printf("brk_ptr: 0x%x\n", brk_ptr);

  if(t = next_command()) {
    run_process(t);
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
    extract_file("pnut_js_m2.c", "artifacts/pnut_js_m2.c");
    extract_file("/pnut_js.exe", "artifacts/pnut_js.exe");
    extract_file("foo.o", "artifacts/foo.o");
    extract_file("/hex0-orig2", "artifacts/hex0-orig2");
    exit(error_code);
  }
}

int vm_mmap() {
  int r;
  int size = regs_data[2];
  int loc = regs_data[1];
  trap_syscalls_off();
  printf("mmap not impl eax: %d ebx: %d ecx: %d edx: %d esi: %d edi: %d ebp: %d\n", regs_data[0], regs_data[1], regs_data[2], regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
  printf("mmap size: %d\n", size);
  if(loc == 0) {
    r = vm_brk();
    printf("brk: %x\n",r);
    vm_brk(r+size);
  } else {
    printf("mmap addr must be null\n");
    exit(1);
  }
  trap_syscalls_on();
  return r;
}

int vm_unlink() {
  int pathname = regs_data[1];
  trap_syscalls_off();
  printf("unlink %s\n", pathname);
  exit(1);
  trap_syscalls_on();
  // dummy impl since opening a file for write always creates a new file
  return 0;
}

extract_file(vfs_name, real_name) {
  int t;
  t = find_file(vfs_name);
  if(t != 0) {
    int ofile=fopen(real_name, "wb");
    fwrite(gfd_get_file_addr(t), 1, gfd_get_file_length(t), ofile);
    fclose(ofile);
  }
}

int wrap_syscall_alt(edi, esi, ebp, esp, ebx, edx, ecx, eax) {
/*
  printf("in wrap_syscall_alt\n");
  printf("eax\t\t0x%x\n", eax);
  printf("ecx\t\t0x%x\n", ecx);
  printf("edx\t\t0x%x\n", edx);
  printf("ebx\t\t0x%x\n", ebx);
  printf("esp\t\t0x%x\n", esp);
  printf("ebp\t\t0x%x\n", ebp);
  printf("esi\t\t0x%x\n", esi);
  printf("edi\t\t0x%x\n", edi);
*/
  regs_data[0] = eax;
  regs_data[1] = ebx;
  regs_data[2] = ecx;
  regs_data[3] = edx;
  regs_data[4] = esi;
  regs_data[5] = edi;
  regs_data[6] = ebp;
  return wrap_syscall();
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
  } else if(n == 192) {
    r = vm_mmap();
  } else if(n == 10) {
    r = vm_unlink();
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
  if(dbg) {printf("find_file: %s\n", filename);}
  filename = absolute_path(filename);
  while(i>3) {
    t = filename_array+(i*filename_size);
    if(dbg) {printf("looking at: %s\n", t);}
    if(strcmp(t, filename) == 0) {
      if(dbg) {printf("found: %s %d\n", filename, i);}
      return i;
    }
    i = i - 1;
  }
  if(dbg) {printf("not found %s\n", filename);}
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
  filename = absolute_path(filename);
  file_addr = file_addr + gfd_get_file_length(next_filenum - 1);
  gfd_set_file_addr(next_filenum, file_addr);
  gfd_set_file_length(next_filenum, 0);
  strcpy(gfn_get_filename(next_filenum), filename);
  next_filenum = next_filenum + 1;
  return next_filenum - 1;
}

load_file(realname, virtualname) {
  printf("load_file: %s virtualname: %s\n", realname, virtualname);
  int f = fopen(realname, "rb");
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

exit_builtin() {
    /* this is a hacky way of 'terminating' the built in command */
    regs_data[1] = 0;
    vm_exit();
}


hex_digit_to_int(c) {
  if((c>='0') && (c <= '9')) {
    c = c - '0';
  } else if((c>='A') && (c <= 'F')) {
    c = 10 + c - 'A';
  } else {
    printf("invalid digit %d\n", c);
    exit(1);
  }
  return c;
}

hex0_compile(src, dst) {
    int ifile;
    int ofile;
    int ilen;
    int olen;
    int ioff;
    int ooff;
    int i;
    int c;
    int t;
    int in_comment;
    int hi = 1;
    trap_syscalls_off();
    printf("hex0 compile: %s %s\n", src, dst);
    ifile = find_file(src);
    ioff =  gfd_get_file_addr(ifile);
    ilen =  gfd_get_file_length(ifile);
    i = 0;
    ofile = new_file(dst);
    ooff = gfd_get_file_addr(ofile);
    olen = 0;
    in_comment = 0;
    while(i < ilen) {
      c = ri8(ioff+i);
      if((c == '#') || (c == ';')) { in_comment = 1;}
      if(in_comment) {
        if(c == '\n') { in_comment = 0;}
      } else {
        if(!((c == ' ') || (c == '\t') || (c == '\n'))) {
          fputc(c, stdout);
          if(hi == 1) {
           hi = 0;
           t = hex_digit_to_int(c);
          } else {
           hi = 1;
           t = t << 4;
           t = t | hex_digit_to_int(c);
           wi8(ooff, t);
           ooff = ooff + 1;
           olen = olen + 1;
//           printf(" t: 0x%x\n", t);
          }
        }
      }
      i = i + 1;
    }
    gfd_set_file_length(ofile, olen);
    /* this is a hacky way of 'terminating' the built in command */
    regs_data[1] = 0;
    vm_exit();
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

run_process(cmdline_) {
/* hack reseve some stack space since reset_process scribbles over the stack */
  int arr[128];
  int o;
  int c;
  int t;
  int args_offset;
  int last_offset;
  int argc;
  int foo;
  int cmdline = cmdline_;

  o = 0;
  args_offset = 2048;
  int *args;
  args = args_base;
  last_offset = args_base+args_offset;

  printf("cmdline: %s 0x%x\n", cmdline, &cmdline);
  reset_process();
  printf("cmdline: %s 0x%x\n", cmdline, &cmdline);

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

  /* dispatch built in commands */
  if(ri8(args[1]) == 'h') {
    /* hex0 foo.hex0 bar.exe (compile foo.hex0 to a binary) */
    hex0_compile(args[2], args[3]);
  } else if(ri8(args[1]) == 'l') {
    /* load_file command */
    load_file(args[2], args[3]);
    exit_builtin();
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
    foo=fopen(args[1], "rb");
    while((c=fgetc(foo))!=-1) {
      wi8(o,c);
      o = o + 1;
    }
    fclose(foo);
  }

  printf("o: %x\n", o);

  brk_ptr = 4096+4096*(o/4096);
  printf("brk_ptr: %x\n", brk_ptr);

  /* hacky way of communicating wrap_syscall_alt address with tracer */
  regs_data[8] = wrap_syscall_alt;
  printf("wrap_syscall_alt address: 0x%x vs regs_data[8] 0x%x\n", wrap_syscall_alt, regs_data[8]);

  trap_syscalls_on();
  asm("mov $0x8045800,%esp");
  asm("mov $0x8048054,%eax");
/* tcc 0.9.27 doesn't support this jmp to address in register */
//  asm("jmp %eax");
  asm(".byte 255");
  asm(".byte 224");
  trap_syscalls_off();

  return 0;
}

init_globals() {
  base_address = 64 * 1024 * 1024;
  file_descriptors = base_address+0x20000+0x200;
  filename_array = base_address+0x0200000;
  file_addr = 256*1024*1024;
  gfds = base_address+0x01000010;


}

int main(int argc, int **argv){
/* big mapping for our heap */
  int res = 0;
  init_globals();
  res = mmap(base_address, 512*1024*1024, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, 0, 0);
  printf("res: %d\n", res);
  if(res == -1 ) {
    printf("mmap error\n");
    exit(1);
  }

  brk_ptr = elf_base;

  load_file("../cjsawk/hello.c", "hello.c");
  load_file("../cjsawk/cjsawk.js", "cjsawk.js");
  load_file("../cjsawk/support_libc.js", "support_libc.js");
  load_file("../cjsawk/simple_support_js_m2_prims.c", "simple_support_js_m2_prims.c");
  load_file("../cjsawk/support_m2.c", "support_m2.c");

  load_file("../pnut_refactor/simple_support_js_m2_prims.c", "pnut_refactor/simple_support_js_m2_prims.c");
  load_file("../pnut_refactor/support_common.c", "pnut_refactor/support_common.c");
  load_file("../pnut_refactor/simple_support_js_m2.js", "pnut_refactor/simple_support_js_m2.js");
  load_file("../pnut_refactor/pnut.js", "pnut_refactor/pnut.js");

  load_file("../m2min_v3/simple_asm_defs.M1", "simple_asm_defs.M1");
  load_file("../m2min_v3/x86_defs.M1", "x86_defs.M1");
  load_file("../m2min_v3/libc-core.M1", "libc-core.M1");
  load_file("../m2min_v3/ELF-i386.hex2", "ELF-i386.hex2");

  load_file("../m2min_v2/hex0_x86.hex0", "hex0_x86.hex0");
  load_file("../m2min_v2/hex1_x86.hex0", "hex1_x86.hex0");
  load_file("../m2min_v2/hex2_x86.hex1", "hex2_x86.hex1");
  load_file("../m2min_v2/catm_x86.hex2", "catm_x86.hex2");
  load_file("../m2min_v2/M0_x86.hex2", "M0_x86.hex2");
  load_file("../m2min_v2/cc_x86_min.M1", "cc_x86_min.M1");

  load_file("../m2min_v3/js_to_c/js_to_c_m2_support.c", "js_to_c_m2_support.c");
  load_file("../m2min_v3/js_to_c/js_to_c_simple.c", "js_to_c_simple.c");

// to test out tcc-pnut
  load_file("../cjsawk/cjsawk.js", "foo.c");

  if(argc > 1) {
    printf("openning command file %s\n", argv[1]);
    command_file = fopen(argv[1], "rb");
  } else {
    command_file = 0;
  }

  run_process(next_command());

  return 0;
}
