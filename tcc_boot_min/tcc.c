#include "tcc.h"

int init_globals() {
  aTOK_HASH_SIZE = TOK_HASH_SIZE;

  CH_EOF_ = sub(0, 1);

  aSTRING_MAX_SIZE = STRING_MAX_SIZE;

  aglobal_stack = tcc_mallocz(sizeof_void); /* Sym *global_stack */
  alocal_stack = tcc_mallocz(sizeof_void); /* Sym *local_stack */

  aint_type = tcc_mallocz(sizeof_CType);
  afunc_old_type = tcc_mallocz(sizeof_CType);
  afunc_vt = tcc_mallocz(sizeof_CType);
  asym_pools = tcc_mallocz(sizeof_void); /* void **sym_pools */
  anb_sym_pools = tcc_mallocz(sizeof_int); /* int nb_sym_pools */

  /* TokenSym *hash_ident[TOK_HASH_SIZE] */
  ahash_ident = hash_ident;
  acstr_buf = &cstr_buf;
  aisidnum_table = isidnum_table;
  atokc = &tokc;
  atokcstr = &tokcstr;
  atoken_buf = &token_buf;
  avstack = vstack;
}
