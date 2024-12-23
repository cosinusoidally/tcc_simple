extern int aTOK_HASH_SIZE;
extern int CH_EOF_;
extern int aSTRING_MAX_SIZE;
extern int aglobal_stack;
extern int alocal_stack;
extern int aint_type;
extern int afunc_old_type;
extern int afunc_vt;
extern int asym_pools;
extern int anb_sym_pools;
extern int ahash_ident;
extern int acstr_buf;
extern int aisidnum_table;
extern int atokc;
extern int atokcstr;
extern int atoken_buf;
extern int avstack;

extern int sizeof_void;
extern int sizeof_int;
extern int sizeof_CType;
extern int sizeof_CString;
extern int sizeof_CValue;
extern int sizeof_void;
extern int sizeof_SValue;

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
  atokc = tcc_mallocz(sizeof_CValue);
  atokcstr = tcc_mallocz(sizeof_CString);

  /* char token_buf[STRING_MAX_SIZE + 1] */
  atoken_buf = tcc_mallocz(add(aSTRING_MAX_SIZE, 1));

  /* VSTACK_SIZE 256 */
  /* SValue vstack[VSTACK_SIZE] */
  avstack = tcc_mallocz(mul(256, sizeof_SValue));
}
