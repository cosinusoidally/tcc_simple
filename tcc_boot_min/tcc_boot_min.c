
/* TCCState accessors */

int ats_sections(int o) {return add(o, TCCState_sections_o);}
int gts_sections(int o) {return ri32(add(o, TCCState_sections_o));}

int ats_nb_sections(int o) {return add(o, TCCState_nb_sections_o);}
int gts_nb_sections(int o) {return ri32(add(o, TCCState_nb_sections_o));}
int sts_nb_sections(int o,int v) {wi32(add(o, TCCState_nb_sections_o), v);}

int ats_priv_sections(int o) {return add(o, TCCState_priv_sections_o);}
int gts_priv_sections(int o) {return ri32(add(o, TCCState_priv_sections_o));}

int ats_nb_priv_sections(int o) {return add(o, TCCState_nb_priv_sections_o);}
int gts_nb_priv_sections(int o) {return ri32(add(o, TCCState_nb_priv_sections_o));}

int gts_symtab(int o) {return ri32(add(o, TCCState_symtab_o));}
int sts_symtab(int o,int v) {wi32(add(o, TCCState_symtab_o), v);}

int ats_files(int o) {return add(o, TCCState_files_o);}
int gts_files(int o) {return ri32(add(o, TCCState_files_o));}

int ats_nb_files(int o) {return add(o, TCCState_nb_files_o);}
int gts_nb_files(int o) {return ri32(add(o, TCCState_nb_files_o));}

int gts_filetype(int o) {return ri32(add(o, TCCState_filetype_o));}
int sts_filetype(int o,int v) {wi32(add(o, TCCState_filetype_o), v);}

int gts_outfile(int o) {ri32(add(o, TCCState_outfile_o));}
int sts_outfile(int o,int v) {wi32(add(o, TCCState_outfile_o), v);}

/* struct filespec accessors */

int gfs_type(int o) {return ri8(add(o,filespec_type_o));}
int sfs_type(int o, int v) {wi8(add(o,filespec_type_o, v));}

int gfs_name(int o) {return add(o,filespec_name_o);}

/* Section accessors */

int gs_data_offset(int o) {return ri32(add(o, Section_data_offset_o));}
int ss_data_offset(int o, int v) {wi32(add(o, Section_data_offset_o), v);}

int gs_data(int o) {return ri32(add(o, Section_data_o));}
int ss_data(int o, int v) {wi32(add(o, Section_data_o), v);}

int gs_data_allocated(int o) {return ri32(add(o, Section_data_allocated_o));}
int ss_data_allocated(int o, int v) {wi32(add(o, Section_data_allocated_o), v);}

int ss_sh_num(int o, int v) {wi32(add(o, Section_sh_num_o), v);}

int gs_sh_type(int o) {return ri32(add(o, Section_sh_type_o));}
int ss_sh_type(int o, int v) {wi32(add(o, Section_sh_type_o), v);}

int ss_sh_flags(int o, int v) {wi32(add(o, Section_sh_flags_o), v);}

int gs_sh_addralign(int o) {return ri32(add(o, Section_sh_addralign_o));}
int ss_sh_addralign(int o, int v) {wi32(add(o, Section_sh_addralign_o), v);}

int gs_sh_size(int o) {ri32(add(o, Section_sh_size_o));}

int ss_sh_offset(int o, int v) {wi32(add(o, Section_sh_offset_o), v);}

int ss_reloc(int o, int v) {wi32(add(o, Section_reloc_o), v);}

int gs_hash(int o) {return ri32(add(o, Section_hash_o));}
int ss_hash(int o, int v) {wi32(add(o, Section_hash_o), v);}

int gs_name(int o) {return add(o, Section_name_o);}

/* SValue accessors */

int gsv_r(int o) {return ri32(add(o, SValue_r_o));} /* int r */
int ssv_r(int o, int v) {wi32(add(o, SValue_r_o), v);}

int gsv_c(int o) { return add(o, SValue_c_o);} /* CValue c */

int gsv_sym(int o) { return ri32(add(o, SValue_sym_o));} /* struct Sym *sym */

/* CValue accessors */

int gcv_i(int o) {return ri32(add(o, CValue_i_o));}
int scv_i(int o, int v) {wi32(add(o, CValue_i_o), v);}

/* CType accessors */

int gct_t(int o) {return ri32(add(o, CType_t_o));}

int gct_ref(int o) {return ri32(add(o, CType_ref_o));}

/* Sym accessors */

int gsym_type(int o) {return add(o, Sym_type_o);} /* CType type */

int gsym_v(int o) {return ri32(add(o, Sym_v_o));} /* int v */

int gsym_next(int o) {return ri32(add(o, Sym_next_o));} /* struct Sym *next */

/* BufferedFile accessors */

int gbf_buf_ptr(int o) {return ri32(add(o, BufferedFile_buf_ptr_o));}
int sbf_buf_ptr(int o, int v) {wi32(add(o, BufferedFile_buf_ptr_o), v);}

int gbf_buf_end(int o) {return ri32(add(o, BufferedFile_buf_end_o));}
int sbf_buf_end(int o, int v) {wi32(add(o, BufferedFile_buf_end_o), v);}

int gbf_fd(int o) {return ri32(add(o, BufferedFile_fd_o));}
int sbf_fd(int o, int v) {wi32(add(o, BufferedFile_fd_o), v);}

int gbf_prev(int o) {return ri32(add(o, BufferedFile_prev_o));}
int sbf_prev(int o, int v) {wi32(add(o, BufferedFile_prev_o), v);}

int gbf_line_num(int o) {return ri32(add(o, BufferedFile_line_num_o));}
int sbf_line_num(int o, int v) {wi32(add(o, BufferedFile_line_num_o), v);}

/* char filename[1024] */
int gbf_filename(int o){return add(o, BufferedFile_filename_o);}

/* unsigned char buffer[1] */
int gbf_buffer(int o){ return add(o, BufferedFile_buffer_o);}

/* TCCOption accessors */

int gto_name(int o) {return ri32(add(o, TCCOption_name_o));}
int sto_name(int o, int v) {wi32(add(o, TCCOption_name_o), v);}

int gto_index(int o) {return ri32(add(o, TCCOption_index_o));}
int sto_index(int o, int v) {wi32(add(o, TCCOption_index_o), v);}

int gto_flags(int o) {return ri32(add(o, TCCOption_flags_o));}
int sto_flags(int o, int v) {wi32(add(o, TCCOption_flags_o), v);}

/* stuff */

int init_runtime(){
  foo=mks("hello world");
  puts(foo);
  printf(mks("blah: %d\n"),add(1,2));

  TCCState_sections_o = 12;
  TCCState_nb_sections_o = 16;
  TCCState_priv_sections_o = 20;
  TCCState_nb_priv_sections_o = 24;
  TCCState_symtab_o = 28;
  TCCState_files_o = 32;
  TCCState_nb_files_o = 36;
  TCCState_filetype_o = 40;
  TCCState_outfile_o = 44;
  sizeof_TCCState = 56;

  filespec_type_o = 0;
  filespec_name_o = 1;
  sizeof_filespec = 2;

  Section_data_offset_o = 0;
  Section_data_o = 4;
  Section_data_allocated_o = 8;
  Section_sh_num_o = 16;
  Section_sh_type_o = 20;
  Section_sh_flags_o = 24;
  Section_sh_addralign_o = 32;
  Section_sh_size_o = 40;
  Section_sh_offset_o = 48;
  Section_reloc_o = 60;
  Section_hash_o = 64;
  Section_name_o = 72;
  sizeof_Section = 76;

  SValue_type_o = 0;
  SValue_r_o = 8;
  SValue_c_o = 12;
  SValue_sym_o = 20;
  sizeof_SValue = 24;

  CValue_i_o = 0;
  sizeof_CValue = 8;

  CType_t_o = 0;
  CType_ref_o = 4;
  sizeof_CType = 8;

  Sym_v_o = 0;
  Sym_type_o = 20;
  Sym_next_o = 28;
  sizeof_Sym = 40;

  BufferedFile_buf_ptr_o = 0;
  BufferedFile_buf_end_o = 4;
  BufferedFile_fd_o = 8;
  BufferedFile_prev_o = 12;
  BufferedFile_line_num_o = 16;
  BufferedFile_filename_o = 20;
  BufferedFile_buffer_o = 1048;
  sizeof_BufferedFile = 1052;
  sizeof_BufferedFile_filename = 1024;

  TCCOption_name_o = 0;
  TCCOption_index_o = 4;
  TCCOption_flags_o = 8;
  sizeof_TCCOption = 12;

  sizeof_Elf32_Ehdr = 52;

  sizeof_Elf32_Phdr = 32;

  sizeof_void = 4;

  FUNC_PROLOG_SIZE = 9;

  VT_VALMASK = 63;
  VT_CONST = 48;
  VT_LOCAL = 50;
  VT_CMP = 51;
  VT_LVAL = 256;
  VT_SYM = 512;

  R_386_32 = 1;
  R_386_PC32 = 2;

  RC_INT = 1;

  RC_EAX = 4;
  RC_ECX = 16;
  RC_IRET = RC_EAX;

  SYM_FIELD = 536870912; /* 0x20000000 */

  SHF_WRITE = shl(1, 0); /* Writable */
  SHF_ALLOC = shl(1, 1);  /* Occupies memory during execution */
  SHF_EXECINSTR = shl(1, 2); /* Executable */
  SHF_PRIVATE = shl(1, 31);

  SHN_COMMON = 65522;  /* 0xfff2 Associated symbol is common */

  SHT_PROGBITS = 1; /* Program data */
  SHT_SYMTAB = 2;  /* Symbol table */
  SHT_STRTAB = 3; /* String table */
  SHT_RELA = 4; /* Relocation entries with addends */
  SHT_HASH = 5;  /* Symbol hash table */
  SHT_NOBITS = 8;  /* Program space with no data (bss) */
  SHT_REL = 9; /* Relocation entries, no addends */

  O_RDONLY = 0;
  O_BINARY = 0;

  IO_BUF_SIZE = 8192;

  CH_EOB = mkc('\\');

  TOK_FLAG_BOL = 1;
  TOK_FLAG_BOF = 2;

  reg_classes=calloc(1,mul(4, 5)); /* NB_REGS is 5 (sort of) */

  init_c();
  init_reg_classes();
  init_globals();

  init_options();
}

/* i386-gen.c */

int init_reg_classes() {
  wi32(reg_classes, or(RC_INT, RC_EAX));
  wi32(add(reg_classes, 4), or(RC_INT, RC_ECX));
}

/* XXX: make it faster ? */
int g(int c) {
    int ind1;
    ind1 = add(ind, 1);
    if (gt(ind1, gs_data_allocated(cur_text_section))) {
        section_realloc(cur_text_section, ind1);
    }
    wi8(add(gs_data(cur_text_section), ind), c);
    ind = ind1;
}

int o(int c) {
    while (neq(0, c)) {
        g(c);
        c = shr(c, 8);
    }
}

int gen_le32(int c) {
    g(c);
    g(shr(c, 8));
    g(shr(c, 16));
    g(shr(c, 24));
}

/* output a symbol and patch all calls to it */
int gsym_addr(int t, int a) {
    int ptr;
    int n;

    while (t) {
        ptr = add(gs_data(cur_text_section), t);
        n = read32le(ptr); /* next value */
        write32le(ptr, sub(sub(a, t), 4));
        t = n;
    }
}

int gsym(int t) {
    gsym_addr(t, ind);
}

/* instruction + 4 bytes data. Return the address of the data */
int oad(int c, int s) {
    int t;
    o(c);
    t = ind;
    gen_le32(s);
    return t;
}

/* generate jmp to a label */
int gjmp2(int instr,int lbl) {
    return oad(instr,lbl);
}

/* output constant with relocation if 'r & VT_SYM' is true */
int gen_addr32(int r, int sym, int c) {
    if (and(r, VT_SYM)) {
        greloc(cur_text_section, sym, ind, R_386_32);
    }
    gen_le32(c);
}

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

/* load 'r' from value 'sv' */
int load(int r, int sv) {
    int v;
    int fc;
    int fr;

    fr = gsv_r(sv);
    fc = gcv_i(gsv_c(sv));

    if (and(fr, VT_LVAL)) {
        o(139);     /* 0x8b movl */
        gen_modrm(r, fr, gsv_sym(sv), fc);
    } else {
            o(add(184, r)); /* 0xb8 mov $xx, r */
            gen_addr32(fr, gsv_sym(sv), fc);
    }
}

/* store register 'r' in lvalue 'v' */
int store(int r, int v) {
    o(137); /* 0x89 */
    gen_modrm(r, gsv_r(v), gsv_sym(v), gcv_i(gsv_c(v)));
}

int gadd_sp(int val) {
    if (eq(val, movsx_eax_al(val))) {
        o(50307); /* 0xc483 */
        g(val);
    } else {
        oad(50305, val); /* 0xc481 add $xxx, %esp */
    }
}

/* 'is_jmp' is '1' if it is a jump */
int gcall_or_jmp(int is_jmp) {
    int r;

    /* constant and relocation case */
    greloc(cur_text_section, gsv_sym(vtop), add(ind, 1), R_386_PC32);
    oad(add(232, is_jmp), sub(gcv_i(gsv_c(vtop)), 4)); /* 0xe8 call/jmp im */
}

/* Generate function call. The function address is pushed first, then
   all the parameters in call order. This functions pops all the
   parameters and the function address. */
int gfunc_call(int nb_args) {
    int r;
    int args_size;
    int i;

    args_size = 0;
    i = 0;
    while(lt(i, nb_args)) {
        r = gv(RC_INT);
        o(add(0x50, r)); /* push r */
        args_size = add(args_size, 4);
        vtop = sub(vtop, sizeof_SValue);
        i = add(i, 1);
    }
    save_regs(0); /* save used temporary registers */

    gcall_or_jmp(0);

    if (neq(args_size, 0)) {
        gadd_sp(args_size);
    }
    vtop = sub(vtop, sizeof_SValue);
}

/* generate function prolog of type 't' */
int gfunc_prolog(int func_type) {
    int addr;
    int align;
    int size;
    int param_addr;
    int sym;
    int type;

    enter();
    align = v_alloca(4);

    sym = gct_ref(func_type);
    addr = 8;
    loc = 0;

    ind = add(ind, FUNC_PROLOG_SIZE);
    func_sub_sp_offset = ind;
    /* define parameters */
    while (1) {
        sym = gsym_next(sym);
        if(eq(sym, 0)) {
            break;
        }
        type = gsym_type(sym);
        size = type_size(type, align);
        size = and(add(size, 3), not(3));
        param_addr = addr;
        addr = add(addr, size);
        sym_push(and(gsym_v(sym) , not(SYM_FIELD)), type,
                 or(VT_LOCAL, lvalue_type(gct_t(type))), param_addr);
    }
    leave(0);
}

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

/* generate a jump to a label */
int gjmp(int t) {
    return gjmp2(233, t); /* 0xe9 */
}

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

/* generate a test. set 'inv' to invert test. Stack entry is popped */
int gtst(int inv, int t) {
    g(15); /* 0x0f */
    t = gjmp2(xor(sub(gcv_i(gsv_c(vtop)), 16), inv), t);
    vtop = sub(vtop, sizeof_SValue);
    return t;
}

/* generate an integer binary operation */
int gen_opi(int op) {
    int r;
    int fr;
    int opc;
    int c;

    opc = 7;
    /* constant case */
    vswap();
    r = gv(RC_INT);
    vswap();
    c = gcv_i(gsv_c(vtop));
    o(131); /* 0x83 */
    o(or(or(192, shl(opc, 3)), r)); /* 0xc0 */
    g(c);
    vtop = sub(vtop, sizeof_SValue);
    ssv_r(vtop, VT_CMP);
    scv_i(gsv_c(vtop), op);
}

/* end of i386-gen.c */

/* start of libtcc.c */

/* copy a string and truncate it. */
int pstrcpy(int buf, int buf_size, int s) {
    int q;
    int q_end;
    int c;

    if (gt(buf_size, 0)) {
        q = buf;
        q_end = sub(add(buf, buf_size), 1);
        while (neq(sub(q, q_end), 0)) {
            c = ri8(s);
            s = add(s, 1);
            if (eq(c, 0)) {
                break;
            }
            wi8(q, c);
            q = add(q, 1);
        }
        wi8(q, 0);
    }
    return buf;
}

/********************************************************/
/* memory management */

int tcc_free(int ptr) {
    free(ptr);
}

int tcc_malloc(int size) {
    int ptr;
    ptr = malloc(size);
    return ptr;
}

int tcc_mallocz(int size) {
    int ptr;
    ptr = tcc_malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

int tcc_realloc(int ptr, int size) {
    int ptr1;
    ptr1 = realloc(ptr, size);
    return ptr1;
}

int tcc_strdup(int str) {
    int ptr;
    ptr = tcc_malloc(add(strlen(str), 1));
    strcpy(ptr, str);
    return ptr;
}

/********************************************************/
/* dynarrays */

int dynarray_add(int ptab, int nb_ptr, int data) {
    int nb;
    int nb_alloc;
    int pp;

    nb = ri32(nb_ptr);
    pp = ri32(ptab);
    /* every power of two we double array size */
    if (eq(and(nb, sub(nb, 1)), 0)) {
        if (eq(nb, 0)) {
            nb_alloc = 1;
        } else {
            nb_alloc = mul(nb, 2);
        }
        pp = tcc_realloc(pp, mul(nb_alloc, sizeof_void));
        wi32(ptab, pp);
    }
    wi32(add(pp, mul(nb, sizeof_void)), data);
    nb = add(nb, 1);
    wi32(nb_ptr, nb);
}

int dynarray_reset(int pp, int n) {
    int p;
    p = ri32(pp);
    while(neq(ri32(n),0 )) {
        if (neq(p,0)) {
            tcc_free(ri32(p));
        }
        p = add(p, sizeof_void);
        wi32(n, sub(ri32(n), 1));
    }
    wi32(n, 0);
    tcc_free(ri32(pp));
    wi32(pp, 0);
}

/********************************************************/
/* I/O layer */
int tcc_open_bf(int s1, int filename, int initlen) {
    int bf;
    int buflen;

    if(neq(initlen,0)) {
        buflen = initlen;
    } else {
        buflen = IO_BUF_SIZE;
    }

    bf = tcc_mallocz(add(sizeof_BufferedFile, buflen));
    sbf_buf_ptr(bf, gbf_buffer(bf));
    sbf_buf_end(bf, add(gbf_buffer(bf), initlen));
    wi8(gbf_buf_end(bf), CH_EOB); /* put eob symbol */
    pstrcpy(gbf_filename(bf), sizeof_BufferedFile_filename, filename);
    sbf_line_num(bf, 1);
    sbf_fd(bf, sub(0, 1));
    sbf_prev(bf, file);
    file = bf;
    tok_flags = or(TOK_FLAG_BOL, TOK_FLAG_BOF);
}

int tcc_close() {
    int bf;
    bf = file;
    if (gt(gbf_fd(bf), 0)) {
        close(gbf_fd(bf));
    }
    file = gbf_prev(bf);
    tcc_free(bf);
}

int tcc_open(int s1, int filename) {
    int fd;
    fd = open(filename, or(O_RDONLY, O_BINARY));
    tcc_open_bf(s1, filename, 0);
    sbf_fd(file, fd);
    return fd;
}

/* compile the file opened in 'file'. Return non zero if errors. */
int tcc_compile(int s1) {
    int filetype;

    filetype = gts_filetype(s1);
    tccelf_begin_file(s1);

    preprocess_start(s1, 0);
    tccgen_compile(s1);

    /* reset define stack, but keep -D and built-ins */
    sym_pop(aglobal_stack, 0, 0);
    sym_pop(alocal_stack, 0, 0);
    tccelf_end_file(s1);
    return 0;
}

int tcc_delete(int s1) {
    /* free sections */
    tccelf_delete(s1);

    tcc_free(gts_outfile(s1));
    dynarray_reset(ats_files(s1), ats_nb_files(s1));

    tcc_free(s1);
}

int tcc_add_file_internal(int s1, int filename, int flags) {
    int ret;

    /* open the file */
    ret = tcc_open(s1, filename);

    ret = tcc_compile(s1);
    tcc_close();
    return ret;
}

int tcc_add_file(int s, int filename) {
    int filetype;
    filetype = gts_filetype(s);
    return tcc_add_file_internal(s, filename, 0);
}

int strstart(int val, int str) {
    int p;
    int q;
    p = ri32(str);
    q = val;
    while (neq(ri8(q),0)) {
        if (neq(ri8(p), ri8(q))) {
            return 0;
        }
        p = add(p, 1);
        q = add(q, 1);
    }
    wi32(str, p);
    return 1;
}

int args_parser_add_file(int s, int filename, int filetype) {
    int f;
    f = tcc_malloc(add(sizeof_filespec, strlen(filename)));
    sfs_type(f, filetype);
    strcpy(gfs_name(f), filename);
    dynarray_add(ats_files(s), ats_nb_files(s), f);
}

int tcc_parse_args(int s, int pargc, int pargv, int optind) {
    int popt;
    int optarg;
    int r;
    int p1;
    int r1;
    int argc;
    int argv;

    argv = ri32(pargv);
    argc = ri32(pargc);

    enter();
    r1 = v_alloca(4);

    while (lt(optind, argc)) {
        r = ri32(add(argv, mul(optind, 4)));
        optind = add(optind, 1);
        if (or(neq(ri8(r), mkc('-')), eq(ri8(add(r, 1)), 0))) {
            args_parser_add_file(s, r, gts_filetype(s));
        } else {
            /* find option in table */
            popt = tcc_options;
            while(1) {
                p1 = gto_name(popt);
                wi32(r1, add(r, 1));
                if (strstart(p1, r1)) {
                    optarg = r1;
                    if (and(gto_flags(popt), TCC_OPTION_HAS_ARG)) {
                        if (and(eq(ri8(ri32(r1)), 0),
                            eq(and(gto_flags(popt), TCC_OPTION_NOSEP), 0))) {
                            optarg = ri32(add(argv, mul(optind, 4)));
                            optind = add(optind, 1);
                        }
                    }
                    break;
                }
                popt = add(popt, sizeof_TCCOption);
            }

            if(eq(gto_index(popt), TCC_OPTION_o)) {
                sts_outfile(s, tcc_strdup(optarg));
            }
        }
    }

    return leave(0);
}

int init_option(int t,int name, int index, int flags) {
  sto_name(t, name);
  sto_index(t, index);
  sto_flags(t, flags);
  return add(t, sizeof_TCCOption);
}

int init_options() {
  int t;

  TCC_OPTION_c = 0;
  TCC_OPTION_o = 1;
  TCC_OPTION_nostdinc = 2;

  TCC_OPTION_HAS_ARG = 1;
  TCC_OPTION_NOSEP = 2; /* 0x0002 cannot have space before option and arg */

  tcc_options = tcc_mallocz(mul(sizeof_TCCOption, 4));

  t = tcc_options;
  t = init_option(t, mks("c"), TCC_OPTION_c, 0);
  t = init_option(t, mks("o"), TCC_OPTION_o, TCC_OPTION_HAS_ARG);
  t = init_option(t, mks("nostdinc"), TCC_OPTION_nostdinc, 1);
  t = init_option(t, 0, 0, 0);
}

/* end of libtcc.c */

/* start of tccelf.c */

/* ------------------------------------------------------------------------- */
/* 1 */
int tccelf_new(int s) {
    /* no section zero */
    dynarray_add(ats_sections(s), ats_nb_sections(s), 0);

    /* create standard sections */
    text_section = new_section(s, mks(".text"), SHT_PROGBITS, or(SHF_ALLOC, SHF_EXECINSTR));
    data_section = new_section(s, mks(".data"), SHT_PROGBITS, or(SHF_ALLOC, SHF_WRITE));
    bss_section = new_section(s, mks(".bss"), SHT_NOBITS, or(SHF_ALLOC, SHF_WRITE));
    common_section = new_section(s, mks(".common"), SHT_NOBITS, SHF_PRIVATE);
    ss_sh_num(common_section, SHN_COMMON);

    /* symbols are always generated for linking stage */
    symtab_section = new_symtab(s, mks(".symtab"), SHT_SYMTAB, 0,
                                mks(".strtab"),
                                mks(".hashtab"), SHF_PRIVATE);
    sts_symtab(s, symtab_section);

}

/* 2 */
int free_section(int s) {
    tcc_free(gs_data(s));
}

/* 3 */
int tccelf_delete(int s1) {
    int i;

    /* free all sections */
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        free_section(ri32(add(gts_sections(s1), mul(i, 4))));
        i = add(i, 1);
    }
    dynarray_reset(ats_sections(s1), ats_nb_sections(s1));

    i = 0;
    while(lt(i, gts_nb_priv_sections(s1))) {
        free_section(ri32(add(gts_priv_sections(s1), mul(i, 4))));
        i = add(i, 1);
    }
    dynarray_reset(ats_priv_sections(s1), ats_nb_priv_sections(s1));

    symtab_section = 0; /* for tccrun.c:rt_printline() */
}

/* 4 */
/* save section data state */
int tccelf_begin_file(int s1) {
    int s;
    int i;
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        s = ri32(add(gts_sections(s1), mul(i, 4)));
        ss_sh_offset(s, gs_data_offset(s));
        i = add(i, 1);
    }
    /* disable symbol hashing during compilation */
    s = gts_symtab(s1);
    ss_reloc(s, gs_hash(s));
    ss_hash(s, 0);
}

/* 6 */
int new_section(int s1, int name, int sh_type, int sh_flags) {
    int sec;

    sec = tcc_mallocz(add(sizeof_Section, strlen(name)));
    strcpy(gs_name(sec), name);
    ss_sh_type(sec, sh_type);
    ss_sh_flags(sec, sh_flags);

    if (eq(sh_type, SHT_STRTAB)) {
        ss_sh_addralign(sec, 1);
    } else {
        ss_sh_addralign(sec, 4);
    }

    if (and(sh_flags, SHF_PRIVATE)) {
        dynarray_add(ats_priv_sections(s1), ats_nb_priv_sections(s1), sec);
    } else {
        ss_sh_num(sec, gts_nb_sections(s1));
        dynarray_add(ats_sections(s1), ats_nb_sections(s1), sec);
    }

    return sec;
}

/* 8 */
/* realloc section and set its content to zero */
int section_realloc(int sec, int new_size) {
    int size;
    int data;

    size = gs_data_allocated(sec);
    if (eq(size, 0)) {
        size = 1;
    }
    while (lt(size, new_size)) {
        size = mul(size, 2);
    }
    data = tcc_realloc(gs_data(sec), size);
    memset(add(data, gs_data_allocated(sec)), 0, sub(size, gs_data_allocated(sec)));
    ss_data(sec, data);
    ss_data_allocated(sec, size);
}

/* 9 */
/* reserve at least 'size' bytes aligned per 'align' in section
   'sec' from current offset, and return the aligned offset */
int section_add(int sec, int size, int align) {
    int offset;
    int offset1;

    offset = and(sub(add(gs_data_offset(sec), align), 1), sub(0, align));
    offset1 = add(offset, size);
    if (and(neq(gs_sh_type(sec), SHT_NOBITS), gt(offset1, gs_data_allocated(sec)))) {
        section_realloc(sec, offset1);
    }
    ss_data_offset(sec, offset1);
    if (gt(align, gs_sh_addralign(sec))) {
        ss_sh_addralign(sec, align);
    }
    return offset;
}

/* 11 */
/* reserve at least 'size' bytes from section start */
int section_reserve(int sec, int size) {
    if (gt(size, gs_data_allocated(sec))) {
        section_realloc(sec, size);
    }
    if (gt(size, gs_data_offset(sec))) {
        ss_data_offset(sec, size);
    }
}

/* ------------------------------------------------------------------------- */
/* 12 */
int put_elf_str(int s, int sym) {
    int offset;
    int len;
    int ptr;

    len = add(strlen(sym), 1);
    offset = gs_data_offset(s);
    ptr = section_ptr_add(s, len);
    memmove(ptr, sym, len);
    return offset;
}

/* 13 */
/* elf symbol hashing function */
int elf_hash(int name) {
    int h;
    int g;

    h = 0;

    while (neq(0, ri8(name))) {
        h = add(shl(h, 4), ri8(name));
        name = add(name, 1);
        g = and(h, shl(15, 28));  /* 0xf0000000 */
        if (g) {
            h = xor(h, and(shr(g, 24), 255)); /* ljw see original extra bit
                                                 twiddling for sign */
        }
        h = and(h, not(g));
    }
    return h;
}

/* 21 */
/* Assign sections to segments and decide how are sections laid out when loaded
   in memory. This function also fills corresponding program headers. */
int layout_sections(int s1, int phdr, int phnum,
                           int interp, int strsec,
                           int dyninf, int sec_order) {
    int i;
    int sh_order_index;
    int file_offset;
    int ph;
    int s;

    sh_order_index = 1;
    file_offset = 0;
    file_offset = add(sizeof_Elf32_Ehdr, mul(phnum, sizeof_Elf32_Phdr));

    /* all other sections come after */
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        s = ri32(add(gts_sections(s1), mul(i, 4)));
        wi32(add(sec_order, mul(sh_order_index, 4)), i);
        sh_order_index = add(sh_order_index, 1);

        file_offset = and((add(file_offset, sub(gs_sh_addralign(s), 1))),
                          not(sub(gs_sh_addralign(s), 1)));
        ss_sh_offset(s, file_offset);
        if (neq(gs_sh_type(s), SHT_NOBITS)) {
            file_offset = add(file_offset, gs_sh_size(s));
        }
        i = add(i, 1);
    }

    return file_offset;
}

/* 23 */
/* Write an elf file */
int tcc_write_elf_file(int s1, int filename, int phnum,
                              int phdr, int file_offset, int sec_order) {
    int fd;
    int mode;
    int f;

    unlink(filename);

    f = fopen(filename, mks("wb"));
    tcc_output_elf(s1, f, phnum, phdr, file_offset, sec_order);
    fclose(f);

    return 0;
}

/* 24 */
/* Output an elf file */
int elf_output_file(int s1, int filename) {
    int i;
    int ret;
    int phnum;
    int shnum;
    int file_offset;
    int sec_order;
    int phdr;
    int sym;
    int strsec;
    int interp;
    int textrel;

    ret = sub(0, 1);
    phdr = 0;
    sec_order = 0;
    interp = 0; /* avoid warning */
    textrel = 0;

    /* we add a section for symbols */
    strsec = new_section(s1, mks(".shstrtab"), SHT_STRTAB, 0);
    put_elf_str(strsec, mks(""));

    /* Allocate strings for section names */
    textrel = alloc_sec_names(s1, strsec);

    /* compute number of program headers */
    phnum = 0;

    /* allocate program segment headers */
    phdr = tcc_mallocz(mul(phnum, sizeof_Elf32_Phdr));

    /* compute number of sections */
    shnum = gts_nb_sections(s1);

    /* this array is used to reorder sections in the output file */
    sec_order = tcc_malloc(mul(4, shnum));
    wi32(sec_order, 0);

    /* compute section to program header mapping */
    file_offset = layout_sections(s1, phdr, phnum, interp, strsec, 0,
                                  sec_order);

    /* Create the ELF file with name 'filename' */
    ret = tcc_write_elf_file(s1, filename, phnum, phdr, file_offset, sec_order);
    sts_nb_sections(s1, shnum);
    tcc_free(sec_order);
    tcc_free(phdr);
    return ret;
}

/* 25 */
int tcc_output_file(int s, int filename) {
    int ret;
    ret = elf_output_file(s, filename);
    return ret;
}

/* end of tccelf.c */

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
