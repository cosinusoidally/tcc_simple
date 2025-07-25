#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>

#include "mujs.h"

#include <stdint.h>

#include <dlfcn.h>

uint8_t* heap;

static char *xoptarg; /* Global argument pointer. */
static int xoptind = 0; /* Global argv index. */
static int xgetopt(int argc, char *argv[], char *optstring)
{
	static char *scan = NULL; /* Private scan pointer. */

	char c;
	char *place;

	xoptarg = NULL;

	if (!scan || *scan == '\0') {
		if (xoptind == 0)
			xoptind++;

		if (xoptind >= argc || argv[xoptind][0] != '-' || argv[xoptind][1] == '\0')
			return EOF;
		if (argv[xoptind][1] == '-' && argv[xoptind][2] == '\0') {
			xoptind++;
			return EOF;
		}

		scan = argv[xoptind]+1;
		xoptind++;
	}

	c = *scan++;
	place = strchr(optstring, c);

	if (!place || c == ':') {
		fprintf(stderr, "%s: unknown option -%c\n", argv[0], c);
		return '?';
	}

	place++;
	if (*place == ':') {
		if (*scan != '\0') {
			xoptarg = scan;
			scan = NULL;
		} else if (xoptind < argc) {
			xoptarg = argv[xoptind];
			xoptind++;
		} else {
			fprintf(stderr, "%s: option requires argument -%c\n", argv[0], c);
			return ':';
		}
	}

	return c;
}

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#else
void using_history(void) { }
void add_history(const char *string) { }
void rl_bind_key(int key, void (*fun)(void)) { }
void rl_insert(void) { }
char *readline(const char *prompt)
{
	static char line[500], *p;
	int n;
	fputs(prompt, stdout);
	p = fgets(line, sizeof line, stdin);
	if (p) {
		n = strlen(line);
		if (n > 0 && line[n-1] == '\n')
			line[--n] = 0;
		p = malloc(n+1);
		memcpy(p, line, n+1);
		return p;
	}
	return NULL;
}
#endif

#define PS1 "> "

static void jsB_gc(js_State *J)
{
	int report = js_toboolean(J, 1);
	js_gc(J, report);
	js_pushundefined(J);
}

static void jsB_load(js_State *J)
{
	int i, n = js_gettop(J);
	for (i = 1; i < n; ++i) {
		js_loadfile(J, js_tostring(J, i));
		js_pushundefined(J);
		js_call(J, 0);
		js_pop(J, 1);
	}
	js_pushundefined(J);
}

static void jsB_compile(js_State *J)
{
	const char *source = js_tostring(J, 1);
	const char *filename = js_isdefined(J, 2) ? js_tostring(J, 2) : "[string]";
	js_loadstring(J, filename, source);
}

static void jsB_print(js_State *J)
{
	int i, top = js_gettop(J);
	for (i = 1; i < top; ++i) {
		const char *s = js_tostring(J, i);
		if (i > 1) putchar(' ');
		fputs(s, stdout);
	}
	putchar('\n');
	js_pushundefined(J);
}

static void jsB_write(js_State *J)
{
	int i, top = js_gettop(J);
	for (i = 1; i < top; ++i) {
		const char *s = js_tostring(J, i);
		if (i > 1) putchar(' ');
		fputs(s, stdout);
	}
	js_pushundefined(J);
}

static void jsB_read(js_State *J)
{
	const char *filename = js_tostring(J, 1);
	FILE *f;
	char *s;
	int n, t;

	f = fopen(filename, "rb");
	if (!f) {
		js_error(J, "cannot open file '%s': %s", filename, strerror(errno));
	}

	if (fseek(f, 0, SEEK_END) < 0) {
		fclose(f);
		js_error(J, "cannot seek in file '%s': %s", filename, strerror(errno));
	}

	n = ftell(f);
	if (n < 0) {
		fclose(f);
		js_error(J, "cannot tell in file '%s': %s", filename, strerror(errno));
	}

	if (fseek(f, 0, SEEK_SET) < 0) {
		fclose(f);
		js_error(J, "cannot seek in file '%s': %s", filename, strerror(errno));
	}

	s = malloc(n + 1);
	if (!s) {
		fclose(f);
		js_error(J, "out of memory");
	}

	t = fread(s, 1, n, f);
	if (t != n) {
		free(s);
		fclose(f);
		js_error(J, "cannot read data from file '%s': %s", filename, strerror(errno));
	}
	s[n] = 0;

	js_pushstring(J, s);
	free(s);
	fclose(f);
}

static void jsB_readline(js_State *J)
{
	char *line = readline("");
	if (!line) {
		js_pushnull(J);
		return;
	}
	js_pushstring(J, line);
	if (*line)
		add_history(line);
	free(line);
}

static void jsB_quit(js_State *J)
{
	exit(js_tonumber(J, 1));
}

static void jsB_repr(js_State *J)
{
	js_repr(J, 1);
}

/* simple_c dialect builtins */

static void jsB_add(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r=a+b;
	js_pushnumber(J,(double)r);
}

static void jsB_sub(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r=a-b;
	js_pushnumber(J,(double)r);
}

static void jsB_mul(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r=a*b;
	js_pushnumber(J,(double)r);
}

static void jsB_shl(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r=a << b;
	js_pushnumber(J,(double)r);
}

static void jsB_shr(js_State *J)
{
	unsigned int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r=a >> b;
	js_pushnumber(J,(double)r);
}

static void jsB_lt(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = a < b;
	js_pushnumber(J,(double)r);
}

static void jsB_gt(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = a > b;
	js_pushnumber(J,(double)r);
}

static void jsB_lte(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = a <= b;
	js_pushnumber(J,(double)r);
}

static void jsB_gte(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = a >= b;
	js_pushnumber(J,(double)r);
}

static void jsB_and(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = a & b;
	js_pushnumber(J,(double)r);
}

static void jsB_or(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = a | b;
	js_pushnumber(J,(double)r);
}

static void jsB_eq(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = (a == b);
	js_pushnumber(J,(double)r);
}

static void jsB_neq(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = (a != b);
	js_pushnumber(J,(double)r);
}

static void jsB_xor(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = (a ^ b);
	js_pushnumber(J,(double)r);
}

static void jsB_div(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = (a / b);
	js_pushnumber(J,(double)r);
}

static void jsB_mod(js_State *J)
{
	int a;
	int b;
	int r;
	a=js_toint32(J,1);
	b=js_toint32(J,2);
	r = (a % b);
	js_pushnumber(J,(double)r);
}

static void jsB_is_string(js_State *J)
{
	js_pushboolean(J, js_isstring(J, 1));
}

static void jsB_str_len(js_State *J)
{
	js_pushnumber(J, (double)js_getlength(J, 1));
}

/* real heap functions */

static void jsB_wm8(js_State *J)
{
	int o;
	int v;
	uint8_t *h = 0;
	o=js_toint32(J,1);
	v=js_toint32(J,2);
	h[o]=v;
//	printf("wi8 o: %d v: %d\n",o,v);
	js_pushundefined(J);
}

static void jsB_rm8(js_State *J)
{
	int o;
	int v;
	uint8_t *h = 0;
	o=js_toint32(J,1);
	v=h[o];
//	printf("ri8 o: %d is v: %d\n",o,v);
	js_pushnumber(J,(double)v);
}


/* virtual heap functions */

static void jsB_wi8(js_State *J)
{
	int o;
	int v;
	o=js_toint32(J,1);
	v=js_toint32(J,2);
	heap[o]=v;
//	printf("wi8 o: %d v: %d\n",o,v);
	js_pushundefined(J);
}

static void jsB_ri8(js_State *J)
{
	int o;
	int v;
	o=js_toint32(J,1);
	v=heap[o];
//	printf("ri8 o: %d is v: %d\n",o,v);
	js_pushnumber(J,(double)v);
}

static void jsB_wi32(js_State *J)
{
/* FIXME make cleaner */
	int o;
	int v;
	int *p;
	o=js_toint32(J,1);
	v=js_toint32(J,2);
	p = (int)heap + o;
	p[0]=v;
//	printf("wi32 o: %d v: %d\n",o,v);
	js_pushundefined(J);
}

static void jsB_ri32(js_State *J)
{
	int o;
	int v;
	int *p;
	o=js_toint32(J,1);
/* FIXME make cleaner */
	p = (int)heap + o;
	v=p[0];
//	printf("ri32 o: %d is v: %d\n",o,v);
	js_pushnumber(J,(double)v);
}

typedef int (* my_ffi_stub)(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

void my_ffi_call(js_State *J) {
	int i;
	int ptr;
	double ret;
        int args[8];
	for(i = 0; i<8; i = i + 1) {
		args[i]=js_toint32(J,2+i);
	}
	ptr = js_toint32(J,1);
//	printf("ptr: %x arg: %x\n", ptr, args[0]);
	__asm__("and $0xfffffff0,%esp");
	ret=(double)(((my_ffi_stub)ptr)(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7]));
	js_pushnumber(J,ret);
}

static void str_adr(js_State *J)
{
	int str;
	str=js_tostring(J,1);
//	printf("str_adr 0x%x is str: %s\n",str,str);
	js_pushnumber(J,(double)str);
}

int _add(int a, int b){
	return a+b;
}

static int eval_print(js_State *J, const char *source)
{
	if (js_ploadstring(J, "[stdin]", source)) {
		fprintf(stderr, "%s\n", js_trystring(J, -1, "Error"));
		js_pop(J, 1);
		return 1;
	}
	js_pushundefined(J);
	if (js_pcall(J, 0)) {
		fprintf(stderr, "%s\n", js_trystring(J, -1, "Error"));
		js_pop(J, 1);
		return 1;
	}
	if (js_isdefined(J, -1)) {
		printf("%s\n", js_tryrepr(J, -1, "can't convert to string"));
	}
	js_pop(J, 1);
	return 0;
}

static char *read_stdin(void)
{
	int n = 0;
	int t = 512;
	char *s = NULL;

	for (;;) {
		char *ss = realloc(s, t);
		if (!ss) {
			free(s);
			fprintf(stderr, "cannot allocate storage for stdin contents\n");
			return NULL;
		}
		s = ss;
		n += fread(s + n, 1, t - n - 1, stdin);
		if (n < t - 1)
			break;
		t *= 2;
	}

	if (ferror(stdin)) {
		free(s);
		fprintf(stderr, "error reading stdin\n");
		return NULL;
	}

	s[n] = 0;
	return s;
}

static void usage(void)
{
	fprintf(stderr, "Usage: mujs [options] [script [scriptArgs*]]\n");
	fprintf(stderr, "\t-i: Enter interactive prompt after running code.\n");
	fprintf(stderr, "\t-s: Check strictness.\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	char *input;
	js_State *J;
	int status = 0;
	int strict = 0;
	int interactive = 0;
	int i, c;

	while ((c = xgetopt(argc, argv, "is")) != -1) {
		switch (c) {
		default: usage(); break;
		case 'i': interactive = 1; break;
		case 's': strict = 1; break;
		}
	}

	J = js_newstate(NULL, NULL, strict ? JS_STRICT : 0);

	js_newcfunction(J, jsB_gc, "gc", 0);
	js_setglobal(J, "gc");

	js_newcfunction(J, jsB_load, "load", 1);
	js_setglobal(J, "load");

	js_newcfunction(J, jsB_compile, "compile", 2);
	js_setglobal(J, "compile");

	js_newcfunction(J, jsB_print, "print", 0);
	js_setglobal(J, "print");

	js_newcfunction(J, jsB_write, "write", 0);
	js_setglobal(J, "write");

	js_newcfunction(J, jsB_read, "read", 1);
	js_setglobal(J, "read");

	js_newcfunction(J, jsB_readline, "readline", 0);
	js_setglobal(J, "readline");

	js_newcfunction(J, jsB_repr, "repr", 0);
	js_setglobal(J, "repr");

	js_newcfunction(J, jsB_quit, "quit", 1);
	js_setglobal(J, "quit");

/* define simple_c builtins */

	js_newcfunction(J, jsB_add, "add", 2);
	js_setglobal(J, "add");

	js_newcfunction(J, jsB_sub, "sub", 2);
	js_setglobal(J, "sub");

	js_newcfunction(J, jsB_mul, "mul", 2);
	js_setglobal(J, "mul");

	js_newcfunction(J, jsB_shl, "shl", 2);
	js_setglobal(J, "shl");

	js_newcfunction(J, jsB_shr, "shr", 2);
	js_setglobal(J, "shr");

	js_newcfunction(J, jsB_lt, "lt", 2);
	js_setglobal(J, "lt");

	js_newcfunction(J, jsB_gt, "gt", 2);
	js_setglobal(J, "gt");

	js_newcfunction(J, jsB_lte, "lte", 2);
	js_setglobal(J, "lte");

	js_newcfunction(J, jsB_gte, "gte", 2);
	js_setglobal(J, "gte");

	js_newcfunction(J, jsB_and, "and", 2);
	js_setglobal(J, "and");

	js_newcfunction(J, jsB_or, "or", 2);
	js_setglobal(J, "or");

	js_newcfunction(J, jsB_eq, "eq", 2);
	js_setglobal(J, "eq");

	js_newcfunction(J, jsB_neq, "neq", 2);
	js_setglobal(J, "neq");

	js_newcfunction(J, jsB_xor, "xor", 2);
	js_setglobal(J, "xor");

	js_newcfunction(J, jsB_div, "div", 2);
	js_setglobal(J, "div");

	js_newcfunction(J, jsB_mod, "mod", 2);
	js_setglobal(J, "mod");

	js_newcfunction(J, jsB_is_string, "is_string", 1);
	js_setglobal(J, "is_string");

	js_newcfunction(J, jsB_str_len, "str_len", 1);
	js_setglobal(J, "str_len");

/* FIXME this should be set from JS */
	heap = calloc(128*1024*1024, 1);
	js_pushnumber(J, (double)((int)heap));
	js_setglobal(J, "heap");

	js_newcfunction(J, jsB_wi8, "wi8", 2);
	js_setglobal(J, "wi8");

	js_newcfunction(J, jsB_ri8, "ri8", 1);
	js_setglobal(J, "ri8");

	js_newcfunction(J, jsB_wi32, "_wi32", 2);
	js_setglobal(J, "_wi32");

	js_newcfunction(J, jsB_ri32, "_ri32", 1);
	js_setglobal(J, "_ri32");

	js_newcfunction(J, jsB_wm8, "wm8", 2);
	js_setglobal(J, "wm8");

	js_newcfunction(J, jsB_rm8, "rm8", 1);
	js_setglobal(J, "rm8");

/* ffi function */
	js_newcfunction(J, my_ffi_call, "ffi", 9);
	js_setglobal(J, "ffi");

	js_newcfunction(J, str_adr, "str_adr", 1);
	js_setglobal(J, "str_adr");

	js_pushnumber(J, (double)((int)dlopen));
	js_setglobal(J, "_dlopen");

	js_pushnumber(J, (double)((int)dlsym));
	js_setglobal(J, "_dlsym");

	js_pushnumber(J, (double)(RTLD_LAZY));
	js_setglobal(J, "RTLD_LAZY");

/* tmp test */
	js_pushnumber(J, (double)((int)_add));
	js_setglobal(J, "_add");

	if (xoptind == argc) {
		interactive = 1;
	} else {
		c = xoptind++;

		js_newarray(J);
		i = 0;
		while (xoptind < argc) {
			js_pushstring(J, argv[xoptind++]);
			js_setindex(J, -2, i++);
		}
		js_setglobal(J, "scriptArgs");

		if (js_dofile(J, argv[c]))
			status = 1;
	}

	if (interactive) {
		if (isatty(0)) {
			using_history();
			rl_bind_key('\t', rl_insert);
			input = readline(PS1);
			while (input) {
				eval_print(J, input);
				if (*input)
					add_history(input);
				free(input);
				input = readline(PS1);
			}
			putchar('\n');
		} else {
			input = read_stdin();
			if (!input || !js_dostring(J, input))
				status = 1;
			free(input);
		}
	}

	js_gc(J, 0);
	js_freestate(J);

	return status;
}
