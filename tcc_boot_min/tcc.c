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

#include "tcc.h"

static TokenSym *hash_ident[TOK_HASH_SIZE];
static char token_buf[STRING_MAX_SIZE + 1];
static CString cstr_buf;
static unsigned char isidnum_table[256 - CH_EOF];

extern TokenString *macro_stack;

/* LJW HACK addreses of these variables */
/* actually one or two are not addreses */
extern int aTOK_HASH_SIZE;
extern int ahash_ident;
extern int acstr_buf;
extern int CH_EOF_;
extern int aisidnum_table;
extern int atokc;
extern int atokcstr;
extern int atoken_buf;
extern int avstack;

int init_tccpp_globals(){
  aTOK_HASH_SIZE = TOK_HASH_SIZE;
  ahash_ident = hash_ident;
  acstr_buf = &cstr_buf;
  CH_EOF_ = CH_EOF;
  aisidnum_table = isidnum_table;
  atokc = &tokc;
  atokcstr = &tokcstr;
  atoken_buf = &token_buf;
  avstack = vstack;
  aSTRING_MAX_SIZE = STRING_MAX_SIZE;
}

int init_globals() {
    aglobal_stack = &global_stack;
    alocal_stack = &local_stack;
}

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
int block(int *bsym, int *csym, int is_expr);
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
