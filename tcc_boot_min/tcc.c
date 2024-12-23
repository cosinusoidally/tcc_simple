#include "tcc.h"

int init_globals() {
  aTOK_HASH_SIZE = 16384;     /* must be a power of two */

  CH_EOF_ = sub(0, 1);

  aSTRING_MAX_SIZE = 1024;

  aglobal_stack = tcc_mallocz(sizeof_void); /* Sym *global_stack */
  alocal_stack = tcc_mallocz(sizeof_void); /* Sym *local_stack */

  aint_type = tcc_mallocz(sizeof_CType);
  afunc_old_type = tcc_mallocz(sizeof_CType);
  afunc_vt = tcc_mallocz(sizeof_CType);
  asym_pools = tcc_mallocz(sizeof_void); /* void **sym_pools */
  anb_sym_pools = tcc_mallocz(sizeof_int); /* int nb_sym_pools */

  /* TokenSym *hash_ident[TOK_HASH_SIZE] */
  ahash_ident = tcc_mallocz(mul(aTOK_HASH_SIZE, 4));
  acstr_buf = tcc_mallocz(sizeof_CString);

  /* unsigned char isidnum_table[256 - CH_EOF] */
  aisidnum_table = tcc_mallocz(sub(256, CH_EOF_));
  atokc = &tokc;
  atokcstr = &tokcstr;
  atoken_buf = &token_buf;
  avstack = vstack;
}
