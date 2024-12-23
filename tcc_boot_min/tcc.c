#include "tcc.h"

int init_globals() {
  aglobal_stack = &global_stack;
  alocal_stack = &local_stack;

  aint_type = &int_type;
  afunc_old_type = &func_old_type;
  afunc_vt = &func_vt;
  asym_pools = &sym_pools;
  anb_sym_pools = &nb_sym_pools;

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
