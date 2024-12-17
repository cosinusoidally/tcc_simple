/*
 *  TCC - Tiny C Compiler
 *
 *  Copyright (c) 2001-2004 Fabrice Bellard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define _GNU_SOURCE
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <inttypes.h>

extern int O_BINARY;

/* -------------------------------------------- */

#include "libtcc.h"
#include "elf.h"

/* -------------------------------------------- */

#define PUB_FUNC

#define ST_INLN 
#define ST_FUNC 
#define ST_DATA 

/* -------------------------------------------- */
/* include the target specific definitions */

/* number of available registers */
#define NB_REGS         5

/* a register can belong to several classes. The classes must be
   sorted from more general to more precise (see gv2() code which does
   assumptions on it). */
extern int RC_INT;  /*  0x0001 generic integer register */
extern int RC_EAX;   /*  0x0004 */
extern int RC_ECX;   /*  0x0010 */

extern int RC_IRET;    /* RC_EAX function return: integer register */

/* pretty names for the registers */
enum {
    TREG_EAX = 0,
    TREG_ECX,
    TREG_EDX,
    TREG_EBX,
    TREG_ST0,
    TREG_ESP = 4
};

/* return registers for function */
#define REG_IRET TREG_EAX /* single word int return register */

/* pointer size, in bytes */
#define PTR_SIZE 4

/* maximum alignment (for aligned attribute support) */
#define MAX_ALIGN     8

#define EM_TCC_TARGET EM_386

/* relocation type for 32 bit data relocation */
#define R_DATA_32   R_386_32
#define R_DATA_PTR  R_386_32
#define R_RELATIVE  R_386_RELATIVE

#define ELF_START_ADDR 0x08048000
#define ELF_PAGE_SIZE  0x1000

/* -------------------------------------------- */

# define ELFCLASSW ELFCLASS32
# define ElfW(type) Elf##32##_##type
# define ELFW(type) ELF##32##_##type
# define ElfW_Rel ElfW(Rel)
extern int SHT_RELX; /* = SHT_REL */
extern int REL_SECTION_FMT; /* ".rel%s" */
/* target address type */
#define addr_t ElfW(Addr)
#define ElfSym ElfW(Sym)

/* -------------------------------------------- */

#define VSTACK_SIZE         256
#define STRING_MAX_SIZE     1024
#define TOKSTR_MAX_SIZE     256

#define TOK_HASH_SIZE       16384 /* must be a power of two */
#define TOK_ALLOC_INCR      512  /* must be a power of two */
#define TOK_MAX_SIZE        4 /* token max size in int unit when stored in string */

/* token symbol management */
typedef struct TokenSym {
    struct TokenSym *hash_next;
    struct Sym *sym_identifier; /* direct pointer to identifier */
    int tok; /* token number */
    int len;
    char str[1];
} TokenSym;

typedef struct CString {
    int size; /* size in bytes */
    void *data; /* either 'char *' or 'nwchar_t *' */
    int size_allocated;
} CString;

/* type definition */
typedef struct CType {
    int t;
    struct Sym *ref;
} CType;

/* constant value */
typedef union CValue {
    int i;
    struct {
        int size;
        const void *data;
    } str;
    int tab[1];
} CValue;

/* value on stack */
typedef struct SValue {
    CType type;      /* type */
    int r;      /* register + flags */
    CValue c;              /* constant, if VT_CONST */
    struct Sym *sym;       /* symbol, if (VT_SYM | VT_CONST), or if
    			      result of unary() for an identifier. */
} SValue;

/* function attributes or temporary attributes for parsing */
typedef struct FuncAttr {
    int func_type; /* FUNC_OLD/NEW/ELLIPSIS */
} FuncAttr;

/* GNUC attribute definition */
typedef struct AttributeDef {
    struct FuncAttr f;
    struct Section *section;
} AttributeDef;

/* symbol management */
typedef struct Sym {
    int v; /* symbol token */
    int r; /* associated register or VT_CONST/VT_LOCAL and LVAL type */
    int c; /* associated number or Elf symbol index */
    int sym_scope; /* scope level for locals */
    struct FuncAttr f; /* function attributes */
    CType type; /* associated type */
    struct Sym *next; /* next related symbol (for fields and anoms) */
    struct Sym *prev; /* prev symbol in stack */
    struct Sym *prev_tok; /* previous symbol for this token */
} Sym;

/* section definition */
typedef struct Section {
    unsigned long data_offset; /* current data offset */
    unsigned char *data;       /* section data */
    unsigned long data_allocated; /* used for realloc() handling */
    int sh_name;             /* elf section name (only used during output) */
    int sh_num;              /* elf section number */
    int sh_type;             /* elf section type */
    int sh_flags;            /* elf section flags */
    int sh_info;             /* elf section info */
    int sh_addralign;        /* elf section alignment */
    int sh_entsize;          /* elf entry size */
    unsigned long sh_size;   /* section size (only used during output) */
    addr_t sh_addr;          /* address at which the section is relocated */
    unsigned long sh_offset; /* file offset */
    int nb_hashed_syms;      /* used to resize the hash table */
    struct Section *link;    /* link to another section */
    struct Section *reloc;   /* corresponding section for relocation, if any */
    struct Section *hash;    /* hash table for symbols */
    struct Section *prev;    /* previous section on section stack */
    char name[1];           /* section name */
} Section;

/* -------------------------------------------------- */

#define SYM_STRUCT     0x40000000 /* struct/union/enum symbol space */
extern int SYM_FIELD;  /*    0x20000000 struct/union field symbol space */
#define SYM_FIRST_ANOM 0x10000000 /* first anonymous sym */

/* stored in 'Sym->f.func_type' field */
#define FUNC_NEW       1 /* ansi function prototype */
#define FUNC_OLD       2 /* old function prototype */

extern int IO_BUF_SIZE; /* 8192 */

typedef struct BufferedFile {
    uint8_t *buf_ptr;
    uint8_t *buf_end;
    int fd;
    struct BufferedFile *prev;
    int line_num;    /* current line number - here to simplify code */
    char filename[1024];    /* filename */
    unsigned char unget[4];
    unsigned char buffer[1]; /* extra size for CH_EOB char */
} BufferedFile;

extern int CH_EOB; /* '\\'   end of buffer or '\0' char in file */
#define CH_EOF   (-1)   /* end of file */

/* used to record tokens */
typedef struct TokenString {
    int *str;
    int len;
    int lastlen;
    int allocated_len;
    /* used to chain token-strings with begin/end_macro() */
    struct TokenString *prev;
    const int *prev_ptr;
    char alloc;
} TokenString;

struct TCCState {

    addr_t text_addr; /* address of text section */
    int has_text_addr;

    unsigned section_align; /* section alignment */

    /* sections */
    Section **sections;
    int nb_sections; /* number of sections, including first dummy section */

    Section **priv_sections;
    int nb_priv_sections; /* number of private sections */

    /* copy of the global symtab_section variable */
    Section *symtab;

    /* used by main and tcc_parse_args only */
    struct filespec **files; /* files seen on command line */
    int nb_files; /* number thereof */
    int filetype;
    char *outfile; /* output filename */
    int argc;
    char **argv;
};

struct filespec {
    char type;
    char name[1];
};

/* The current value can be: */
extern int VT_VALMASK;  /* mask for value location, register or: */
extern int VT_CONST;    /* constant in vc (must be first non register value) */
#define VT_LLOCAL    0x0031  /* lvalue, offset on stack */
extern int VT_LOCAL;  /* offset on stack */
extern int VT_CMP;     /*  0x0033  the value is stored in processor flags (in vc) */
extern int VT_LVAL;  /*    0x0100  var is an lvalue */
#define VT_SYM       0x0200  /* a symbol value is added */

/* types */
#define VT_BTYPE       0x000f  /* mask for basic type */
#define VT_BYTE             1  /* signed byte type */
#define VT_INT              3  /* integer type */
#define VT_PTR              5  /* pointer */
#define VT_FUNC             6  /* function type */

#define VT_UNSIGNED    0x0010  /* unsigned type */
#define VT_ARRAY       0x0040  /* array type (also has VT_PTR) */
#define VT_CONSTANT    0x0100  /* const modifier */

/* storage */
#define VT_EXTERN  0x00001000  /* extern definition */
#define VT_STATIC  0x00002000  /* static variable */
/* currently unused: 0x000[1248]0000  */

/* type mask (except storage) */
#define VT_STORAGE (VT_EXTERN | VT_STATIC)
#define VT_TYPE (~(VT_STORAGE))

/* token values */

/* warning: the following compare tokens depend on i386 asm code */
#define TOK_NE  0x95

/* tokens that carry values (in additional token string space / tokc) --> */
#define TOK_CCHAR   0xb3 /* char constant in tokc */
#define TOK_CINT    0xb5 /* number in tokc */
#define TOK_CUINT   0xb6 /* unsigned int constant */
#define TOK_STR     0xb9 /* pointer to string in tokc */
#define TOK_PPNUM   0xbe /* preprocessor number */
#define TOK_PPSTR   0xbf /* preprocessor string */
/* <-- */

#define TOK_EOF       (-1)  /* end of file */
#define TOK_LINEFEED  10    /* line feed */

/* all identifiers and strings have token above that */
#define TOK_IDENT 256

enum tcc_token {
    TOK_LAST = TOK_IDENT - 1
#define DEF(id, str) ,id
#include "tcctok.h"
#undef DEF
};

/* keywords: tok >= TOK_IDENT && tok < TOK_UIDENT */
#define TOK_UIDENT TOK_DEFINE

/* ------------ libtcc.c ------------ */

/* XXX: get rid of this ASAP */
extern struct TCCState *tcc_state;

/* public functions currently used by the tcc main function */
ST_FUNC char *pstrcpy(char *buf, int buf_size, const char *s);
ST_FUNC char *pstrcat(char *buf, int buf_size, const char *s);
ST_FUNC char *pstrncpy(char *out, const char *in, size_t num);
PUB_FUNC char *tcc_basename(const char *name);
PUB_FUNC char *tcc_fileextension (const char *name);

PUB_FUNC int tcc_free(int ptr);
PUB_FUNC int tcc_malloc(int size);
PUB_FUNC int tcc_mallocz(int size);
PUB_FUNC int tcc_realloc(int ptr, int size);
PUB_FUNC int tcc_strdup(int str);

PUB_FUNC void tcc_memcheck(void);
PUB_FUNC void tcc_error_noabort(const char *fmt, ...);
PUB_FUNC void tcc_warning(const char *fmt, ...);

/* other utilities */
int dynarray_add(int ptab, int nb_ptr, int data);
int dynarray_reset(int pp, int n);
ST_INLN void cstr_ccat(CString *cstr, int ch);
ST_FUNC void cstr_cat(CString *cstr, const char *str, int len);
ST_FUNC void cstr_wccat(CString *cstr, int ch);
ST_FUNC void cstr_new(CString *cstr);
ST_FUNC void cstr_free(CString *cstr);
ST_FUNC void cstr_reset(CString *cstr);

ST_INLN void sym_free(Sym *sym);
ST_FUNC Sym *sym_push2(Sym **ps, int v, int t, int c);
ST_FUNC Sym *sym_find2(Sym *s, int v);
ST_FUNC Sym *sym_push(int v, CType *type, int r, int c);
ST_FUNC void sym_pop(Sym **ptop, Sym *b, int keep);
ST_INLN Sym *struct_find(int v);
ST_INLN Sym *sym_find(int v);
ST_FUNC Sym *global_identifier_push(int v, int t, int c);

ST_FUNC void tcc_open_bf(TCCState *s1, const char *filename, int initlen);
ST_FUNC int tcc_open(TCCState *s1, const char *filename);
ST_FUNC void tcc_close(void);

ST_FUNC int tcc_add_file_internal(TCCState *s1, const char *filename, int flags);

ST_FUNC int tcc_add_crt(TCCState *s, const char *filename);
ST_FUNC int tcc_add_dll(TCCState *s, const char *filename, int flags);
ST_FUNC void tcc_add_pragma_libs(TCCState *s1);
PUB_FUNC int tcc_add_library_err(TCCState *s, const char *f);
PUB_FUNC void tcc_print_stats(TCCState *s, unsigned total_time);

/* ------------ tccpp.c ------------ */

ST_DATA struct BufferedFile *file;
ST_DATA int ch, tok;
ST_DATA CValue tokc;
ST_DATA const int *macro_ptr;
ST_DATA int parse_flags;
extern int tok_flags;
ST_DATA CString tokcstr; /* current parsed string, if any */

/* display benchmark infos */
ST_DATA int total_bytes;
ST_DATA int tok_ident;
ST_DATA TokenSym **table_ident;

extern int TOK_FLAG_BOL; /*  0x0001 beginning of line before */
extern int TOK_FLAG_BOF; /*  0x0002 beginning of file before */

#define PARSE_FLAG_PREPROCESS 0x0001 /* activate preprocessing */
#define PARSE_FLAG_TOK_NUM    0x0002 /* return numbers instead of TOK_PPNUM */
#define PARSE_FLAG_LINEFEED   0x0004 /* line feed is returned as a
                                        token. line feed is also
                                        returned at eof */
#define PARSE_FLAG_SPACES     0x0010 /* next() returns space tokens (for -E) */
#define PARSE_FLAG_TOK_STR    0x0040 /* return parsed strings instead of TOK_PPSTR */

/* isidnum_table flags: */
#define IS_SPC 1
#define IS_ID  2
#define IS_NUM 4

ST_FUNC TokenSym *tok_alloc(const char *str, int len);
ST_FUNC const char *get_tok_str(int v, CValue *cv);
ST_FUNC void begin_macro(TokenString *str, int alloc);
ST_FUNC void end_macro(void);
ST_FUNC int set_idnum(int c, int val);
ST_INLN void tok_str_new(TokenString *s);
ST_FUNC TokenString *tok_str_alloc(void);
ST_FUNC void tok_str_free(TokenString *s);
ST_FUNC void tok_str_free_str(int *str);
ST_FUNC void tok_str_add(TokenString *s, int t);
ST_FUNC void tok_str_add_tok(TokenString *s);
ST_INLN void define_push(int v, int macro_type, int *str, Sym *first_arg);
ST_FUNC void define_undef(Sym *s);
ST_INLN Sym *define_find(int v);
ST_FUNC void free_defines(Sym *b);
ST_FUNC Sym *label_find(int v);
ST_FUNC Sym *label_push(Sym **ptop, int v, int flags);
ST_FUNC void label_pop(Sym **ptop, Sym *slast, int keep);
ST_FUNC void parse_define(void);
ST_FUNC void preprocess(int is_bof);
ST_FUNC void next_nomacro(void);
ST_FUNC void next(void);
ST_INLN void unget_tok(int last_tok);
ST_FUNC void preprocess_start(TCCState *s1, int is_asm);
ST_FUNC void preprocess_end(TCCState *s1);
void tccpp_new(TCCState *s);
ST_FUNC void tccpp_delete(TCCState *s);
ST_FUNC int tcc_preprocess(TCCState *s1);
ST_FUNC void skip(int c);

/* space excluding newline */
static inline int is_space(int ch) {
    return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r';
}
static inline int isid(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
static inline int isnum(int c) {
    return c >= '0' && c <= '9';
}
static inline int toup(int c) {
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}

/* ------------ tccgen.c ------------ */

#define SYM_POOL_NB (8192 / sizeof(Sym))
ST_DATA Sym *sym_free_first;
ST_DATA void **sym_pools;
ST_DATA int nb_sym_pools;

ST_DATA Sym *global_stack;
ST_DATA Sym *local_stack;
ST_DATA Sym *local_label_stack;
ST_DATA Sym *global_label_stack;
ST_DATA CType char_pointer_type, func_old_type, int_type, size_type;
ST_DATA SValue __vstack[1+/*to make bcheck happy*/ VSTACK_SIZE], *pvtop;
extern SValue *vtop;
#define vstack  (__vstack + 1)
ST_DATA int rsym, anon_sym;
extern int loc;

ST_DATA int const_wanted; /* true if constant wanted */
ST_DATA int global_expr;  /* true if compound literals must be allocated globally (used during initializers parsing */
ST_DATA CType func_vt; /* current function return type (used by return instruction) */
ST_DATA int func_var; /* true if current function is variadic */
ST_DATA int func_vc;
ST_DATA int last_line_num, last_ind, func_ind; /* debug last line number and pc */
ST_DATA const char *funcname;
ST_DATA int g_debug;

ST_FUNC void tcc_debug_start(TCCState *s1);
ST_FUNC void tcc_debug_end(TCCState *s1);
ST_FUNC void tcc_debug_funcstart(TCCState *s1, Sym *sym);
ST_FUNC void tcc_debug_funcend(TCCState *s1, int size);
ST_FUNC void tcc_debug_line(TCCState *s1);

ST_FUNC int tccgen_compile(TCCState *s1);
ST_FUNC void free_inline_functions(TCCState *s);
ST_FUNC void check_vstack(void);

ST_INLN int is_float(int t);
ST_FUNC int ieee_finite(double d);
ST_FUNC void test_lvalue(void);
ST_FUNC void vpushi(int v);
ST_FUNC ElfSym *elfsym(Sym *);
ST_FUNC void update_storage(Sym *sym);
ST_FUNC Sym *external_global_sym(int v, CType *type, int r);
ST_FUNC void vset(CType *type, int r, int v);
ST_FUNC void vswap(void);
ST_FUNC void vpush_global_sym(CType *type, int v);
ST_FUNC void vrote(SValue *e, int n);
ST_FUNC void vrott(int n);
ST_FUNC void vrotb(int n);
ST_FUNC void vpushv(SValue *v);
ST_FUNC void save_reg(int r);
ST_FUNC void save_reg_upstack(int r, int n);
ST_FUNC int get_reg(int rc);
ST_FUNC void save_regs(int n);
ST_FUNC void gaddrof(void);
ST_FUNC int gv(int rc);
ST_FUNC void gv2(int rc1, int rc2);
ST_FUNC void vpop(void);
ST_FUNC void gen_op(int op);
ST_FUNC int type_size(CType *type, int *a);
ST_FUNC void mk_pointer(CType *type);
ST_FUNC void vstore(void);
ST_FUNC void inc(int post, int c);
ST_FUNC void parse_mult_str (CString *astr, const char *msg);
ST_FUNC void parse_asm_str(CString *astr);
ST_FUNC int lvalue_type(int t);
ST_FUNC void indir(void);
ST_FUNC void unary(void);
ST_FUNC void expr_prod(void);
ST_FUNC void expr_sum(void);
ST_FUNC void gexpr(void);
ST_FUNC int expr_const(void);

/* ------------ tccelf.c ------------ */

ST_DATA Section *text_section, *data_section, *bss_section; /* predefined sections */
ST_DATA Section *common_section;
extern Section *cur_text_section; /* current section where function code is generated */
ST_DATA Section *last_text_section; /* to handle .previous asm directive */
/* symbol sections */
ST_DATA Section *symtab_section;
/* debug sections */
ST_DATA Section *stab_section, *stabstr_section;

void tccelf_new(TCCState *s);
ST_FUNC void tccelf_delete(TCCState *s);
ST_FUNC void tccelf_stab_new(TCCState *s);
ST_FUNC void tccelf_begin_file(TCCState *s1);
ST_FUNC void tccelf_end_file(TCCState *s1);

ST_FUNC Section *new_section(TCCState *s1, char *name, int sh_type, int sh_flags);
ST_FUNC void section_realloc(Section *sec, unsigned long new_size);
ST_FUNC size_t section_add(Section *sec, addr_t size, int align);
ST_FUNC void *section_ptr_add(Section *sec, addr_t size);
ST_FUNC void section_reserve(Section *sec, int size);
ST_FUNC Section *find_section(TCCState *s1, const char *name);
ST_FUNC Section *new_symtab(TCCState *s1, const char *symtab_name, int sh_type, int sh_flags, const char *strtab_name, const char *hash_name, int hash_sh_flags);

ST_FUNC void put_extern_sym2(Sym *sym, int sh_num, addr_t value, unsigned long size, int can_add_underscore);
ST_FUNC void put_extern_sym(Sym *sym, Section *section, addr_t value, unsigned long size);
ST_FUNC void greloc(Section *s, Sym *sym, unsigned long offset, int type);
ST_FUNC void greloca(Section *s, Sym *sym, unsigned long offset, int type, addr_t addend);

ST_FUNC int put_elf_str(Section *s, const char *sym);
ST_FUNC int put_elf_sym(Section *s, addr_t value, unsigned long size, int info, int other, int shndx, const char *name);
ST_FUNC int set_elf_sym(Section *s, addr_t value, unsigned long size, int info, int other, int shndx, const char *name);
ST_FUNC int find_elf_sym(Section *s, const char *name);
ST_FUNC void put_elf_reloc(Section *symtab, Section *s, unsigned long offset, int type, int symbol);
ST_FUNC void put_elf_reloca(Section *symtab, Section *s, unsigned long offset, int type, int symbol, addr_t addend);

ST_FUNC void put_stabs(const char *str, int type, int other, int desc, unsigned long value);
ST_FUNC void put_stabs_r(const char *str, int type, int other, int desc, unsigned long value, Section *sec, int sym_index);
ST_FUNC void put_stabn(int type, int other, int desc, int value);
ST_FUNC void put_stabd(int type, int other, int desc);

ST_FUNC void resolve_common_syms(TCCState *s1);
ST_FUNC void relocate_syms(TCCState *s1, Section *symtab, int do_resolve);
ST_FUNC void relocate_section(TCCState *s1, Section *s);

ST_FUNC int tcc_object_type(int fd, ElfW(Ehdr) *h);
ST_FUNC int tcc_load_object_file(TCCState *s1, int fd, unsigned long file_offset);
ST_FUNC int tcc_load_archive(TCCState *s1, int fd);
ST_FUNC void tcc_add_bcheck(TCCState *s1);
ST_FUNC void tcc_add_runtime(TCCState *s1);

ST_FUNC void build_got_entries(TCCState *s1);
ST_FUNC void squeeze_multi_relocs(Section *sec, size_t oldrelocoffset);

ST_FUNC addr_t get_elf_sym_addr(TCCState *s, const char *name, int err);
ST_FUNC void *tcc_get_symbol_err(TCCState *s, const char *name);

ST_FUNC int tcc_load_dll(TCCState *s1, int fd, const char *filename, int level);
ST_FUNC int tcc_load_ldscript(TCCState *s1);
ST_FUNC uint8_t *parse_comment(uint8_t *p);
ST_FUNC void minp(void);
ST_INLN void inp(void);
ST_FUNC int handle_eob(void);

/* ------------ xxx-gen.c ------------ */

extern int reg_classes;

int gsym_addr(int t, int a);
int gsym(int t);
int load(int r, SValue *sv);
int store(int r, SValue *v);
int gfunc_sret(CType *vt, int variadic, CType *ret, int *align, int *regsize);
int gfunc_call(int nb_args);
int gfunc_prolog(int func_type);
int gfunc_epilog(void);
int gjmp(int t);
int gjmp_addr(int a);
int gtst(int inv, int t);
int gtst_addr(int inv, int a);
int gen_opi(int op);
int o(int c);

static inline uint16_t read16le(unsigned char *p) {
    return p[0] | (uint16_t)p[1] << 8;
}
static inline void write16le(unsigned char *p, uint16_t x) {
    p[0] = x & 255;  p[1] = x >> 8 & 255;
}
uint32_t read32le(unsigned char *p) {
  return read16le(p) | (uint32_t)read16le(p + 2) << 16;
}
void write32le(unsigned char *p, uint32_t x) {
    write16le(p, x);  write16le(p + 2, x >> 16);
}

/********************************************************/

extern int sizeof_TCCState;
extern int sizeof_SValue;
extern int FUNC_PROLOG_SIZE;
extern int sizeof_void;
extern int sizeof_BufferedFile;
extern int sizeof_BufferedFile_filename;
extern int sizeof_filespec;
extern int sizeof_TCCOption;
extern int sizeof_Section;
extern int sizeof_Elf32_Ehdr;
extern int sizeof_Elf32_Phdr;
extern int sizeof_Elf32_Rel;
extern int sizeof_Elf32_Sym;


extern int aglobal_stack;
extern int alocal_stack;
