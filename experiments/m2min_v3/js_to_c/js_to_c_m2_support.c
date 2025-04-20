/* Copyright (C) 2016 Jeremiah Orians
 * This file is part of M2-Planet.
 *
 * M2-Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * M2-Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2-Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

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
void exit(int value);

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
	int error = close(stream);
	return error;
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


int strlen(char* str )
{
	int i = 0;
	while(0 != str[i]) {
		i = i + 1;
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

void* calloc(int count, int size)
{
	void* ret = malloc(count * size);
	if(NULL == ret) return NULL;
	memset(ret, 0, (count * size));
	return ret;
}

void free(void* l)
{
	return;
}

void exit(int value)
{
	asm("pop_ebx"
	    "pop_ebx"
	    "mov_eax, %1"
	    "int !0x80");
}

void require(int bool, char* error)
{
	if(!bool)
	{
		fputs(error, stderr);
		exit(1);
	}
}


int match(char* a, char* b)
{
	if((NULL == a) && (NULL == b)) return TRUE;
	if(NULL == a) return FALSE;
	if(NULL == b) return FALSE;

	int i = -1;
	do
	{
		i = i + 1;
		if(a[i] != b[i])
		{
			return FALSE;
		}
	} while((0 != a[i]) && (0 !=b[i]));
	return TRUE;
}


int in_set(int c, char* s)
{
	/* NULL set is always false */
	if(NULL == s) return FALSE;

	while(0 != s[0])
	{
		if(c == s[0]) return TRUE;
		s = s + 1;
	}
	return FALSE;
}

/* INTERNAL ONLY */
int __index_number(char* s, char c)
{
	int i = 0;
	while(s[i] != c)
	{
		i = i + 1;
		if(0 == s[i]) return -1;
	}
	return i;
}

/* INTERNAL ONLY */
int __toupper(int c)
{
	if(in_set(c, "abcdefghijklmnopqrstuvwxyz")) return (c & 0xDF);
	return c;
}

/* INTERNAL ONLY */
int __set_reader(char* set, int mult, char* input)
{
	int n = 0;
	int i = 0;
	int hold;
	int negative_p = FALSE;

	if(input[0] == '-')
	{
		negative_p = TRUE;
		i = i + 1;
	}

	while(in_set(input[i], set))
	{
		n = n * mult;
		hold = __index_number(set, __toupper(input[i]));

		/* Input managed to change between in_set and index_number */
		if(-1 == hold) return 0;
		n = n + hold;
		i = i + 1;
	}

	/* loop exited before NULL and thus invalid input */
	if(0 != input[i]) return 0;

	if(negative_p)
	{
		n = 0 - n;
	}

	return n;
}

int strtoint(char *a)
{
	int result = 0;
	/* If NULL string */
	if(0 == a[0])
	{
		result = 0;
	}
	/* Deal with binary */
	else if ('0' == a[0] && 'b' == a[1])
	{
		result = __set_reader("01", 2, a+2);
	}
	/* Deal with hex */
	else if ('0' == a[0] &&  'x' == a[1])
	{
		result = __set_reader("0123456789ABCDEFabcdef", 16, a+2);
	}
	/* Deal with octal */
	else if('0' == a[0])
	{
		result = __set_reader("01234567", 8, a+1);
	}
	/* Deal with decimal */
	else
	{
		result = __set_reader("0123456789", 10, a);
	}

	/* Deal with sign extension for 64bit hosts */
	if(0 != (0x80000000 & result)) result = (0xFFFFFFFF << 31) | result;
	return result;
}

char* int2str(int x, int base, int signed_p)
{
	require(1 < base, "int2str doesn't support a base less than 2\n");
	require(37 > base, "int2str doesn't support a base more than 36\n");
	/* Be overly conservative and save space for 32binary digits and padding null */
	char* p = calloc(34, sizeof(char));
	/* if calloc fails return null to let calling code deal with it */
	if(NULL == p) return p;

	p = p + 32;
	unsigned i;
	int sign_p = FALSE;
	char* table = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if(signed_p && (10 == base) && (0 != (x & 0x80000000)))
	{
		/* Truncate to 31bits */
		i = -x & 0x7FFFFFFF;
		if(0 == i) return "-2147483648";
		sign_p = TRUE;
	} /* Truncate to 32bits */
	else i = x & (0x7FFFFFFF | (1 << 31));

	do
	{
		p[0] = table[i % base];
		p = p - 1;
		i = i / base;
	} while(0 < i);

	if(sign_p)
	{
		p[0] = '-';
		p = p - 1;
	}

	return p + 1;
}
/* decls for real functions */
void *malloc(int size);
int strlen(char *s);
int fopen(int path, int mode);
int fclose(int f);
int fread(int a,int b, int c, int d);
int puts(char *a);
int dlsym(int h, int sym);
int exit(int x);
int memset(int a, int b, int c);
int memcpy(int a, int b, int c);

int p_size;
int verbose;
void puts_num(int x);

int puts(char *a) {
  fputs(a,stdout);
  fputs("\n",stdout);
}

int init_c(void) {
  p_size=4;
  stdin=0;
  stdout=1;
  stderr=3;
  NULL = 0;
  TRUE = 1;
  FALSE = 0;
}

int memcpy(int a, int b, int c) {
  char *dest;
  char *src;
  int i;
  dest=a;
  src=b;
  for(i=0;i<c;i=i+1){
    dest[i]=src[i];
  }
}

