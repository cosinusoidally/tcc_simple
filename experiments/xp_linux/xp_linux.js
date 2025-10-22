/* note this is not yet xp_linux, see ./mk_test/tcc for the actual current
 *  version of xp_linux. This file is a prototype for the "v2" version
 */

int foobar() {
  asm("DEFINE int 9090909090909090909090CD");
}

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

int stdin;
int stdout;
int stderr;
int NULL;
int TRUE;
int FALSE;

int puts(int a);
int exit(int value);

int fgetc(int f)
{
	asm("mov_eax, %3"
	    "lea_ebx,[esp+DWORD] %4"
	    "mov_ebx,[ebx]"
	    "push_ebx"
	    "mov_ecx,esp"
	    "mov_edx, %1"
	    "int !0x80"
	    "test_eax,eax"
	    "pop_eax"
	    "jne %FUNCTION_fgetc_Done"
	    "mov_eax, %-1"
	    ":FUNCTION_fgetc_Done");
}

int fputc(int s, int f)
{
	asm("mov_eax, %4"
	    "lea_ebx,[esp+DWORD] %4"
	    "mov_ebx,[ebx]"
	    "lea_ecx,[esp+DWORD] %8"
	    "mov_edx, %1"
	    "int !0x80");
}

int fputs(int si, int f) {
	while(neq(0, ri8(si))) {
		fputc(ri8(si), f);
		si = add(si, 1);
	}
}


int open(int name, int flag, int mode)
{
	asm("lea_ebx,[esp+DWORD] %12"
	    "mov_ebx,[ebx]"
	    "lea_ecx,[esp+DWORD] %8"
	    "mov_ecx,[ecx]"
	    "lea_edx,[esp+DWORD] %4"
	    "mov_edx,[edx]"
	    "mov_eax, %5"
	    "int !0x80");
}

int fopen(int filename, int mode)
{
	int f;
	if(eq('w', ri8(mode)))
	{ /* 577 is O_WRONLY|O_CREAT|O_TRUNC, 384 is 600 in octal */
		f = open(filename, 577 , 384);
	} else { /* Everything else is a read */
		f = open(filename, 0, 0);
	}

	/* Negative numbers are error codes */
	if(gt(0, f))
	{
		return 0;
	}
	return f;
}


int close(int fd)
{
	asm("lea_ebx,[esp+DWORD] %4"
	    "mov_ebx,[ebx]"
	    "mov_eax, %6"
	    "int !0x80");
}

int fclose(int stream)
{
	int error;
	error = close(stream);
	return error;
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

int brk(int addr)
{
	asm("mov_eax,[esp+DWORD] %4"
	    "push_eax"
	    "mov_eax, %45"
	    "pop_ebx"
	    "int !0x80");
}

int _malloc_ptr;
int _brk_ptr;

int malloc(int size)
{
	int old_malloc;
	if(eq(NULL, _brk_ptr))
	{
		_brk_ptr = brk(0);
		_malloc_ptr = _brk_ptr;
	}

	if(lt(_brk_ptr, add(_malloc_ptr, size)))
	{
		_brk_ptr = brk(add(_malloc_ptr, size));
		if(eq(sub(0,1), _brk_ptr)) return 0;
	}

	old_malloc = _malloc_ptr;
	_malloc_ptr = add(_malloc_ptr, size);
	return old_malloc;
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

int calloc(int count, int size)
{
	int ret;
	ret = malloc(mul(count, size));
	if(eq(NULL, ret)) return NULL;
	memset(ret, 0, mul(count, size));
	return ret;
}


int free(int l)
{
	return;
}

int exit(int value)
{
	asm("pop_ebx"
	    "pop_ebx"
	    "mov_eax, %1"
	    "int !0x80");
}

int require(int bool, int error)
{
	if(eq(0,bool))
	{
		fputs(error, stderr);
		exit(1);
	}
}

int match(int a, int b) {
	int i;
	if(and(eq(NULL, a), eq(NULL, b))) {
		return TRUE;
	}
	if(eq(NULL, a)) {
		return FALSE;
	}
	if(eq(NULL, b)) {
		return FALSE;
	}

	i = sub(0, 1);
	while(1) {
		i = add(i, 1);
		if(neq(ri8(add(a, i)), ri8(add(b, i)))) {
			return FALSE;
		}
		if(eq(0,and(neq(0, ri8(add(a,i))), neq(0, ri8(add(b, i)))))){
			break;
		}
	}
	return TRUE;
}

int in_set(int c, int s) {
	/* NULL set is always false */
	if(eq(NULL, s)) {
		return FALSE;
	}

	while(neq(0, ri8(s))) {
		if(eq(c, ri8(s))) {
			return TRUE;
		}
		s = add(s, 1);
	}
	return FALSE;
}

int int2str(int x, int base, int signed_p) {
	int p;
	int i;
	int sign_p;
	int table;
	/* Be overly conservative and save space for 32binary digits and padding null */
/* FIXME should pick a safer address */
	p = mul(256,mul(1024,1024));
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
			return "-2147483648";
		}
		sign_p = TRUE;
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

int p_size;
int verbose;

int puts(int a) {
  fputs(a,stdout);
  fputs("\n",stdout);
}

int init_c() {
  p_size = 4;
  stdin = 0;
  stdout = 1;
  stderr = 3;
  NULL = 0;
  TRUE = 1;
  FALSE = 0;
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

function base_address() {
  return 0x4000000;
}

function host_params() {
  return 0x4020028;
}

function host_stdout() {
  return ri32(0x4020048);
}

function syscall_hook() {
  return 0x402004C;
}

function regs_data() {
  return 0x4020000;
}

function get_reg(x) {
  return ri32(add(regs_data(), mul(x,4)));
}


function set_param(i, v) {
  wi32(add(host_params(), mul(i, 4)), v);
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

function wrap_syscall() {
/* needed to set up stack frame correctly when called from tcc generated code */
  return wrap_syscall_();
}
function wrap_syscall_() {
  var r;
  var n;
/*  host_puts(mks("wrap_syscall called")); */
  n = get_reg(0);
/*
  host_fputs("n: ", host_stdout());
  host_fputs(int2str(n, 10, 0), host_stdout());
  host_fputs("\n", host_stdout());
*/
  if(eq(n, 4)) {
    r = vm_write();
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

function elf_base() {
  return 0x8048000;
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
  trap_syscalls_on();
  a = mks("reloc blah\n");
  fputs(a, 1);
  fputs(int2str(a,16,0), 1);
  fputs(mks("\n"), 1);
  l = load_boot(mks("../cjsawk/artifacts/builds/hello/hello.exe"));
  host_fputs(mks("file length: "), host_stdout());
  host_fputs(int2str(l, 10, 0), host_stdout());
  host_fputs(mks("\n"), host_stdout());
  trap_syscalls_on();
  run_process();
  trap_syscalls_off();
  exit(0);
}

function reloc_entrypoint_addr() {
  asm("mov_eax, &FUNCTION_reloc_entrypoint");
  asm("ret");
}

function test_reloc() {
  fputs(mks("test_reloc\n"), 1);
  memcpy(base_address(), elf_base(), 0x10000);
  wi32(syscall_hook(), add(base_address(), sub(wrap_syscall_addr(),elf_base())));
  wi32(0x4020050, add(base_address(), sub(reloc_entrypoint_addr(),elf_base())));
  enter_reloc();
}

function main(argc, argv) {
  int i;
  int fi;
  int fo;
  int l;
  i = 0;
  init_c();
  fputs("xp_linux test\n", 1);
  fputs("xp_linux test2\n", 1);
  wi32(syscall_hook(), wrap_syscall_addr());
  trap_syscalls_on();
  fputs("test3\n", 1);
  trap_syscalls_off();
  host_puts("This is a test");
  host_fwrite("Hello\n", 1, 6, host_stdout());
  fi = host_fopen("./artifacts/read_test.txt", "rb");
  l = host_fread(0x10000000, 1, 1024, fi);
  fo = host_fopen("./artifacts/write_test.txt", "wb");
  host_fwrite(0x10000000, 1, l, fo);
  host_fclose(fo);
  test_reloc();
  return 0;
}
