
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

  VT_VALMASK = 63;
  VT_CONST = 48;
  VT_LOCAL = 50;
  VT_SYM = 512;

  R_386_32 = 1;

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

/* 8 */
/* output constant with relocation if 'r & VT_SYM' is true */
int gen_addr32(int r, int sym, int c) {
    if (and(r, VT_SYM)) {
        greloc(cur_text_section, sym, ind, R_386_32);
    }
    gen_le32(c);
}

/* 9 */
/* generate a modrm reference. 'op_reg' contains the additional 3
   opcode bits */
int gen_modrm(int op_reg, int r, int sym, int c) {
    op_reg = shl(op_reg, 3);
    if (eq(and(r, VT_VALMASK), VT_CONST)) {
        /* constant memory reference */
        o(or(5, op_reg));
        gen_addr32(r, sym, c);
    } else if (eq(and(r, VT_VALMASK), VT_LOCAL)) {
        /* currently, we use only ebp as base */
        if (eq(c, movsx_eax_al(c))) {
            /* short reference */
            o(or(69, op_reg)); /* 0x45 */
            g(c);
        } else {
            oad(or(133, op_reg), c); /* 0x85 */
        }
    } else {
        g(or(op_reg, and(r, VT_VALMASK)));
    }
}

/* 12 */
int gadd_sp(int val) {
    if (eq(val, movsx_eax_al(val))) {
        o(50307); /* 0xc483 */
        g(val);
    } else {
        oad(50305, val); /* 0xc481 add $xxx, %esp */
    }
}

/* 16 */
/* generate function epilog */
int gfunc_epilog() {
    int v;
    int saved_ind;

    /* align local size to word & save local variables */
    v = and(add(sub(0,loc), 3), sub(0,4));

    o(201); /* 0xc9 leave */
    o(195); /* 0xc3 ret */
    saved_ind = ind;
    ind = sub(func_sub_sp_offset, FUNC_PROLOG_SIZE);
    o(15042901);  /* 0xe58955 push %ebp, mov %esp, %ebp */
    o(60545);  /* 0xec81 sub esp, stacksize */
    gen_le32(v);
    ind = saved_ind;
}

/* 17 */
/* generate a jump to a label */
int gjmp(int t) {
    return gjmp2(233, t); /* 0xe9 */
}

/* 18 */
/* generate a jump to a fixed address */
int gjmp_addr(int a) {
    int r;
    r = sub(sub(a, ind), 2);
    if (eq(r, movsx_eax_al(r))) {
        g(235); /* 0xeb */
        g(r);
    } else {
        oad(233, sub(sub(a, ind), 5)); /* 0xe9 */
    }
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
