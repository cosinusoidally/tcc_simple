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
extern int ind;

extern Sym *sym_free_first;
ST_DATA void **sym_pools;
ST_DATA int nb_sym_pools;

ST_DATA Sym *global_stack;
ST_DATA Sym *local_stack;
ST_DATA Sym *local_label_stack;
extern int local_scope;
extern int section_sym;

ST_DATA int global_expr;  /* true if compound literals must be allocated globally (used during initializers parsing */
ST_DATA CType func_vt; /* current function return type (used by return instruction) */
ST_DATA int func_vc;
ST_DATA int func_ind;

ST_DATA CType func_old_type;
ST_DATA CType int_type;

/* ------------------------------------------------------------------------- */

void gen_cast(CType *type);
static void gen_cast_s(int t);
static inline CType *pointed_type(CType *type);
static int is_compatible_types(CType *type1, CType *type2);
static CType *type_decl(CType *type, AttributeDef *ad, int *v);
static void init_putv(CType *type, Section *sec, unsigned long c);
int decl_initializer(CType *type, Section *sec, unsigned long c, int first, int size_only);
void block(int *bsym, int *csym, int is_expr);
int decl_initializer_alloc(CType *type, AttributeDef *ad, int r, int has_init, int v, int scope);
void decl(int l);
int decl0(int l, int is_for_loop_init, Sym *);
static void expr_eq(void);
static int is_compatible_unqualified_types(CType *type1, CType *type2);
static void vpush(CType *type);
static int gvtst(int inv, int t);
static void skip_or_save_block(TokenString **str);
static void gv_dup(void);

extern int aint_type;
extern int afunc_old_type;
extern int afunc_vt;
extern int asym_pools;
extern int anb_sym_pools;

int init_tccgen_globals(){
  aint_type = &int_type;
  afunc_old_type = &func_old_type;
  afunc_vt = &func_vt;
  asym_pools = &sym_pools;
  anb_sym_pools = &nb_sym_pools;
}

/* 47 */
int unary() {
    int n;
    int t;
    int align;
    int size;
    int r;
    int type;
    int s;
    int ad;
    int name;
    int ret;
    int sa;
    int nb_args;
    int ret_nregs;
    int regsize;

/* FIXME there is some bug with the virtual stack */
    enter();
    /* definitely something weird going on */
    /* 64 bytes of padding seems to fix */
    v_alloca(64);
    ret = v_alloca(sizeof_SValue);
    type = v_alloca(sizeof_CType);
    ad = v_alloca(mul(2, sizeof_AttributeDef));

    sct_ref(type, 0);
    if(or(eq(tok, TOK_CINT), eq(tok, TOK_CCHAR))) {
	t = VT_INT;
	sct_t(type, t);
	vsetc(type, VT_CONST, atokc);
        next();
    } else if(eq(tok, TOK_CUINT)) {
        t = or(VT_INT, VT_UNSIGNED);
	sct_t(type, t);
	vsetc(type, VT_CONST, atokc);
        next();
    } else if(eq(tok, TOK_STR)) {
        /* string parsing */
        t = VT_BYTE;
        sct_t(type, t);
        mk_pointer(type);
        sct_t(type, or(gct_t(type), VT_ARRAY));
        memset(ad, 0, sizeof_AttributeDef);
        decl_initializer_alloc(type, ad, VT_CONST, 2, 0, 0);
    } else if(eq(tok, mkc('('))) {
        next();
        parse_btype(type, ad);
        gexpr();
        skip(mkc(')'));
    } else {
        t = tok;
        next();
        s = sym_find(t);
        if (eq(0, s)) {
            name = get_tok_str(t, 0);
            s = external_global_sym(t, afunc_old_type, 0);
        }

        r = gsym_r(s);
        vset(gsym_type(s), r, gsym_c(s));
        /* Point to s as backpointer (even without r&VT_SYM).
	   Will be used by at least the x86 inline asm parser for
	   regvars.  */
	ssv_sym(vtop, s);

        if (and(r, VT_SYM)) {
            scv_i(gsv_c(vtop), 0);
        }
    }
    
    /* post operations */
    while (1) {
        if (eq(tok, mkc('('))) {

            ssv_r(vtop, and(gsv_r(vtop), not(VT_LVAL))); /* no lvalue */
            /* get return type */
            s = gct_ref(gsv_type(vtop));
            next();
            sa = gsym_next(s); /* first parameter */
            nb_args = 0;
            regsize = 0;
            ret_nregs = 1;
            memmove(gsv_type(ret), gsym_type(s), sizeof_CType);

            ssv_r(ret, REG_IRET);
            scv_i(gsv_c(ret), 0);
            if (neq(tok, mkc(')'))) {
                while(1) {
                    expr_eq();
                    gfunc_param_typed(s, sa);
                    nb_args = add(nb_args, 1);
                    if (sa) {
                        sa = gsym_next(sa);
                    }
                    if (eq(tok, mkc(')'))) {
                        break;
                    }
                    skip(mkc(','));
                }
            }
            skip(mkc(')'));
            gfunc_call(nb_args);

            /* return value */
            r = add(gsv_r(ret), ret_nregs);
            while(gt(r, gsv_r(ret))) {
                r = sub(r, 1);
                vsetc(gsv_type(ret), r, gsv_c(ret));
            }
        } else {
            break;
        }
    }

    return leave(0);
}

/* 51 */
void block(int *bsym, int *csym, int is_expr) {
    int a;
    int b;
    int c;
    int d;
    Sym *s;
    Sym *llabel;

    if (eq(tok, TOK_IF)) {
        /* if test */
        next();
        skip(mkc('('));
        gexpr();
        skip(mkc(')'));
        a = gvtst(1, 0);
        block(bsym, csym, 0);
        c = tok;
        if (eq(c, TOK_ELSE)) {
            next();
            d = gjmp(0);
            gsym(a);
            block(bsym, csym, 0);
            gsym(d); /* patch else jmp */
        } else {
            gsym(a);
        }
    } else if(eq(tok, TOK_WHILE)) {
        next();
        d = ind;
        skip(mkc('('));
        gexpr();
        skip(mkc(')'));
        a = gvtst(1, 0);
        b = 0;
        local_scope = local_scope + 1;
        block(&a, &b, 0);
        local_scope = local_scope - 1;
        gjmp_addr(d);
        gsym(a);
        gsym_addr(b, d);
    } else if (eq(tok, mkc('{'))) {

        next();
        /* record local declaration stack position */
        s = local_stack;
        llabel = local_label_stack;
        local_scope = local_scope + 1;
        
        while (neq(tok, mkc('}'))) {
	    decl(VT_LOCAL);
            if (neq(tok, mkc('}'))) {
                block(bsym, csym, is_expr);
            }
        }
        /* pop locally defined symbols */
        local_scope = local_scope - 1;
	/* In the is_expr case (a statement expression is finished here),
	   vtop might refer to symbols on the local_stack.  Either via the
	   type or via vtop->sym.  We can't pop those nor any that in turn
	   might be referred to.  To make it easier we don't roll back
	   any symbols in that case; some upper level call to block() will
	   do that.  We do have to remove such symbols from the lookup
	   tables, though.  sym_pop will do that.  */
	sym_pop(&local_stack, s, is_expr);

        next();
    } else if(eq(tok, TOK_RETURN)) {
        next();
        if (neq(tok, mkc(';'))) {
            gexpr();
            gen_assign_cast(&func_vt);
            gfunc_return(&func_vt);
        }
        skip(mkc(';'));
        /* jump unless last stmt in top-level block */
        if(or(neq(tok, mkc('}')), neq(local_scope, 1))) {
            rsym = gjmp(rsym);
        }
    } else if(eq(tok, TOK_BREAK)) {
        /* compute jump */
        *bsym = gjmp(*bsym);
        next();
        skip(';');
    } else {
        if (neq(tok, mkc(';'))) {
                gexpr();
                vpop();
        }
        skip(mkc(';'));
    }
}
