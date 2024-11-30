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

/********************************************************/
/* global variables */

/* loc : local variable index
   ind : output code index
   rsym: return symbol
   anon_sym: anonymous symbol index
*/
ST_DATA int rsym, anon_sym, ind, loc;

ST_DATA Sym *sym_free_first;
ST_DATA void **sym_pools;
ST_DATA int nb_sym_pools;

ST_DATA Sym *global_stack;
ST_DATA Sym *local_stack;
ST_DATA Sym *define_stack;
ST_DATA Sym *global_label_stack;
ST_DATA Sym *local_label_stack;
static int local_scope;
static int in_sizeof;
static int section_sym;

ST_DATA int vlas_in_scope; /* number of VLAs that are currently in scope */
ST_DATA int vla_sp_root_loc; /* vla_sp_loc for SP before any VLAs were pushed */
ST_DATA int vla_sp_loc; /* Pointer to variable holding location to store stack pointer on the stack when modifying stack pointer */

ST_DATA SValue __vstack[1+VSTACK_SIZE], *vtop, *pvtop;

ST_DATA int const_wanted; /* true if constant wanted */
ST_DATA int nocode_wanted; /* no code generation wanted */
#define NODATA_WANTED (nocode_wanted > 0) /* no static data output wanted either */
#define STATIC_DATA_WANTED (nocode_wanted & 0xC0000000) /* only static data output */
ST_DATA int global_expr;  /* true if compound literals must be allocated globally (used during initializers parsing */
ST_DATA CType func_vt; /* current function return type (used by return instruction) */
ST_DATA int func_var; /* true if current function is variadic (used by return instruction) */
ST_DATA int func_vc;
ST_DATA int last_line_num, last_ind, func_ind; /* debug last line number and pc */
ST_DATA const char *funcname;
ST_DATA int g_debug;

ST_DATA CType char_pointer_type, func_old_type, int_type, size_type, ptrdiff_type;

ST_DATA struct switch_t {
    struct case_t {
        int64_t v1, v2;
	int sym;
    } **p; int n; /* list of case ranges */
    int def_sym; /* default symbol */
} *cur_switch; /* current switch */

/* ------------------------------------------------------------------------- */

static void gen_cast(CType *type);
static void gen_cast_s(int t);
static inline CType *pointed_type(CType *type);
static int is_compatible_types(CType *type1, CType *type2);
static int parse_btype(CType *type, AttributeDef *ad);
static CType *type_decl(CType *type, AttributeDef *ad, int *v, int td);
static void init_putv(CType *type, Section *sec, unsigned long c);
static void decl_initializer(CType *type, Section *sec, unsigned long c, int first, int size_only);
static void block(int *bsym, int *csym, int is_expr);
static void decl_initializer_alloc(CType *type, AttributeDef *ad, int r, int has_init, int v, int scope);
static void decl(int l);
static int decl0(int l, int is_for_loop_init, Sym *);
static void expr_eq(void);
static int is_compatible_unqualified_types(CType *type1, CType *type2);
static void vpush(CType *type);
static int gvtst(int inv, int t);
static void skip_or_save_block(TokenString **str);
static void gv_dup(void);

ST_INLN int is_float(int t)
{
    int bt;
    bt = t & VT_BTYPE;
    return bt == VT_LDOUBLE || bt == VT_DOUBLE || bt == VT_FLOAT || bt == VT_QFLOAT;
}

/* ------------------------------------------------------------------------- */
ST_FUNC int tccgen_compile(TCCState *s1)
{
    cur_text_section = NULL;
    funcname = "";
    anon_sym = SYM_FIRST_ANOM;
    section_sym = 0;
    const_wanted = 0;
    nocode_wanted = 0x80000000;

    /* define some often used types */
    int_type.t = VT_INT;
    char_pointer_type.t = VT_BYTE;
    mk_pointer(&char_pointer_type);
    size_type.t = VT_INT | VT_UNSIGNED;
    ptrdiff_type.t = VT_INT;
    func_old_type.t = VT_FUNC;
    func_old_type.ref = sym_push(SYM_FIELD, &int_type, 0, 0);
    func_old_type.ref->f.func_call = FUNC_CDECL;
    func_old_type.ref->f.func_type = FUNC_OLD;

    /* an elf symbol of type STT_FILE must be put so that STB_LOCAL
       symbols can be safely used */
    put_elf_sym(symtab_section, 0, 0,
                ELFW(ST_INFO)(STB_LOCAL, STT_FILE), 0,
                SHN_ABS, file->filename);

    parse_flags = PARSE_FLAG_PREPROCESS | PARSE_FLAG_TOK_NUM | PARSE_FLAG_TOK_STR;
    next();
    decl(VT_CONST);
    return 0;
}

/* ------------------------------------------------------------------------- */
ST_FUNC ElfSym *elfsym(Sym *s)
{
  if (!s || !s->c)
    return NULL;
  return &((ElfSym *)symtab_section->data)[s->c];
}

/* apply storage attributes to Elf symbol */
ST_FUNC void update_storage(Sym *sym)
{
    ElfSym *esym;
    int sym_bind, old_sym_bind;

    esym = elfsym(sym);
    if (!esym)
        return;

    if (sym->type.t & VT_STATIC)
        sym_bind = STB_LOCAL;
    else
        sym_bind = STB_GLOBAL;
    old_sym_bind = ELFW(ST_BIND)(esym->st_info);
}

/* ------------------------------------------------------------------------- */
/* update sym->c so that it points to an external symbol in section
   'section' with value 'value' */

ST_FUNC void put_extern_sym2(Sym *sym, int sh_num,
                            addr_t value, unsigned long size,
                            int can_add_underscore)
{
    int sym_type, sym_bind, info, other, t;
    ElfSym *esym;
    const char *name;
    char buf1[256];

    if (!sym->c) {
        name = get_tok_str(sym->v, NULL);
        t = sym->type.t;
        if ((t & VT_BTYPE) == VT_FUNC) {
            sym_type = STT_FUNC;
        } else {
            sym_type = STT_OBJECT;
        }
        if (t & VT_STATIC)
            sym_bind = STB_LOCAL;
        else
            sym_bind = STB_GLOBAL;
        other = 0;
        info = ELFW(ST_INFO)(sym_bind, sym_type);
        sym->c = put_elf_sym(symtab_section, value, size, info, other, sh_num, name);
    } else {
        esym = elfsym(sym);
        esym->st_value = value;
        esym->st_size = size;
        esym->st_shndx = sh_num;
    }
    update_storage(sym);
}

ST_FUNC void put_extern_sym(Sym *sym, Section *section,
                           addr_t value, unsigned long size)
{
    int sh_num = section ? section->sh_num : SHN_UNDEF;
    put_extern_sym2(sym, sh_num, value, size, 1);
}

/* add a new relocation entry to symbol 'sym' in section 's' */
ST_FUNC void greloca(Section *s, Sym *sym, unsigned long offset, int type,
                     addr_t addend)
{
    int c = 0;

    if (sym) {
        if (0 == sym->c)
            put_extern_sym(sym, NULL, 0, 0);
        c = sym->c;
    }

    /* now we can add ELF relocation info */
    put_elf_reloca(symtab_section, s, offset, type, c, addend);
}

ST_FUNC void greloc(Section *s, Sym *sym, unsigned long offset, int type)
{
    greloca(s, sym, offset, type, 0);
}

/* ------------------------------------------------------------------------- */
/* symbol allocator */
static Sym *__sym_malloc(void)
{
    Sym *sym_pool, *sym, *last_sym;
    int i;

    sym_pool = tcc_malloc(SYM_POOL_NB * sizeof(Sym));
    dynarray_add(&sym_pools, &nb_sym_pools, sym_pool);

    last_sym = sym_free_first;
    sym = sym_pool;
    for(i = 0; i < SYM_POOL_NB; i++) {
        sym->next = last_sym;
        last_sym = sym;
        sym++;
    }
    sym_free_first = last_sym;
    return last_sym;
}

static inline Sym *sym_malloc(void)
{
    Sym *sym;
    sym = sym_free_first;
    if (!sym)
        sym = __sym_malloc();
    sym_free_first = sym->next;
    return sym;
}

ST_INLN void sym_free(Sym *sym)
{
    sym->next = sym_free_first;
    sym_free_first = sym;
}

/* push, without hashing */
ST_FUNC Sym *sym_push2(Sym **ps, int v, int t, int c)
{
    Sym *s;

    s = sym_malloc();
    memset(s, 0, sizeof *s);
    s->v = v;
    s->type.t = t;
    s->c = c;
    /* add in stack */
    s->prev = *ps;
    *ps = s;
    return s;
}

/* find an identifier */
ST_INLN Sym *sym_find(int v)
{
    v -= TOK_IDENT;
    if ((unsigned)v >= (unsigned)(tok_ident - TOK_IDENT))
        return NULL;
    return table_ident[v]->sym_identifier;
}

/* push a given symbol on the symbol stack */
ST_FUNC Sym *sym_push(int v, CType *type, int r, int c)
{
    Sym *s, **ps;
    TokenSym *ts;

    if (local_stack)
        ps = &local_stack;
    else
        ps = &global_stack;
    s = sym_push2(ps, v, type->t, c);
    s->type.ref = type->ref;
    s->r = r;
    /* don't record fields or anonymous symbols */
    /* XXX: simplify */
    if (!(v & SYM_FIELD) && (v & ~SYM_STRUCT) < SYM_FIRST_ANOM) {
        /* record symbol in token array */
        ts = table_ident[(v & ~SYM_STRUCT) - TOK_IDENT];
        ps = &ts->sym_identifier;
        s->prev_tok = *ps;
        *ps = s;
        s->sym_scope = local_scope;
    }
    return s;
}

/* push a global identifier */
ST_FUNC Sym *global_identifier_push(int v, int t, int c)
{
    Sym *s, **ps;
    s = sym_push2(&global_stack, v, t, c);
    /* don't record anonymous symbol */
    if (v < SYM_FIRST_ANOM) {
        ps = &table_ident[v - TOK_IDENT]->sym_identifier;
        /* modify the top most local identifier, so that
           sym_identifier will point to 's' when popped */
        while (*ps != NULL && (*ps)->sym_scope)
            ps = &(*ps)->prev_tok;
        s->prev_tok = *ps;
        *ps = s;
    }
    return s;
}

/* pop symbols until top reaches 'b'.  If KEEP is non-zero don't really
   pop them yet from the list, but do remove them from the token array.  */
ST_FUNC void sym_pop(Sym **ptop, Sym *b, int keep)
{
    Sym *s, *ss, **ps;
    TokenSym *ts;
    int v;

    s = *ptop;
    while(s != b) {
        ss = s->prev;
        v = s->v;
        /* remove symbol in token array */
        /* XXX: simplify */
        if (!(v & SYM_FIELD) && (v & ~SYM_STRUCT) < SYM_FIRST_ANOM) {
            ts = table_ident[(v & ~SYM_STRUCT) - TOK_IDENT];
            ps = &ts->sym_identifier;
            *ps = s->prev_tok;
        }
	if (!keep)
	    sym_free(s);
        s = ss;
    }
    if (!keep)
	*ptop = b;
}

/* ------------------------------------------------------------------------- */

static void vsetc(CType *type, int r, CValue *vc)
{
    int v;

    /* cannot let cpu flags if other instruction are generated. Also
       avoid leaving VT_JMP anywhere except on the top of the stack
       because it would complicate the code generator.

       Don't do this when nocode_wanted.  vtop might come from
       !nocode_wanted regions (see 88_codeopt.c) and transforming
       it to a register without actually generating code is wrong
       as their value might still be used for real.  All values
       we push under nocode_wanted will eventually be popped
       again, so that the VT_CMP/VT_JMP value will be in vtop
       when code is unsuppressed again.

       Same logic below in vswap(); */
    if (vtop >= vstack && !nocode_wanted) {
        v = vtop->r & VT_VALMASK;
        if (v == VT_CMP || (v & ~1) == VT_JMP)
            gv(RC_INT);
    }

    vtop++;
    vtop->type = *type;
    vtop->r = r;
    vtop->r2 = VT_CONST;
    vtop->c = *vc;
    vtop->sym = NULL;
}

ST_FUNC void vswap(void)
{
    SValue tmp;
    /* cannot vswap cpu flags. See comment at vsetc() above */
    if (vtop >= vstack && !nocode_wanted) {
        int v = vtop->r & VT_VALMASK;
        if (v == VT_CMP || (v & ~1) == VT_JMP)
            gv(RC_INT);
    }
    tmp = vtop[0];
    vtop[0] = vtop[-1];
    vtop[-1] = tmp;
}

/* pop stack value */
ST_FUNC void vpop(void)
{
    int v;
    v = vtop->r & VT_VALMASK;
    /* for x86, we need to pop the FP stack */
    if (v == VT_JMP || v == VT_JMPI) {
        /* need to put correct jump if && or || without test */
        gsym(vtop->c.i);
    }
    vtop--;
}

/* push integer constant */
ST_FUNC void vpushi(int v)
{
    CValue cval;
    cval.i = v;
    vsetc(&int_type, VT_CONST, &cval);
}

ST_FUNC void vset(CType *type, int r, int v)
{
    CValue cval;

    cval.i = v;
    vsetc(type, r, &cval);
}

/* push a symbol value of TYPE */
static inline void vpushsym(CType *type, Sym *sym)
{
    CValue cval;
    cval.i = 0;
    vsetc(type, VT_CONST | VT_SYM, &cval);
    vtop->sym = sym;
}

/* Return a static symbol pointing to a section */
ST_FUNC Sym *get_sym_ref(CType *type, Section *sec, unsigned long offset, unsigned long size)
{
    int v;
    Sym *sym;

    v = anon_sym++;
    sym = global_identifier_push(v, type->t | VT_STATIC, 0);
    sym->type.ref = type->ref;
    sym->r = VT_CONST | VT_SYM;
    put_extern_sym(sym, sec, offset, size);
    return sym;
}

/* push a reference to a section offset by adding a dummy symbol */
static void vpush_ref(CType *type, Section *sec, unsigned long offset, unsigned long size)
{
    vpushsym(type, get_sym_ref(type, sec, offset, size));  
}

/* define a new external reference to a symbol 'v' of type 'u' */
ST_FUNC Sym *external_global_sym(int v, CType *type, int r)
{
    Sym *s;

    s = sym_find(v);
    if (!s) {
        /* push forward reference */
        s = global_identifier_push(v, type->t | VT_EXTERN, 0);
        s->type.ref = type->ref;
        s->r = r | VT_CONST | VT_SYM;
    }
    return s;
}

/* Merge some type attributes.  */
static void patch_type(Sym *sym, CType *type)
{
    if (!(type->t & VT_EXTERN)) {
        sym->type.t &= ~VT_EXTERN;
    }

    if ((sym->type.t & VT_BTYPE) == VT_FUNC) {
        int static_proto = sym->type.t & VT_STATIC;

        if (0 == (type->t & VT_EXTERN)) {
            /* put complete type, use static from prototype */
            sym->type.t = (type->t & ~VT_STATIC) | static_proto;
            if (type->t & VT_INLINE)
                sym->type.t = type->t;
            sym->type.ref = type->ref;
        }
    }
}


/* Merge some storage attributes.  */
static void patch_storage(Sym *sym, AttributeDef *ad, CType *type)
{
    if (type)
        patch_type(sym, type);

    sym->a.weak |= ad->a.weak;
    update_storage(sym);
}

/* define a new external reference to a symbol 'v' */
static Sym *external_sym(int v, CType *type, int r, AttributeDef *ad)
{
    Sym *s;
    s = sym_find(v);
    if (!s) {
        /* push forward reference */
        s = sym_push(v, type, r | VT_CONST | VT_SYM, 0);
        s->type.t |= VT_EXTERN;
        s->a = ad->a;
        s->sym_scope = 0;
    } else {
        patch_storage(s, ad, type);
    }
    return s;
}

/* push a reference to global symbol v */
ST_FUNC void vpush_global_sym(CType *type, int v)
{
    vpushsym(type, external_global_sym(v, type, 0));
}

/* save registers up to (vtop - n) stack entry */
ST_FUNC void save_regs(int n)
{
    SValue *p, *p1;
    for(p = vstack, p1 = vtop - n; p <= p1; p++)
        save_reg(p->r);
}

/* save r to the memory stack, and mark it as being free */
ST_FUNC void save_reg(int r)
{
    save_reg_upstack(r, 0);
}

/* save r to the memory stack, and mark it as being free,
   if seen up to (vtop - n) stack entry */
ST_FUNC void save_reg_upstack(int r, int n)
{
    int l, saved, size, align;
    SValue *p, *p1, sv;
    CType *type;

    if ((r &= VT_VALMASK) >= VT_CONST)
        return;

    /* modify all stack values */
    saved = 0;
    l = 0;
    for(p = vstack, p1 = vtop - n; p <= p1; p++) {
        if ((p->r & VT_VALMASK) == r ||
            ((p->type.t & VT_BTYPE) == VT_LLONG && (p->r2 & VT_VALMASK) == r)) {
            /* must save value on stack if not already done */
            if (!saved) {
                /* NOTE: must reload 'r' because r might be equal to r2 */
                r = p->r & VT_VALMASK;
                /* store register in the stack */
                type = &p->type;
                if ((p->r & VT_LVAL) ||
                    (!is_float(type->t) && (type->t & VT_BTYPE) != VT_LLONG))
                    type = &int_type;
                size = type_size(type, &align);
                loc = (loc - size) & -align;
                sv.type.t = type->t;
                sv.r = VT_LOCAL | VT_LVAL;
                sv.c.i = loc;
                store(r, &sv);

                l = loc;
                saved = 1;
            }
            /* mark that stack entry as being saved on the stack */
            p->r = lvalue_type(p->type.t) | VT_LOCAL;
            p->r2 = VT_CONST;
            p->c.i = l;
        }
    }
}

/* find a free register of class 'rc'. If none, save one register */
ST_FUNC int get_reg(int rc)
{
    int r;
    SValue *p;

    /* find a free register */
    for(r=0;r<NB_REGS;r++) {
        if (reg_classes[r] & rc) {
            for(p=vstack;p<=vtop;p++) {
                if ((p->r & VT_VALMASK) == r ||
                    (p->r2 & VT_VALMASK) == r)
                    goto notfound;
            }
            return r;
        }
    notfound: ;
    }
}

/* store vtop a register belonging to class 'rc'. lvalues are
   converted to values. Cannot be used if cannot be converted to
   register value (such as structures). */
ST_FUNC int gv(int rc)
{
    int r, bit_pos, bit_size, size, align, rc2;

        r = vtop->r & VT_VALMASK;
        rc2 = (rc & RC_FLOAT) ? RC_FLOAT : RC_INT;
        if (rc == RC_IRET)
            rc2 = RC_LRET;
        /* need to reload if:
           - constant
           - lvalue (need to dereference pointer)
           - already a register, but not in the right class */
        if (r >= VT_CONST
         || (vtop->r & VT_LVAL)
         || !(reg_classes[r] & rc)
         || ((vtop->type.t & VT_BTYPE) == VT_LLONG && !(reg_classes[vtop->r2] & rc2))
            )
        {
            r = get_reg(rc);
            if ((vtop->r & VT_LVAL) && !is_float(vtop->type.t)) {
                int t1, t;
                /* lvalue of scalar type : need to use lvalue type
                   because of possible cast */
                t = vtop->type.t;
                t1 = t;
                vtop->type.t = t;
                load(r, vtop);
                /* restore wanted type */
                vtop->type.t = t1;
            } else {
                /* one register type load */
                load(r, vtop);
            }
        }
        vtop->r = r;
    return r;
}

/* wrapper around RC_FRET to return a register by type */
static int rc_fret(int t)
{
    return RC_FRET;
}

/* wrapper around REG_FRET to return a register by type */
static int reg_fret(int t)
{
    return REG_FRET;
}

/* Generate value test
 *
 * Generate a test for any value (jump, comparison and integers) */
ST_FUNC int gvtst(int inv, int t)
{
    int v = vtop->r & VT_VALMASK;
    if (v != VT_CMP && v != VT_JMP && v != VT_JMPI) {
        vpushi(0);
        gen_op(TOK_NE);
    }
    if ((vtop->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST) {
        /* constant jmp optimization */
        if ((vtop->c.i != 0) != inv)
            t = gjmp(t);
        vtop--;
        return t;
    }
    return gtst(inv, t);
}

/* handle integer constant optimizations and various machine
   independent opt */
static void gen_opic(int op)
{
    SValue *v1 = vtop - 1;
    SValue *v2 = vtop;
    int t1 = v1->type.t & VT_BTYPE;
    int t2 = v2->type.t & VT_BTYPE;
    int c1 = (v1->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST;
    int c2 = (v2->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST;
    uint64_t l1 = c1 ? v1->c.i : 0;
    uint64_t l2 = c2 ? v2->c.i : 0;
    int shm = (t1 == VT_LLONG) ? 63 : 31;

    if (t1 != VT_LLONG && (PTR_SIZE != 8 || t1 != VT_PTR))
        l1 = ((uint32_t)l1 |
              (v1->type.t & VT_UNSIGNED ? 0 : -(l1 & 0x80000000)));
    if (t2 != VT_LLONG && (PTR_SIZE != 8 || t2 != VT_PTR))
        l2 = ((uint32_t)l2 |
              (v2->type.t & VT_UNSIGNED ? 0 : -(l2 & 0x80000000)));

    if (c1 && c2) {
        switch(op) {
        case TOK_NE: l1 = l1 != l2; break;
        }
	if (t1 != VT_LLONG && (PTR_SIZE != 8 || t1 != VT_PTR))
	    l1 = ((uint32_t)l1 |
		(v1->type.t & VT_UNSIGNED ? 0 : -(l1 & 0x80000000)));
        v1->c.i = l1;
        vtop--;
    } else {
        gen_opi(op);
    }
}

/* generic gen_op: handles types problems */
ST_FUNC void gen_op(int op)
{
    int u, t1, t2, bt1, bt2, t;
    CType type1;

    t1 = vtop[-1].type.t;
    t2 = vtop[0].type.t;
    bt1 = t1 & VT_BTYPE;
    bt2 = t2 & VT_BTYPE;
        
    /* integer operations */
    t = VT_INT | (VT_LONG & (t1 | t2));
    vswap();
    type1.t = t;
    type1.ref = NULL;
    gen_cast(&type1);
    vswap();
    gen_cast(&type1);
    gen_opic(op);
    /* relational op: the result is an int */
    vtop->type.t = VT_INT;
}

static void gen_cast(CType *type)
{
    int sbt, dbt, sf, df, c, p;

    dbt = type->t & (VT_BTYPE | VT_UNSIGNED);
    sbt = vtop->type.t & (VT_BTYPE | VT_UNSIGNED);

    if (sbt != dbt) {
        sf = is_float(sbt);
        df = is_float(dbt);
        c = (vtop->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == VT_CONST;
        p = (vtop->r & (VT_VALMASK | VT_LVAL | VT_SYM)) == (VT_CONST | VT_SYM);
        if (c) {
            if (sbt & VT_UNSIGNED)
                vtop->c.i = (uint32_t)vtop->c.i;
            else if (sbt != VT_LLONG)
                vtop->c.i = ((uint32_t)vtop->c.i |
                              -(vtop->c.i & 0x80000000));

                uint32_t m = ((dbt & VT_BTYPE) == VT_BYTE ? 0xff :
                              (dbt & VT_BTYPE) == VT_SHORT ? 0xffff :
                              0xffffffff);
                vtop->c.i &= m;
                if (!(dbt & VT_UNSIGNED))
                    vtop->c.i |= -(vtop->c.i & ((m >> 1) + 1));
            }
    }
    vtop->type = *type;
}

/* return type size as known at compile time. Put alignment at 'a' */
ST_FUNC int type_size(CType *type, int *a)
{
    Sym *s;
    int bt;

    bt = type->t & VT_BTYPE;
    if (bt == VT_PTR) {
        int ts;

        s = type->ref;
        ts = type_size(&s->type, a);

        return ts * s->c;
    } else if (bt == VT_INT || bt == VT_FLOAT) {
        *a = 4;
        return 4;
    } else {
        /* char, void, function, _Bool */
        *a = 1;
        return 1;
    }
}

/* return the pointed type of t */
static inline CType *pointed_type(CType *type)
{
    return &type->ref->type;
}

/* modify type so that its it is a pointer to type. */
ST_FUNC void mk_pointer(CType *type)
{
    Sym *s;
    s = sym_push(SYM_FIELD, type, 0, -1);
    type->t = VT_PTR | (type->t & VT_STORAGE);
    type->ref = s;
}

/* verify type compatibility to store vtop in 'dt' type, and generate
   casts if needed. */
static void gen_assign_cast(CType *dt)
{
    gen_cast(dt);
}

/* store vtop in lvalue pushed on stack */
ST_FUNC void vstore(void)
{
    int sbt, dbt, ft, r, t, size, align, bit_size, bit_pos, rc, delayed_cast;

    ft = vtop[-1].type.t;
    sbt = vtop->type.t & VT_BTYPE;
    dbt = ft & VT_BTYPE;
    delayed_cast = 0;
    if (!(ft & VT_BITFIELD))
        gen_assign_cast(&vtop[-1].type);

    rc = RC_INT;
    r = gv(rc);  /* generate value */
    store(r, vtop - 1);

    vswap();
    vtop--; /* NOT vpop() because on x86 it would flush the fp stack */
    vtop->r |= delayed_cast;
}

/* return 0 if no type declaration. otherwise, return the basic type
   and skip it. 
 */
static int parse_btype(CType *type, AttributeDef *ad)
{
    int t, u, bt, st, type_found, typespec_found, g;
    Sym *s;
    CType type1;

    memset(ad, 0, sizeof(AttributeDef));
    type_found = 0;
    typespec_found = 0;
    t = VT_INT;
    bt = st = -1;
    type->ref = NULL;

    while(1) {
        switch(tok) {
            /* basic types */
        basic_type:
            next();
        basic_type1:
            bt = u;
            if (u != VT_INT)
                t = (t & ~(VT_BTYPE|VT_LONG)) | u;
            typespec_found = 1;
            break;
        case TOK_VOID:
            u = VT_VOID;
            goto basic_type;
        case TOK_INT:
            u = VT_INT;
            goto basic_type;
        default:
            if (typespec_found)
                goto the_end;
            s = sym_find(tok);
            goto the_end;
        }
        type_found = 1;
    }
the_end:
    /* VT_LONG is used just as a modifier for VT_INT / VT_LLONG */
    bt = t & (VT_BTYPE|VT_LONG);
    type->t = t;
    return type_found;
}

/* convert a function parameter type (array to pointer and function to
   function pointer) */
static inline void convert_parameter_type(CType *pt)
{
    /* remove const and volatile qualifiers (XXX: const could be used
       to indicate a const function parameter */
    pt->t &= ~(VT_CONSTANT | VT_VOLATILE);
    /* array must be transformed to pointer according to ANSI C */
    pt->t &= ~VT_ARRAY;
}

static int post_type(CType *type, AttributeDef *ad, int storage, int td)
{
    int n, l, t1, arg_size, align;
    Sym **plast, *s, *first;
    AttributeDef ad1;
    CType pt;

    if (tok == '(') {
        /* function type, or recursive declarator (return if so) */
        next();
	if (tok == ')') {
	  l = 0;
	} else {
            parse_btype(&pt, &ad1);
            l = FUNC_NEW;
        }
        first = NULL;
        plast = &first;
        arg_size = 0;
        if (l) {
            for(;;) {
                /* read param name and compute offset */
                type_decl(&pt, &ad1, &n, TYPE_DIRECT | TYPE_ABSTRACT);
                arg_size += (type_size(&pt, &align) + PTR_SIZE - 1) / PTR_SIZE;
                convert_parameter_type(&pt);
                s = sym_push(n | SYM_FIELD, &pt, 0, 0);
                *plast = s;
                plast = &s->next;
                if (tok == ')')
                    break;
                skip(',');
                parse_btype(&pt, &ad1);
            }
        } else
            /* if no parameters, then old type prototype */
            l = FUNC_OLD;
        skip(')');
        /* NOTE: const is ignored in returned type as it has a special
           meaning in gcc / C++ */
        type->t &= ~VT_CONSTANT; 
        /* we push a anonymous symbol which will contain the function prototype */
        ad->f.func_args = arg_size;
        ad->f.func_type = l;
        s = sym_push(SYM_FIELD, type, 0, 0);
        s->a = ad->a;
        s->f = ad->f;
        s->next = first;
        type->t = VT_FUNC;
        type->ref = s;
    }
    return 1;
}

/* Parse a type declarator (except basic type), and return the type
   in 'type'. 'td' is a bitmask indicating which kind of type decl is
   expected. 'type' should contain the basic type. 'ad' is the
   attribute definition of the basic type. It can be modified by
   type_decl().  If this (possibly abstract) declarator is a pointer chain
   it returns the innermost pointed to type (equals *type, but is a different
   pointer), otherwise returns type itself, that's used for recursive calls.  */
static CType *type_decl(CType *type, AttributeDef *ad, int *v, int td)
{
    CType *post, *ret;
    int qualifiers, storage;

    /* recursive type, remove storage bits first, apply them later again */
    storage = type->t & VT_STORAGE;
    type->t &= ~VT_STORAGE;
    post = ret = type;

    /* type identifier */
    *v = tok;
    next();
    post_type(post, ad, storage, 0);
    type->t |= storage;
    return ret;
}

/* compute the lvalue VT_LVAL_xxx needed to match type t. */
ST_FUNC int lvalue_type(int t)
{
    return VT_LVAL;
}

/* pass a parameter to a function and do type checking and casting */
static void gfunc_param_typed(Sym *func, Sym *arg)
{
    int func_type;
    CType type;

    func_type = func->f.func_type;
    if (func_type != FUNC_OLD) {
        type = arg->type;
        type.t &= ~VT_CONSTANT; /* need to do that to avoid false warning */
        gen_assign_cast(&type);
    }
}

ST_FUNC void unary(void)
{
    int n, t, align, size, r, sizeof_caller;
    CType type;
    Sym *s;
    AttributeDef ad;

    sizeof_caller = in_sizeof;
    in_sizeof = 0;
    type.ref = NULL;
    switch(tok) {
    case TOK_LCHAR:
    case TOK_CINT:
    case TOK_CCHAR: 
	t = VT_INT;
 push_tokc:
	type.t = t;
	vsetc(&type, VT_CONST, &tokc);
        next();
        break;
    case TOK_CUINT:
        t = VT_INT | VT_UNSIGNED;
        goto push_tokc;
    case TOK_STR:
        /* string parsing */
        t = VT_BYTE;
    str_init:
        type.t = t;
        mk_pointer(&type);
        type.t |= VT_ARRAY;
        memset(&ad, 0, sizeof(AttributeDef));
        decl_initializer_alloc(&type, &ad, VT_CONST, 2, 0, 0);
        break;
    case '(':
        next();
        parse_btype(&type, &ad);
        gexpr();
        skip(')');
        break;
    default:
    tok_identifier:
        t = tok;
        next();
        s = sym_find(t);
        if (!s || IS_ASM_SYM(s)) {
            const char *name = get_tok_str(t, NULL);
            s = external_global_sym(t, &func_old_type, 0); 
        }

        r = s->r;
        vset(&s->type, r, s->c);
        /* Point to s as backpointer (even without r&VT_SYM).
	   Will be used by at least the x86 inline asm parser for
	   regvars.  */
	vtop->sym = s;

        if (r & VT_SYM) {
            vtop->c.i = 0;
        }
        break;
    }
    
    /* post operations */
    while (1) {
        if (tok == '(') {
            SValue ret;
            Sym *sa;
            int nb_args, ret_nregs, ret_align, regsize, variadic;

            vtop->r &= ~VT_LVAL; /* no lvalue */
            /* get return type */
            s = vtop->type.ref;
            next();
            sa = s->next; /* first parameter */
            nb_args = regsize = 0;
            ret.r2 = VT_CONST;
            ret_nregs = 1;
            ret.type = s->type;

            ret.r = REG_IRET;
            ret.c.i = 0;
            if (tok != ')') {
                for(;;) {
                    expr_eq();
                    gfunc_param_typed(s, sa);
                    nb_args++;
                    if (sa)
                        sa = sa->next;
                    if (tok == ')')
                        break;
                    skip(',');
                }
            }
            skip(')');
            gfunc_call(nb_args);

            /* return value */
            for (r = ret.r + ret_nregs + !ret_nregs; r-- > ret.r;) {
                vsetc(&ret.type, r, &ret.c);
                vtop->r2 = ret.r2; /* Loop only happens when r2 is VT_CONST */
            }
        } else {
            break;
        }
    }
}

static void expr_eq(void)
{
    int t;
    
    unary();
    if (tok == '=' ||
        (tok >= TOK_A_MOD && tok <= TOK_A_DIV) ||
        tok == TOK_A_XOR || tok == TOK_A_OR ||
        tok == TOK_A_SHL || tok == TOK_A_SAR) {
        t = tok;
        next();
        if (t == '=') {
            expr_eq();
        }
        vstore();
    }
}

ST_FUNC void gexpr(void)
{
    expr_eq();
}

static void gfunc_return(CType *func_type)
{
    gv(RC_IRET);
    vtop--; /* NOT vpop() because on x86 it would flush the fp stack */
}

static void block(int *bsym, int *csym, int is_expr)
{
    int a, b, c, d;
    Sym *s;

    if (tok == TOK_IF) {
        /* if test */
	int saved_nocode_wanted = nocode_wanted;
        next();
        skip('(');
        gexpr();
        skip(')');
        a = gvtst(1, 0);
        block(bsym, csym, 0);
        nocode_wanted = saved_nocode_wanted;
        c = tok;
        if (c == TOK_ELSE) {
            next();
            d = gjmp(0);
            gsym(a);
            block(bsym, csym, 0);
            gsym(d); /* patch else jmp */
            nocode_wanted = saved_nocode_wanted;
        } else
            gsym(a);
    } else if (tok == TOK_WHILE) {
	int saved_nocode_wanted;
	nocode_wanted &= ~0x20000000;
        next();
        d = ind;
        skip('(');
        gexpr();
        skip(')');
        a = gvtst(1, 0);
        b = 0;
        ++local_scope;
	saved_nocode_wanted = nocode_wanted;
        block(&a, &b, 0);
	nocode_wanted = saved_nocode_wanted;
        --local_scope;
        gjmp_addr(d);
        gsym(a);
        gsym_addr(b, d);
    } else if (tok == '{') {
        Sym *llabel;
        int block_vla_sp_loc = vla_sp_loc, saved_vlas_in_scope = vlas_in_scope;

        next();
        /* record local declaration stack position */
        s = local_stack;
        llabel = local_label_stack;
        ++local_scope;
        
        while (tok != '}') {
	    decl(VT_LOCAL);
            if (tok != '}') {
                block(bsym, csym, is_expr);
            }
        }
        /* pop locally defined labels */
        label_pop(&local_label_stack, llabel, is_expr);
        /* pop locally defined symbols */
        --local_scope;
	/* In the is_expr case (a statement expression is finished here),
	   vtop might refer to symbols on the local_stack.  Either via the
	   type or via vtop->sym.  We can't pop those nor any that in turn
	   might be referred to.  To make it easier we don't roll back
	   any symbols in that case; some upper level call to block() will
	   do that.  We do have to remove such symbols from the lookup
	   tables, though.  sym_pop will do that.  */
	sym_pop(&local_stack, s, is_expr);

        vlas_in_scope = saved_vlas_in_scope;
        
        next();
    } else if (tok == TOK_RETURN) {
        next();
        if (tok != ';') {
            gexpr();
            gen_assign_cast(&func_vt);
            gfunc_return(&func_vt);
        }
        skip(';');
        /* jump unless last stmt in top-level block */
        if (tok != '}' || local_scope != 1)
            rsym = gjmp(rsym);
	nocode_wanted |= 0x20000000;
    } else if (tok == TOK_BREAK) {
        /* compute jump */
        *bsym = gjmp(*bsym);
        next();
        skip(';');
	nocode_wanted |= 0x20000000;
    } else {
        if (tok != ';') {
                gexpr();
                vpop();
        }
        skip(';');
    }
}

/* store a value or an expression directly in global data or in local array */
static void init_putv(CType *type, Section *sec, unsigned long c)
{
    int bt;
    void *ptr;
    CType dtype;

    dtype = *type;
    dtype.t &= ~VT_CONSTANT; /* need to do that to avoid false warning */

    int size, align;
    /* XXX: not portable */
    /* XXX: generate error if incorrect relocation */
       gen_assign_cast(&dtype);
    bt = type->t & VT_BTYPE;

    size = type_size(type, &align);
    section_reserve(sec, c + size);
    ptr = sec->data + c;

    *(char *)ptr |= vtop->c.i;
    vtop--;
}

/* 't' contains the type and storage info. 'c' is the offset of the
   object in section 'sec'. If 'sec' is NULL, it means stack based
   allocation. 'first' is true if array '{' must be read (multi
   dimension implicit array init handling). 'size_only' is true if
   size only evaluation is wanted (only for arrays). */
static void decl_initializer(CType *type, Section *sec, unsigned long c, 
                             int first, int size_only)
{
    int len, n, no_oblock, nb, i;
    int size1, align1;
    int have_elem;
    Sym *s, *f;
    Sym indexsym;
    CType *t1;

    s = type->ref;
    n = s->c;
    t1 = pointed_type(type);
    size1 = type_size(t1, &align1);

    no_oblock = 1;

    /* only parse strings here if correct type (otherwise: handle
       them as ((w)char *) expressions */
    if ((tok == TOK_LSTR && 
         (t1->t & VT_BTYPE) == VT_INT
        ) || (tok == TOK_STR && (t1->t & VT_BTYPE) == VT_BYTE)) {
        len = 0;
        while (tok == TOK_STR || tok == TOK_LSTR) {
            int cstr_len, ch;

            cstr_len = tokc.str.size;
            cstr_len--;
            nb = cstr_len;
            if (!size_only) {
                memcpy(sec->data + c + len, tokc.str.data, nb);
            }
            len += nb;
            next();
        }
        /* only add trailing zero if enough storage (no
           warning in this case since it is standard) */
        if (n < 0 || len < n) {
            if (!size_only) {
                vpushi(0);
                init_putv(t1, sec, c + (len * size1));
            }
            len++;
        }
        len *= size1;
    }

    /* patch type size if needed, which happens only for array types */
    if (n < 0)
        s->c = size1 == 1 ? len : ((len + size1 - 1)/size1);
}

/* parse an initializer for type 't' if 'has_init' is non zero, and
   allocate space in local or global data space ('r' is either
   VT_LOCAL or VT_CONST). If 'v' is non zero, then an associated
   variable 'v' of scope 'scope' is declared before initializers
   are parsed. If 'v' is zero, then a reference to the new object
   is put in the value stack. If 'has_init' is 2, a special parsing
   is done to handle string constants. */
static void decl_initializer_alloc(CType *type, AttributeDef *ad, int r, 
                                   int has_init, int v, int scope)
{
    int size, align, addr;
    TokenString *init_str = NULL;

    Section *sec;
    Sym *flexible_array;
    Sym *sym = NULL;
    int saved_nocode_wanted = nocode_wanted;

    if (type->t & VT_STATIC)
        nocode_wanted |= NODATA_WANTED ? 0x40000000 : 0x80000000;

    flexible_array = NULL;
    if ((type->t & VT_BTYPE) == VT_STRUCT) {
        Sym *field = type->ref->next;
        if (field) {
            while (field->next)
                field = field->next;
            if (field->type.t & VT_ARRAY && field->type.ref->c < 0)
                flexible_array = field;
        }
    }

    size = type_size(type, &align);
    /* If unknown size, we must evaluate it before
       evaluating initializers because
       initializers can generate global data too
       (e.g. string pointers or ISOC99 compound
       literals). It also simplifies local
       initializers handling */
    if (size < 0 || (flexible_array && has_init)) {
        /* get all init string */
        init_str = tok_str_alloc();
        /* only get strings */
        while (tok == TOK_STR || tok == TOK_LSTR) {
            tok_str_add_tok(init_str);
            next();
        }
        tok_str_add(init_str, -1);
        tok_str_add(init_str, 0);
        unget_tok(0);

        /* compute size */
        begin_macro(init_str, 1);
        next();
        decl_initializer(type, NULL, 0, 1, 1);
        /* prepare second initializer parsing */
        macro_ptr = init_str->str;
        next();
        
        /* if still unknown size, error */
        size = type_size(type, &align);
        if (size < 0) 
            tcc_error("unknown type size");
    }
    /* take into account specified alignment if bigger */
    if (ad->a.aligned) {
	int speca = 1 << (ad->a.aligned - 1);
        if (speca > align)
            align = speca;
    } else if (ad->a.packed) {
        align = 1;
    }

    if (NODATA_WANTED)
        size = 0, align = 1;

    if ((r & VT_VALMASK) == VT_LOCAL) {
        sec = NULL;
        loc = (loc - size) & -align;
        addr = loc;
        if (v) {
            sym = sym_push(v, type, r, addr);
            sym->a = ad->a;
        } else {
            /* push local reference */
            vset(type, r, addr);
        }
    } else {

        /* allocate symbol in corresponding section */
        sec = ad->section;
        if (!sec) {
            if (has_init)
                sec = data_section;
            else if (tcc_state->nocommon)
                sec = bss_section;
        }

        if (sec) {
	    addr = section_add(sec, size, align);
        } else {
            addr = align; /* SHN_COMMON is special, symbol value is align */
	    sec = common_section;
        }

        if (v) {
            if (!sym) {
                sym = sym_push(v, type, r | VT_SYM, 0);
                patch_storage(sym, ad, NULL);
            }
            /* Local statics have a scope until now (for
               warnings), remove it here.  */
            sym->sym_scope = 0;
            /* update symbol definition */
	    put_extern_sym(sym, sec, addr, size);
        } else {
            /* push global reference */
            sym = get_sym_ref(type, sec, addr, size);
	    vpushsym(type, sym);
	    vtop->r |= r;
        }

    }

    if (type->t & VT_VLA) {
        int a;

        vla_sp_loc = addr;
        vlas_in_scope++;

    } else if (has_init) {
	size_t oldreloc_offset = 0;
	if (sec && sec->reloc)
	  oldreloc_offset = sec->reloc->data_offset;
        decl_initializer(type, sec, addr, 1, 0);
	if (sec && sec->reloc)
	  squeeze_multi_relocs(sec, oldreloc_offset);
        /* patch flexible array member size back to -1, */
        /* for possible subsequent similar declarations */
        if (flexible_array)
            flexible_array->type.ref->c = -1;
    }

 no_alloc:
    /* restore parse state if needed */
    if (init_str) {
        end_macro();
        next();
    }

    nocode_wanted = saved_nocode_wanted;
}

/* parse a function defined by symbol 'sym' and generate its code in
   'cur_text_section' */
static void gen_function(Sym *sym)
{
    nocode_wanted = 0;
    ind = cur_text_section->data_offset;
    /* NOTE: we patch the symbol size later */
    put_extern_sym(sym, cur_text_section, ind, 0);
    funcname = get_tok_str(sym->v, NULL);
    func_ind = ind;
    /* Initialize VLA state */
    vla_sp_loc = -1;
    vla_sp_root_loc = -1;
    /* push a dummy symbol to enable local sym storage */
    sym_push2(&local_stack, SYM_FIELD, 0, 0);
    local_scope = 1; /* for function parameters */
    gfunc_prolog(&sym->type);
    local_scope = 0;
    rsym = 0;
    block(NULL, NULL, 0);
    nocode_wanted = 0;
    gsym(rsym);
    gfunc_epilog();
    cur_text_section->data_offset = ind;
    label_pop(&global_label_stack, NULL, 0);
    /* reset local stack */
    local_scope = 0;
    sym_pop(&local_stack, NULL, 0);
    /* end of function */
    /* patch symbol size */
    elfsym(sym)->st_size = ind - func_ind;
    /* It's better to crash than to generate wrong code */
    cur_text_section = NULL;
    funcname = ""; /* for safety */
    func_vt.t = VT_VOID; /* for safety */
    func_var = 0; /* for safety */
    ind = 0; /* for safety */
    nocode_wanted = 0x80000000;
}

/* 'l' is VT_LOCAL or VT_CONST to define default storage type, or VT_CMP
   if parsing old style parameter decl list (and FUNC_SYM is set then) */
static int decl0(int l, int is_for_loop_init, Sym *func_sym)
{
    int v, has_init, r;
    CType type, btype;
    Sym *sym;
    AttributeDef ad;

    while (1) {
        if (!parse_btype(&btype, &ad)) {
                break;
        }
        while (1) { /* iterate thru each declaration */
            type = btype;
            type_decl(&type, &ad, &v, TYPE_DIRECT);
            if ((type.t & VT_BTYPE) == VT_FUNC) {
                /* if old style function prototype, we accept a
                   declaration list */
                sym = type.ref;
                if (sym->f.func_type == FUNC_OLD && l == VT_CONST)
                    decl0(VT_CMP, 0, sym);
            }

            if (tok == '{') {
                /* put function symbol */
                sym = external_global_sym(v, &type, 0);
                type.t &= ~VT_EXTERN;
                patch_storage(sym, &ad, &type);

                /* compute text section */
                cur_text_section = ad.section;
                if (!cur_text_section)
                    cur_text_section = text_section;
                gen_function(sym);
                break;
            } else {
                    r = 0;
                    if ((type.t & VT_BTYPE) == VT_FUNC) {
                        /* external function definition */
                        /* specific case for func_call attribute */
                        type.ref->f = ad.f;
                    } else if (!(type.t & VT_ARRAY)) {
                        /* not lvalue if array */
                        r |= lvalue_type(type.t);
                    }
                    has_init = (tok == '=');
                    if (((type.t & VT_EXTERN) && (!has_init || l != VT_CONST)) ||
			((type.t & VT_BTYPE) == VT_FUNC) ||
                        ((type.t & VT_ARRAY) && (type.t & VT_STATIC) &&
                         !has_init && l == VT_CONST && type.ref->c < 0)) {
                        /* external variable or function */
                        /* NOTE: as GCC, uninitialized global static
                           arrays of null size are considered as
                           extern */
                        type.t |= VT_EXTERN;
                        sym = external_sym(v, &type, r, &ad);
                    } else {
                        r |= l;
                        if (l == VT_CONST)
                            /* uninitialized global variables may be overridden */
                            type.t |= VT_EXTERN;
                        decl_initializer_alloc(&type, &ad, r, has_init, v, l);
                    }
                if (tok != ',') {
                    skip(';');
                    break;
                }
            }
        }
    }
    return 0;
}

static void decl(int l)
{
    decl0(l, 0, NULL);
}

/* ------------------------------------------------------------------------- */
