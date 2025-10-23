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
  trap_syscalls_off();
  host_puts("vm_write to file not impl");
  host_exit(1);
}

function vm_exit() {
  trap_syscalls_off();
  host_exit(get_reg(1));
}

function wrap_syscall() {
/* needed to set up stack frame correctly when called from tcc generated code */
  return wrap_syscall_();
}
function wrap_syscall_() {
  var r;
  var n;
/*  host_puts(mks("wrap_syscall called")); */
  n = get_reg(0);

  trap_syscalls_off();
  host_fputs(mks("n: "), host_stdout());
  host_fputs(int2str(n, 10, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  trap_syscalls_on();

  if(eq(n, 4)) {
    r = vm_write();
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

function load_boot(filename) {
  var f;
  var o;
  var size;
  f = host_fopen(filename, mks("rb"));
  o = elf_base();
  while(size = host_fread(o, 1, 4096, f)) {
    o = add(o, size);
  }
  host_fclose(f);
  return sub(o, elf_base());
}

function run_process() {
  asm("DEFINE mov_esp, BC");
  asm("DEFINE jmp_indirect FF25");
  asm("mov_esp, %0x8045800");
  /* set up stack pointer */
  asm("mov_esp, %0x8045800");
  /* this is a jmp to the entrypoint, stored in elf_base + 0x18 */
  asm("jmp_indirect %0x8048018");
}

function reloc_entrypoint() {
  int a;
  int l;
  memset(elf_base(), 0, 0x10000);
  a = mks("reloc blah\n");
  host_fputs(a, host_stdout());
  host_fputs(int2str(a,16,0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  l = load_boot(mks("../cjsawk/artifacts/builds/hello/hello.exe"));
/*  l = load_boot(mks("../cjsawk/artifacts/builds/full_cc_x86_min/cjsawk.exe")); */
  host_fputs(mks("file length: "), host_stdout());
  host_fputs(int2str(l, 10, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  trap_syscalls_on();
  run_process();
  trap_syscalls_off();
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
  reloc_self();
  return 0;
}
