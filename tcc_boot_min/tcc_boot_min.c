
/* TCCState accessors */

int gts_files(int o) {
  ri32(add(o, TCCState_files_o));
}

int sts_filetype(int o,int v) {
  wi32(add(o, TCCState_filetype_o), v);
}

int gts_outfile(int o) {
  ri32(add(o, TCCState_outfile_o));
}

/* struct filespec accessors */

int gfs_type(int o) {
  return ri8(add(o,filespec_type_o));
}

int gfs_name(int o) {
  return add(o,filespec_name_o);
}

int init_runtime(){
  foo=mks("hello world");
  puts(foo);
  printf(mks("blah: %d\n"),add(1,2));

  TCCState_files_o = 32;
  TCCState_filetype_o = 40;
  TCCState_outfile_o = 44;
  sizeof_TCCState = 56;

  filespec_type_o = 0;
  filespec_name_o = 1;
  sizeof_filespec = 2;

  sizeof_SValue = 24;

  FUNC_PROLOG_SIZE = 9;

  init_c();
}

/* i386-gen.c */

/* 2 */
int o(int c) {
    while (neq(0, c)) {
        g(c);
        c = shr(c, 8);
    }
}

/* 3 */
int gen_le32(int c) {
    g(c);
    g(shr(c, 8));
    g(shr(c, 16));
    g(shr(c, 24));
}

/* 5 */
int gsym(int t) {
    gsym_addr(t, ind);
}

/* 6 */
/* instruction + 4 bytes data. Return the address of the data */
int oad(int c, int s) {
    int t;
    o(c);
    t = ind;
    gen_le32(s);
    return t;
}

/* 7 */
/* generate jmp to a label */
int gjmp2(int instr,int lbl) {
    return oad(instr,lbl);
}

/* end of i386-gen.c */

int tcc_new() {
    int s;

    s = tcc_mallocz(sizeof_TCCState);
    tcc_state = s;

    tccelf_new(s);
    tccpp_new(s);

    return s;
}

int main(argc0, argv0){
    int s;
    int argc;
    int argv;
    int f;

    init_runtime();

    enter();

    argc = v_alloca(4);
    argv = v_alloca(4);
    wi32(argc, argc0);
    wi32(argv, argv0);

    s = tcc_new();

    tcc_parse_args(s, argc, argv, 1);

    f = ri32(gts_files(s));
    sts_filetype(s,gfs_type(f));
    tcc_add_file(s, gfs_name(f));
    sts_filetype(s, 0);

    tcc_output_file(s, gts_outfile(s));

    tcc_delete(s);
    return leave(0);
}
