function add(a, b) {
/*	return a + b; */
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

function sub(a, b) {
/*	return a - b; */
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

function mul(a, b) {
/*	return a * b; */
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

function shl(a, b) {
/*	return a << b; */
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

function shr(a, b) {
/*	return a >> b; */
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

function lt(a, b) {
/*	return a < b; */
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

function gt(a, b) {
/*	return a > b; */
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

function lte(a, b) {
/*	return a <= b; */
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

function gte(a, b) {
/*	return a >= b; */
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

function and(a, b) {
/*	return a & b; */
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

function or(a, b) {
/*	return a | b; */
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

function ri8(o) {
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

function eq(a, b) {
/*	return a == b; */
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

function neq(a, b) {
/*	return a != b; */
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

function xor(a, b) {
/*	return a ^ b; */
	asm(
		"lea_eax,[ebp+DWORD] %-4"
		"mov_eax,[eax]"
		"push_eax"
		"lea_eax,[ebp+DWORD] %-8"
		"mov_eax,[eax]"
		"pop_ebx"
		"xor_eax,ebx"
		"ret"
	);
}

function wi8(o, v) {
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

function div(a, b) {
/*	return a / b; */
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

function mod(a, b) {
/*	return a % b; */
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

function fgetc(f) {
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

function fputc(s, f) {
	asm("mov_eax, %4"
	    "lea_ebx,[esp+DWORD] %4"
	    "mov_ebx,[ebx]"
	    "lea_ecx,[esp+DWORD] %8"
	    "mov_edx, %1"
	    "int !0x80");
}

function open(name, flag, mode) {
	asm("lea_ebx,[esp+DWORD] %12"
	    "mov_ebx,[ebx]"
	    "lea_ecx,[esp+DWORD] %8"
	    "mov_ecx,[ecx]"
	    "lea_edx,[esp+DWORD] %4"
	    "mov_edx,[edx]"
	    "mov_eax, %5"
	    "int !0x80");
}

function close(fd) {
	asm("lea_ebx,[esp+DWORD] %4"
	    "mov_ebx,[ebx]"
	    "mov_eax, %6"
	    "int !0x80");
}

function brk(addr) {
	asm("mov_eax,[esp+DWORD] %4"
	    "push_eax"
	    "mov_eax, %45"
	    "pop_ebx"
	    "int !0x80");
}

function exit(value) {
	asm("pop_ebx"
	    "pop_ebx"
	    "mov_eax, %1"
	    "int !0x80");
}
