int eq(int a, int b){
/*      return a == b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "sete_al"
                "movzx_eax,al"
                "ret"
        );
}

int neq(int a, int b){
/*      return a != b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "setne_al"
                "movzx_eax,al"
                "ret"
        );
}

int lt(int a, int b){
/*      return a < b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "setl_al"
                "movzx_eax,al"
                "ret"
        );
}

int lte(int a, int b){
/*      return a <= b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "setle_al"
                "movzx_eax,al"
                "ret"
        );
}

int gt(int a, int b){
/*      return a > b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "setg_al"
                "movzx_eax,al"
                "ret"
        );
}

int gte(int a, int b){
/*      return a >= b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "cmp"
                "setge_al"
                "movzx_eax,al"
                "ret"
        );
}

int ri8(int o) {
/*
  char *h = 0;
  return h[o] & 0xFF;
*/
        asm(
                "mov_eax, %0"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-12"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "pop_ebx"
                "add_eax,ebx"
                "movsx_eax,BYTE_PTR_[eax]"
                "push_eax"
                "mov_eax, %0xFF"
                "pop_ebx"
                "and_eax,ebx"
                "pop_ebx"
                "ret"
        );
}

int wi8(int o,int v) {
/*
  char *h = 0;
  h[o]=v;
  return;
*/
        asm(
                "mov_eax, %0"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-16"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "pop_ebx"
                "add_eax,ebx"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "mov_[ebx],al"
                "pop_ebx"
                "ret"
        );
}

int add(int a, int b){
/*      return a + b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "add_eax,ebx"
                "ret"
        );
}

int sub(int a, int b){
/*      return a - b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "sub_ebx,eax"
                "mov_eax,ebx"
                "ret"
        );
}

int mul(int a, int b){
/*      return a * b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "imul_ebx"
                "ret"
        );
}

int div(int a, int b){
/*      return a / b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "xchg_ebx,eax"
                "cdq"
                "idiv_ebx"
                "ret"
        );
}

int mod(int a, int b){
/*      return a % b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "xchg_ebx,eax"
                "cdq"
                "idiv_ebx"
                "mov_eax,edx"
                "ret"
        );
}

int and(int a, int b){
/*      return a & b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "and_eax,ebx"
                "ret"
        );
}

int or(int a, int b){
/*      return a | b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "or_eax,ebx"
                "ret"
        );
}

int shl(int a, int b){
/*      return a << b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "mov_ecx,eax"
                "mov_eax,ebx"
                "sal_eax,cl"
                "ret"
        );
}

int shr(int a, int b){
/*      return a >> b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "mov_ecx,eax"
                "mov_eax,ebx"
                "sar_eax,cl"
                "ret"
        );
}

int ri32(int o) {
        return or(or(and(ri8(o), 255),
                shl(and(ri8(add(o, 1)), 255), 8)),
                or(shl(and(ri8(add(o, 2)), 255), 16),
                shl(and(ri8(add(o, 3)), 255), 24)));
}

int wi32(int o, int v) {
  wi8(o, and(v, 0xFF));
  v = shr(v, 8);
  wi8(add(o, 1), and(v, 0xFF));
  v = shr(v, 8);
  wi8(add(o, 2), and(v, 0xFF));
  v = shr(v, 8);
  wi8(add(o, 3), and(v, 0xFF));
}

function get_eip() {
  asm("pop_eax");
  asm("push_eax");
  asm("ret");
}

function mks(s) {
  int r;
  if(gte(get_eip(),elf_base())) {
    r = s;
  } else {
    r = add(base_address(), sub(s, elf_base()));
  }
/*  fputs(int2str(r, 16, 0), 1); */
  return r;
}

int strlen(int str)
{
	int i;
	i = 0;
	while(neq(0, ri8(add(str, i)))) {
		i = add(i, 1);
	}
	return i;
}

int memset(int ptr, int value, int num)
{
	int s;
	s = ptr;
	while(lt(0, num))
	{
		wi8(s, value);
		s = add(s, 1);
		num = sub(num, 1);
	}
}

function match(a, b) {
  var i;
  i = sub(0, 1);
  while(1) {
    i = add(i, 1);
    if(neq(ri8(add(a, i)), ri8(add(b, i)))) {
      return 0;
    }
    if(eq(0,and(neq(0, ri8(add(a,i))), neq(0, ri8(add(b, i)))))){
      break;
    }
  }
  return 1;
}

int int2str(int x, int base, int signed_p) {
	int p;
	int i;
	int sign_p;
	int table;
	/* Be overly conservative and save space for 32binary digits and padding null */
	p = int2str_tmp();
	/* if calloc fails return null to let calling code deal with it */
	if(eq(0, p)) {
		return p;
	}

	p = add(p, 32);
	sign_p = 0;
	table = mks("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	if(and(and(signed_p, eq(10, base)), neq(0, (and(x, 0x80000000))))) {
		/* Truncate to 31bits */
		i = and(sub(0, x), 0x7FFFFFFF);
		if(eq(0, i)) {
			return mks("-2147483648");
		}
		sign_p = 1;
	} else {
		/* Truncate to 32bits */
		i = and(x, or(0x7FFFFFFF, shl(1, 31)));
	}

	while(1) {
		wi8(p, ri8(add(table, mod(i, base))));
		p = sub(p, 1);
		i = div(i, base);
		if(gte(0, i)) {
			break;
		}
	}

	if(sign_p) {
		wi8(p, '-');
		p = sub(p, 1);
	}

	return add(p, 1);
}

function print_labled_hex(s, v) {
  host_fputs(s, host_stdout());
  host_fputs(mks(": 0x"), host_stdout());
  host_fputs(int2str(v, 16, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
}

function print_labled_string(s, v) {
  host_fputs(s, host_stdout());
  host_fputs(mks(": "), host_stdout());
  host_fputs(v, host_stdout());
  host_fputs(mks("\n"), host_stdout());
}

int memcpy(int a, int b, int c) {
  int dest;
  int src;
  int i;
  dest=a;
  src=b;
  i = 0;
  while(lt(i, c)) {
    wi8(add(dest, i), ri8(add(src, i)));
    i = add(i, 1);
  }
}

function host_call() {
  asm("DEFINE call_indirect FF15");
  asm("call_indirect %0x4020024");
}

function enter_reloc() {
  asm("DEFINE call_indirect FF15");
  asm("call_indirect %0x4020050");
}

function trap_syscalls_off() {
  set_param(0, 1);
  return host_call();
}

function trap_syscalls_on() {
  set_param(0, 2);
  return host_call();
}

function host_puts(s) {
  set_param(0, 3);
  set_param(1, s);
  return host_call();
}

function host_fwrite(ptr, size, nitems, stream) {
  set_param(0, 4);
  set_param(1, ptr);
  set_param(2, size);
  set_param(3, nitems);
  set_param(4, stream);
  return host_call();
}

function host_fopen(path, mode) {
  set_param(0, 5);
  set_param(1, path);
  set_param(2, mode);
  return host_call();
}

function host_fread(ptr, size, nmemb, stream) {
  set_param(0, 7);
  set_param(1, ptr);
  set_param(2, size);
  set_param(3, nmemb);
  set_param(4, stream);
  return host_call();
}

function host_fgetc(stream) {
  int t;
  t = host_fread(char_buf(), 1, 1, stream);
  if(neq(t, 1)) {
    return sub(0,1);
  } else {
    return ri8(char_buf());
  }
}

function host_fputc(c, stream) {
  wi8(char_buf(), c);
  host_fwrite(char_buf(), 1, 1, stream);
}

function host_fclose(fp) {
  set_param(0, 6);
  set_param(1, fp);
  return host_call();
}

function host_fputs(s, fd) {
  var l;
  l=strlen(s);
  host_fwrite(s, 1, l, fd);
}

function host_exit(s) {
  set_param(0, 8);
  set_param(1, s);
  host_call();
}

function vm_write() {
  var r;
  var t;
  var fd;
  var buf;
  var count;
  var c2;
  fd = get_reg(1);
  buf = get_reg(2);
  count = get_reg(3);
  c2 = count;
  if(lt(fd, 3)) {
    trap_syscalls_off();
    host_fwrite(buf, 1, count, host_stdout());
    trap_syscalls_on();
    return count;
  }
  t = fd_get_file_offset(fd);
  while(gt(c2, 0)) {
    wi8(add(gfd_get_file_addr(fd_get_filenum(fd)), t), ri8(buf));
    t = add(t, 1);
    fd_set_file_offset(fd, t);
    gfd_set_file_length(fd_get_filenum(fd), t);
    c2 = sub(c2, 1);
    buf = add(buf, 1);
  }
  return count;
}

function vm_exit() {
  /* reset esp back to a known good value (since catm leaves esp in a bad
     state */
  asm("mov_esp, %0x8045700");
  return vm_exit_();
}
function vm_exit_() {
  var t;
  var error_code;
  error_code = get_reg(1);
  trap_syscalls_off();
  print_labled_hex(mks("vm_exit"), error_code);
  print_labled_hex(mks("brk_ptr"), ri32(brk_ptr()));
  if(t = next_command()) {
    run_process(t);
    host_puts(mks("shouldn't get here"));
    host_exit(1);
  } else {
    extract_file(mks("xp_linux_self.exe.M1"), mks("artifacts/xp_linux_self.exe.M1"));
    extract_file(mks("xp_linux_self.exe-0.M1"), mks("artifacts/xp_linux_self.exe-0.M1"));
    extract_file(mks("/xp_linux_self.exe"), mks("artifacts/xp_linux_self.exe"));
    extract_file(mks("artifacts/out2.M1"), mks("artifacts/out2.M1"));
    extract_file(mks("/hex0"), mks("artifacts/hex0"));
    extract_file(mks("/hex1"), mks("artifacts/hex1"));
    extract_file(mks("/hex2-0"), mks("artifacts/hex2-0"));
    extract_file(mks("/catm"), mks("artifacts/catm"));
    extract_file(mks("/M0"), mks("artifacts/M0"));
    extract_file(mks("/cc_x86_min.exe"), mks("artifacts/cc_x86_min.exe"));
    extract_file(mks("/js_to_c.exe"), mks("artifacts/js_to_c.exe"));
    extract_file(mks("cjsawk_boot.exe.M1"), mks("artifacts/cjsawk_boot.exe.M1"));
    extract_file(mks("/cjsawk.exe"), mks("artifacts/cjsawk.exe"));
    extract_file(mks("/m0.exe"), mks("artifacts/m0.exe"));
    extract_file(mks("/hex2.exe"), mks("artifacts/hex2.exe"));
    host_exit(error_code);
  }
}

function vm_brk() {
  var p;
  p = get_reg(1);
/*  print_labled_hex(mks("vm_brk: p"), p); */
  if(neq(p, 0)) {
    wi32(brk_ptr(), p);
  }
/*
  trap_syscalls_off();
  print_labled_hex(mks("vm_brk: "), ri32(brk_ptr()));
  trap_syscalls_on();
*/
  return ri32(brk_ptr());
}

function new_fd(filenum) {
  var t;
  fd_set_filenum(ri32(next_fd()), filenum);
  fd_set_file_offset(ri32(next_fd()), 0);
  wi32(next_fd(), add(ri32(next_fd()), 1));
  t = sub(ri32(next_fd()), 1);
  return t;
}

function vm_open() {
  var filename;
  var flags;
  var mode;
  var t;
  var r;
  filename = get_reg(1);
  flags = get_reg(2);
  mode = get_reg(3);
  flags = and(flags, 0xFFFF);
  mode = and(mode, 0xFFFF);
  trap_syscalls_off();
  host_fputs(mks("open: "), host_stdout());
  host_fputs(filename, host_stdout());
  host_fputs(mks(" "), host_stdout());
  host_fputs(int2str(flags, 10, 0), host_stdout());
  host_fputs(mks(" "), host_stdout());
  host_fputs(int2str(mode, 10, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  /* fixme parse mode properly rather than special case all write modes */
  /* 578 might need special handing as I think it may be append */
  if(and(or(eq(flags, 578), eq(flags, 577)), or(eq(mode, 420), or(eq(mode, 384), or(eq(mode, 448), or(eq(mode, 438), eq(mode, 511))))))) {
    host_fputs(mks("open "), host_stdout());
    host_fputs(filename, host_stdout());
    host_fputs(mks(" for write"), host_stdout());
    host_fputs(mks("\n"), host_stdout());
    t = new_file(filename);
    r = new_fd(t);
  } else {
    host_fputs(mks("open "), host_stdout());
    host_fputs(filename, host_stdout());
    host_fputs(mks(" for read"), host_stdout());
    host_fputs(mks("\n"), host_stdout());
    t = find_file(filename);
    if(t) {
      r = new_fd(t);
    } else {
      r = sub(0, 1);
      host_fputs(mks("vm_open: file not found "), host_stdout());
      host_fputs(filename, host_stdout());
      host_fputs(mks(" returning -1\n"), host_stdout());
    }
  }
  print_labled_hex(mks("open: fd"),r);
  trap_syscalls_on();
  return r;
}

function vm_read() {
  var r;
  var c;
  var fd;
  var buf;
  var count;
  var t;

  fd = get_reg(1);
  buf = get_reg(2);
  count = get_reg(3);
  t = 0;
  if(eq(fd_get_file_offset(fd), gfd_get_file_length(fd_get_filenum(fd)))) {
    r = 0;
  } else {
    while(lt(t, count)) {
      if(eq(fd_get_file_offset(fd), gfd_get_file_length(fd_get_filenum(fd)))) {
        break;
      }
      c = ri8(add(gfd_get_file_addr(fd_get_filenum(fd)),fd_get_file_offset(fd)));
      fd_set_file_offset(fd, add(fd_get_file_offset(fd), 1));
      wi8(buf, c);
      buf = add(buf, 1);
      t = add(t, 1);
    }
    r = t;
  }
  return r;
}

function vm_close() {
  int r;
  int t;
  int tn;
  int fd;
  fd = get_reg(1);
  trap_syscalls_off();
  print_labled_hex(mks("close"), fd);
  t = fd_get_filenum(fd);
  tn = gfn_get_filename(t);
  print_labled_hex(mks("close t"), t);
  print_labled_string(mks("close tn"), tn);
  trap_syscalls_on();
  return r;
}

function vm_lseek() {
  var fd;
  var offset;
  var whence;
  fd = get_reg(1);
  offset = get_reg(2);
  whence = get_reg(3);
  trap_syscalls_off();
  if(eq(whence, 0)) {
    /* nothing needed */
  } else if(eq(whence, 2)){
    /* SEEK_END */
    offset = add(offset, gfd_get_file_length(fd_get_filenum(fd)));
  } else if(eq(whence, 1)){
    /* SEEK_CUR */
    offset = add(offset, fd_get_file_offset(fd));
  } else {
    host_puts(mks("vm_lseek invalid whence"));
    host_exit(1);
  }
  print_labled_hex(mks("vm_lseek fd:"), fd);
  print_labled_hex(mks("vm_lseek offset:"), offset);
  print_labled_hex(mks("vm_lseek whence:"), whence);
  fd_set_file_offset(fd, offset);
  trap_syscalls_on();
  return offset;
}

function wrap_syscall() {
/* needed to set up stack frame correctly when called from tcc generated code */
/* also need to preserve more registers since some of the early tool use them */
  asm("push_edi");
  asm("push_ebx");
  asm("push_ecx");
  asm("push_edx");
  wrap_syscall_();
  asm("pop_edx");
  asm("pop_ecx");
  asm("pop_ebx");
  asm("pop_edi");
}
function wrap_syscall_() {
  var r;
  var n;
/*  host_puts(mks("wrap_syscall called")); */
  n = get_reg(0);
/*
  trap_syscalls_off();
  host_fputs(mks("n: "), host_stdout());
  host_fputs(int2str(n, 10, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  trap_syscalls_on();
*/
  if(eq(n, 45)) {
    r = vm_brk();
  } else if(eq(n, 3)) {
    r = vm_read();
  } else if(eq(n, 4)) {
    r = vm_write();
  } else if(eq(n, 19)) {
    r = vm_lseek();
  } else if(eq(n, 5)) {
    r = vm_open();
  } else if(eq(n, 6)) {
    r = vm_close();
  } else if(eq(n, 1)) {
    r = vm_exit();
  } else {
    trap_syscalls_off();
    host_fputs(mks("unsupported syscall: "), host_stdout());
    host_fputs(int2str(n, 10, 0), host_stdout());
    host_fputs(mks("\n"), host_stdout());
    host_exit(1);
  }
  return r;
}

function wrap_syscall_addr() {
  asm("mov_eax, &FUNCTION_wrap_syscall");
  asm("ret");
}

function get_afb() {
  /* ideally I would declare this in globals.js but mk_asm relies on
   * globals.js being unchanged from this point (I should fix this)
   */
  return dummy_global();
}

function absolute_path(filename) {
  var i;
  var j;
  var t;
  var afb;
  i = 0;
  j = 0;
  afb = get_afb();
  while(lt(j, 1024)) {
    wi8(add(afb, j),0);
    j = add(j, 1);
  }
  j = 0;
  wi8(afb,'/');
  j = add(j, 1);
  if(eq(ri8(filename), '/')) {
    i = add(i, 1);
  }
  while(neq((t=ri8(add(filename, i))), 0)) {
    if(eq(t, '.')) {
      i = add(i, 1);
      t = ri8(add(filename, i));
      if(eq(t, '.')) {
        i = add(i, 1);
        t = ri8(add(filename, i));
        if(eq(t, '/')) {
          i = add(i, 1);
          t = ri8(add(filename, i));
          while(and(neq(ri8(add(afb, j)), '/'), gte(j, 0))) {
            j = sub(j, 1);
          }
          j = sub(j, 1);
/*
          if(j < 0) { j = 0;}
          while((ri8(afb+j) != '/')&& (j>=0)){
            j = j - 1;
          }
          j = j + 1;
*/
        } else {
          host_puts(mks("absolute_path error"));
          exit(1);
        }
      } else if(eq(t, '/')) {
        i = add(i, 1);
      } else {
        i = sub(i, 1);
        t = ri8(add(filename, i));
      }
    }
    wi8(add(afb, j), t);
    i = add(i, 1);
    j = add(j, 1);
  }
  wi8(add(afb, j), 0);


  print_labled_string(mks("afb"), afb);
/* FIXME remove this once port is done */
  afb = filename;

  return afb;
}

function find_file(filename) {
  int t;
  int i;
  i = sub(ri32(next_filenum()),  1);
  host_fputs(mks("find_file: "), host_stdout());
  host_fputs(filename, host_stdout());
  host_fputs(mks("\n"), host_stdout());
  filename = absolute_path(filename);
  while(gt(i, 3)) {
    t = add(filename_array(),mul(i, filename_size()));
    host_fputs(mks("looking at: "), host_stdout());
    host_fputs(t, host_stdout());
    host_fputs(mks("\n"), host_stdout());
    if(match(t, filename)) {
      host_fputs(mks("found: "), host_stdout());
      host_fputs(t, host_stdout());
      host_fputs(mks("\n"), host_stdout());
      return i;
    }
    i = sub(i, 1);
  }
  host_fputs(mks("not found: "), host_stdout());
  host_fputs(filename, host_stdout());
  host_fputs(mks("\n"), host_stdout());
  return 0;
}

function print_file_addr() {
    host_fputs(mks("file_addr: 0x"), host_stdout());
    host_fputs(int2str(ri32(file_addr()), 16, 0), host_stdout());
    host_fputs(mks("\n"), host_stdout());
}

function new_file(filename) {
  var l_prev;
  filename = absolute_path(filename);
  l_prev = gfd_get_file_length(sub(ri32(next_filenum()), 1));
  if(eq(l_prev, 0)) {
    host_puts(mks("new_file: applying hacky work around to open multiple files for write\n"));
    wi32(file_addr(), add(ri32(file_addr()), mul(1024, 1024)));
  }
  wi32(file_addr(),add(ri32(file_addr()), l_prev));
  gfd_set_file_addr(ri32(next_filenum()), ri32(file_addr()));
  gfd_set_file_length(ri32(next_filenum()), 0);
  memcpy(gfn_get_filename(ri32(next_filenum())), filename, add(strlen(filename), 1));
  wi32(next_filenum(), add(ri32(next_filenum()), 1));
  return sub(ri32(next_filenum()), 1);
}


function load_file(realname, virtualname) {
  int f;
  int c;
  int t;
  int size;
  int ptr;
  host_fputs(mks("load_file: realname: "), host_stdout());
  host_fputs(realname, host_stdout());
  host_fputs(mks(" virtualname: "), host_stdout());
  host_fputs(virtualname, host_stdout());
  host_fputs(mks("\n"), host_stdout());
  f = host_fopen(realname, mks("rb"));
  t = new_file(virtualname);
  while(1) {
    ptr = add(ri32(file_addr()), gfd_get_file_length(t));
    size = host_fread(ptr, 1, 4096, f);
    if(eq(size, 0)) {
      break;
    }
    gfd_set_file_length(t, add(gfd_get_file_length(t), size));
  }
  host_fclose(f);
  host_fputs(mks("file_length: "), host_stdout());
  host_fputs(int2str(gfd_get_file_length(t), 10, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  return t;
}

function hex_digit_to_int(c) {
  if(and(gte(c, '0'), lte(c, '9'))) {
    c = sub(c, '0');
  } else if(and(gte(c,'A'), lte(c, 'F'))) {
    c = add(10, sub(c, 'A'));
  } else {
    print_labled_hex(mks("invalid digit"), c);
    host_exit(1);
  }
  return c;
}

function hex0_compile(src, dst) {
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
  int hi;
  hi = 1;

  host_fputs(mks("hex0 compile: "), host_stdout());
  host_fputs(src, host_stdout());
  host_fputs(mks(" "), host_stdout());
  host_fputs(dst, host_stdout());
  host_fputs(mks("\n"), host_stdout());
  ifile = find_file(src);
  ioff =  gfd_get_file_addr(ifile);
  ilen =  gfd_get_file_length(ifile);
  i = 0;
  ofile = new_file(dst);
  ooff = gfd_get_file_addr(ofile);
  olen = 0;
  in_comment = 0;
  while(lt(i, ilen)) {
    c = ri8(add(ioff, i));
    if(or(eq(c, '#'), eq(c, ';'))) { in_comment = 1;}
    if(in_comment) {
      if(eq(c, '\n')) { in_comment = 0;}
    } else {
      if(eq(or(eq(c, ' '), or(eq(c, '\t'), eq(c, '\n'))),0)) {
        host_fputc(c, host_stdout());
        if(eq(hi, 1)) {
          hi = 0;
          t = hex_digit_to_int(c);
        } else {
          hi = 1;
          t = shl(t, 4);
          t = or(t, hex_digit_to_int(c));
          wi8(ooff, t);
          ooff = add(ooff, 1);
          olen = add(olen, 1);
//         printf(" t: 0x%x\n", t);
        }
      }
    }
    i = add(i, 1);
  }
  gfd_set_file_length(ofile, olen);
}

function exit_builtin() {
  /* this is a hacky way of 'terminating' the built in command */
  set_reg(1, 0);
  vm_exit();
}

function extract_file(vfs_name, real_name) {
  var t;
  var ofile;
  t = find_file(vfs_name);
  if(neq(t, 0)) {
    ofile = host_fopen(real_name, mks("wb"));
    host_fwrite(gfd_get_file_addr(t), 1, gfd_get_file_length(t), ofile);
    host_fclose(ofile);
  }
}

function next_command() {
  var o;
  var c;
  o = 0;
  while(neq((c = host_fgetc(ri32(command_file()))), sub(0,1))) {
    if(eq(c, 10)) {break;}
    wi8(add(command_buffer(), o), c);
    o = add(o, 1);
  }
  wi8(add(command_buffer(), o), 0);
  print_labled_string(mks("command_buffer"), command_buffer());
  if(eq(o, 0)) { return 0;}
  return command_buffer();
}

function reset_process() {
  int base_addr;
  int i;
  int upper_addr;
  base_addr = args_base();
  upper_addr = ri32(brk_ptr());
  host_fputs(mks("reset_memory: 0x"), host_stdout());
  host_fputs(int2str(base_addr, 16,0), host_stdout());
  host_fputs(mks(" to 0x"), host_stdout());
  host_fputs(int2str(upper_addr, 16,0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  i = base_addr;
  while(lte(i, upper_addr)) {
    wi8(i,0);
    i = add(i, 1);
  }
/* reset file descriptors */
  wi32(next_fd(), 4);
}

function run_process(cmdline) {
  int o;
  int c;
  int t;
  int args_offset;
  int last_offset;
  int argc;
  int foo;
  int i;
  int args;
  int p;
  int l;
  int e_phoff;
  int e_phnum;
  int e_phentsize;
  int pheader;
  int p_offset;
  int p_vaddr;
  int p_filesz;
  int j;
  int c0;

  args_offset = 2048;
  args = args_base();
  last_offset = add(args_base(), args_offset);

  host_fputs(mks("cmdline: "), host_stdout());
  host_fputs(cmdline, host_stdout());
  host_fputs(mks(" addr: 0x"), host_stdout());
  host_fputs(int2str(cmdline, 16, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  reset_process();
  host_fputs(mks("cmdline: "), host_stdout());
  host_fputs(cmdline, host_stdout());
  host_fputs(mks("\n"), host_stdout());

  o = 0;
  while(1) {
    c = ri8(add(cmdline, o));
    t = c;
    if(or(eq(t, ' '), eq(t, 0))) {
      t = 0;
      argc = add(ri32(args),1);
      wi32(args, argc);
      wi32(add(args, mul(argc, 4)), last_offset);
      last_offset = add(add(args_base(), args_offset), add(o,1));
    }
    wi8(add(add(args_base(), args_offset), o), t);
    if(eq(c, 0)) {
      break;
    }
    o = add(o, 1);
  }
  wi32(add(args, mul(add(argc, 1), 4)), 0);
  i = 0;

  host_fputs(mks("argc: "), host_stdout());
  host_fputs(int2str(argc, 10, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  while(lt(i, argc)) {
    host_fputs(mks("run_process arg["), host_stdout());
    host_fputs(int2str(i, 10,0), host_stdout());
    host_fputs(mks("] "), host_stdout());
    host_fputs(ri32(add(args, mul(add(i, 1), 4))), host_stdout());
    host_fputs(mks("\n"), host_stdout());
    i = add(i, 1);
  }

  /* dispatch built in commands */
  c0 = ri8(ri32(add(args, 4)));
  if(eq(c0, 'h')) {
    /* hex0 foo.hex0 bar.exe (compile foo.hex0 to a binary) */
    hex0_compile(ri32(add(args, 8)), ri32(add(args, 12)));
    exit_builtin();
  } else if(eq(c0, 'l')) {
    /* load_file command */
    load_file(ri32(add(args, 8)), ri32(add(args, 12)));
    exit_builtin();
  }

  if(foo = find_file(ri32(add(args, 4)))) {
    host_fputs(mks("run_process: in vfs found "), host_stdout());
    host_fputs(ri32(add(args, 4)), host_stdout());
    host_fputs(mks("\n"), host_stdout());
  } else {
    foo = load_file(ri32(add(args, 4)), ri32(add(args, 4)));
  }

  p = gfd_get_file_addr(foo);
  l = gfd_get_file_length(foo);

  print_labled_hex(mks("e_entry"), ri32(add(elf_base(), 0x18)));
  e_phoff = ri32(add(p, 0x1C));
  print_labled_hex(mks("e_phoff"), e_phoff);
  e_phnum = and(ri32(add(p, 0x2C)), 0xFF);
  print_labled_hex(mks("e_phnum"), e_phnum);
  e_phentsize = and(ri32(add(p, 0x2A)), 0xFF);
  print_labled_hex(mks("e_phentsize"), e_phentsize);
  if(neq(e_phentsize, 0x20)){
    host_fputs(mks("invalid e_phentsize\n"), host_stdout());
    host_exit(1);
  }

  i = 0;
  while(lt(i, e_phnum)) {
    pheader = add(p, add(e_phoff, mul(i, e_phentsize)));
    p_offset = ri32(add(pheader, 0x4));
    p_vaddr = ri32(add(pheader, 0x8));
    p_filesz = ri32(add(pheader, 0x10));
    print_labled_hex(mks("pheader"), i);
    print_labled_hex(mks("p_offset"), p_offset);
    print_labled_hex(mks("p_vaddr"), p_vaddr);
    print_labled_hex(mks("p_filesz"), p_filesz);
    j = 0;
    while(lt(j, p_filesz)) {
      wi8(add(p_vaddr, j), ri8(add(p, add(p_offset, j))));
      j = add(j, 1);
    }
    i = add(i, 1);
    wi32(brk_ptr(), add(4096, mul(4096, div(add(p_vaddr, p_filesz),4096))));
  }

  /* shouldn't happen but cc_x86_min.exe has an incorrect p_filesz which I
     think will then cause garbage to be appended at the end of the in memory
     image (which then causes cc_x86 to crash) */
  j = l;
  while(lt(j, p_filesz)) {
    wi8(add(p_vaddr, j), 0);
    j = add(j, 1);
  }

  print_labled_hex(mks("brk_ptr"), ri32(brk_ptr()));

  trap_syscalls_on();
  asm("DEFINE mov_esp, BC");
  asm("DEFINE jmp_indirect FF25");
  asm("mov_esp, %0x8045800");
  /* set up stack pointer */
  asm("mov_esp, %0x8045800");
  /* this is a jmp to the entrypoint, stored in elf_base + 0x18 */
  asm("jmp_indirect %0x8048018");
  trap_syscalls_off();
  host_puts("run_process_shouln't get here\n");
  host_exit(123);
}

function reloc_entrypoint() {
  int a;
  int l;
  int c;
  memset(elf_base(), 0, 0x10000);
  a = mks("reloc blah\n");
  host_fputs(a, host_stdout());
  host_fputs(int2str(a,16,0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  host_fputs(mks("file length: "), host_stdout());
  host_fputs(int2str(l, 10, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  wi32(brk_ptr(), elf_base());

  /* load in some test files */
  /* FIXME this is copied from test_tracee.c but we should not rely on these
   * files being preloaded
   */

  load_file(mks("../cjsawk/hello.c"), mks("hello.c"));
  load_file(mks("../cjsawk/cjsawk.js"), mks("cjsawk.js"));
  load_file(mks("../cjsawk/support_libc.js"), mks("support_libc.js"));
  load_file(mks("../cjsawk/simple_support_js_m2_prims.c"), mks("simple_support_js_m2_prims.c"));
  load_file(mks("../cjsawk/support_m2.c"), mks("support_m2.c"));

  load_file(mks("../pnut_refactor/simple_support_js_m2_prims.c"), mks("pnut_refactor/simple_support_js_m2_prims.c"));
  load_file(mks("../pnut_refactor/support_common.c"), mks("pnut_refactor/support_common.c"));
  load_file(mks("../pnut_refactor/simple_support_js_m2.js"), mks("pnut_refactor/simple_support_js_m2.js"));
  load_file(mks("../pnut_refactor/pnut.js"), mks("pnut_refactor/pnut.js"));

  load_file(mks("../m2min_v3/simple_asm_defs.M1"), mks("simple_asm_defs.M1"));
  load_file(mks("../m2min_v3/x86_defs.M1"), mks("x86_defs.M1"));
  load_file(mks("../m2min_v3/libc-core.M1"), mks("libc-core.M1"));
  load_file(mks("../m2min_v3/ELF-i386.hex2"), mks("ELF-i386.hex2"));

  load_file(mks("../m2min_v2/hex0_x86.hex0"), mks("hex0_x86.hex0"));
  load_file(mks("../m2min_v2/hex1_x86.hex0"), mks("hex1_x86.hex0"));
  load_file(mks("../m2min_v2/hex2_x86.hex1"), mks("hex2_x86.hex1"));
  load_file(mks("../m2min_v2/catm_x86.hex2"), mks("catm_x86.hex2"));
  load_file(mks("../m2min_v2/M0_x86.hex2"), mks("M0_x86.hex2"));
  load_file(mks("../m2min_v2/cc_x86_min.M1"), mks("cc_x86_min.M1"));

  load_file(mks("../m2min_v3/js_to_c/js_to_c_m2_support.c"), mks("js_to_c_m2_support.c"));
  load_file(mks("../m2min_v3/js_to_c/js_to_c_simple.c"), mks("js_to_c_simple.c"));

  load_file(mks("../cjsawk/cjsawk.js"), mks("foo.c"));

  run_process(next_command());
  host_exit(0);
}

function reloc_entrypoint_addr() {
  asm("mov_eax, &FUNCTION_reloc_entrypoint");
  asm("ret");
}

function reloc_self() {
  host_fputs(mks("reloc_self\n"), host_stdout());
  memcpy(base_address(), elf_base(), 0x10000);
  wi32(syscall_hook(), add(base_address(), sub(wrap_syscall_addr(),elf_base())));
  wi32(reloc_entrypoint_ptr(), add(base_address(), sub(reloc_entrypoint_addr(),elf_base())));
  enter_reloc();
}

function main(argc, argv) {
  wi32(syscall_hook(), wrap_syscall_addr());
  wi32(next_filenum(), 4);
  wi32(file_addr(), mul(256, mul(1024,1024)));
  reloc_self();
  return 0;
}
