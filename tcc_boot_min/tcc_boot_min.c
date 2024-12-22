
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

int gs_sh_name(int o) {return ri32(add(o, Section_sh_name_o));}
int ss_sh_name(int o, int v) {wi32(add(o, Section_sh_name_o), v);}

int gs_sh_num(int o) {return ri32(add(o, Section_sh_num_o));}
int ss_sh_num(int o, int v) {wi32(add(o, Section_sh_num_o), v);}

int gs_sh_type(int o) {return ri32(add(o, Section_sh_type_o));}
int ss_sh_type(int o, int v) {wi32(add(o, Section_sh_type_o), v);}

int gs_sh_flags(int o) {return ri32(add(o, Section_sh_flags_o));}
int ss_sh_flags(int o, int v) {wi32(add(o, Section_sh_flags_o), v);}

int gs_sh_info(int o) {return ri32(add(o, Section_sh_info_o));}
int ss_sh_info(int o, int v) {wi32(add(o, Section_sh_info_o), v);}

int gs_sh_addralign(int o) {return ri32(add(o, Section_sh_addralign_o));}
int ss_sh_addralign(int o, int v) {wi32(add(o, Section_sh_addralign_o), v);}

int gs_sh_entsize(int o) {return ri32(add(o, Section_sh_entsize_o));}
int ss_sh_entsize(int o, int v) {wi32(add(o, Section_sh_entsize_o), v);}

int gs_sh_size(int o) {return ri32(add(o, Section_sh_size_o));}
int ss_sh_size(int o, int v) {wi32(add(o, Section_sh_size_o), v);}

int gs_sh_addr(int o) {return ri32(add(o, Section_sh_addr_o));}
int ss_sh_addr(int o, int v) {wi32(add(o, Section_sh_addr_o), v);}

int gs_sh_offset(int o) {return ri32(add(o, Section_sh_offset_o));}
int ss_sh_offset(int o, int v) {wi32(add(o, Section_sh_offset_o), v);}

int gs_nb_hashed_syms(int o) {return ri32(add(o, Section_nb_hashed_syms_o));}
int ss_nb_hashed_syms(int o, int v) {wi32(add(o, Section_nb_hashed_syms_o), v);}

int gs_link(int o) {return ri32(add(o, Section_link_o));}
int ss_link(int o, int v) {wi32(add(o, Section_link_o), v);}

int gs_reloc(int o) {return ri32(add(o, Section_reloc_o));}
int ss_reloc(int o, int v) {wi32(add(o, Section_reloc_o), v);}

int gs_hash(int o) {return ri32(add(o, Section_hash_o));}
int ss_hash(int o, int v) {wi32(add(o, Section_hash_o), v);}

int gs_name(int o) {return add(o, Section_name_o);}

/* SValue accessors */

/* CType type */
int gsv_type(int o) {return add(o, SValue_type_o);}

int gsv_r(int o) {return ri32(add(o, SValue_r_o));} /* int r */
int ssv_r(int o, int v) {wi32(add(o, SValue_r_o), v);}

int gsv_c(int o) { return add(o, SValue_c_o);} /* CValue c */

int gsv_sym(int o) { return ri32(add(o, SValue_sym_o));} /* struct Sym *sym */
int ssv_sym(int o, int v) {wi32(add(o, SValue_sym_o), v);}

/* CValue accessors */

int acv_i(int o) {return add(o, CValue_i_o);}
int gcv_i(int o) {return ri32(add(o, CValue_i_o));}
int scv_i(int o, int v) {wi32(add(o, CValue_i_o), v);}

int gcv_str_size(int o) {return ri32(add(o, CValue_str_size_o));}
int scv_str_size(int o, int v) {wi32(add(o, CValue_str_size_o), v);}

int gcv_str_data(int o) {return ri32(add(o, CValue_str_data_o));}
int scv_str_data(int o, int v) {wi32(add(o, CValue_str_data_o), v);}

/*  int tab[1]; */
int gcv_tab(int o) {return add(o, CValue_tab_o);}

/* CType accessors */

int gct_t(int o) {return ri32(add(o, CType_t_o));}
int sct_t(int o, int v) {wi32(add(o, CType_t_o), v);}

int gct_ref(int o) {return ri32(add(o, CType_ref_o));}
int sct_ref(int o, int v) {wi32(add(o, CType_ref_o), v);}

/* Sym accessors */

int gsym_v(int o) {return ri32(add(o, Sym_v_o));} /* int v */
int ssym_v(int o, int v) {wi32(add(o, Sym_v_o), v);}

int gsym_r(int o) {return ri32(add(o, Sym_r_o));}
int ssym_r(int o, int v) {wi32(add(o, Sym_r_o), v);}

int gsym_c(int o) {return ri32(add(o, Sym_c_o));}
int ssym_c(int o, int v) {wi32(add(o, Sym_c_o), v);}

int gsym_sym_scope(int o) {return ri32(add(o, Sym_sym_scope_o));}
int ssym_sym_scope(int o, int v) {wi32(add(o, Sym_sym_scope_o), v);}

/* struct FuncAttr->func_type */
int gsym_f_func_type(int o) {return ri32(add(o, Sym_f_func_type_o));}
int ssym_f_func_type(int o, int v) {wi32(add(o, Sym_f_func_type_o), v);}

int gsym_type(int o) {return add(o, Sym_type_o);} /* CType type */

/* struct Sym *next */
int asym_next(int o) {return add(o, Sym_next_o);}
int gsym_next(int o) {return ri32(add(o, Sym_next_o));}
int ssym_next(int o, int v) {wi32(add(o, Sym_next_o), v);}

/* struct Sym *prev */
int gsym_prev(int o) {return ri32(add(o, Sym_prev_o));}
int ssym_prev(int o, int v) {wi32(add(o, Sym_prev_o), v);}

int asym_prev_tok(int o) {return add(o, Sym_prev_tok_o);}
int gsym_prev_tok(int o) {return ri32(add(o, Sym_prev_tok_o));}
int ssym_prev_tok(int o, int v) {wi32(add(o, Sym_prev_tok_o), v);}

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

/* Elf32_Rel accessors */
int ger_r_offset(int o) {return ri32(add(o, Elf32_Rel_r_offset_o));}
int ser_r_offset(int o, int v) {wi32(add(o, Elf32_Rel_r_offset_o), v);}

int ger_r_info(int o) {return ri32(add(o, Elf32_Rel_r_info_o));}
int ser_r_info(int o, int v) {wi32(add(o, Elf32_Rel_r_info_o), v);}

/* Elf32_Sym accessors */
int ges_st_name(int o) {return ri32(add(o, Elf32_Sym_st_name_o));}
int ses_st_name(int o, int v) {wi32(add(o, Elf32_Sym_st_name_o), v);}

int ges_st_size(int o) {return ri32(add(o, Elf32_Sym_st_size_o));}
int ses_st_size(int o, int v) {wi32(add(o, Elf32_Sym_st_size_o), v);}

int ges_st_value(int o) {return ri32(add(o, Elf32_Sym_st_value_o));}
int ses_st_value(int o, int v) {wi32(add(o, Elf32_Sym_st_value_o), v);}

int ges_st_info(int o) {return ri8(add(o, Elf32_Sym_st_info_o));}
int ses_st_info(int o, int v) {wi8(add(o, Elf32_Sym_st_info_o), v);}

int ges_st_other(int o) {return ri8(add(o, Elf32_Sym_st_other_o));}
int ses_st_other(int o, int v) {wi8(add(o, Elf32_Sym_st_other_o), v);}

int ges_st_shndx(int o) {return ri16(add(o, Elf32_Sym_st_shndx_o));}
int ses_st_shndx(int o, int v) {wi16(add(o, Elf32_Sym_st_shndx_o), v);}

/* Elf32_Ehdr accessors */

/* unsigned char e_ident[16] */
int gee_e_ident(int o) { return add(o, Elf32_Ehdr_e_ident_o);}

/*  Elf32_Half e_type */
int see_e_type(int o, int v) {wi16(add(o, Elf32_Ehdr_e_type_o), v);}

/*  Elf32_Half e_machine */
int see_e_machine(int o, int v) {wi16(add(o, Elf32_Ehdr_e_machine_o), v);}

/*  Elf32_Word e_version */
int see_e_version(int o, int v) {wi32(add(o, Elf32_Ehdr_e_version_o), v);}

/*  Elf32_Off e_shoff */
int gee_e_shoff(int o) {return ri32(add(o, Elf32_Ehdr_e_shoff_o));}
int see_e_shoff(int o, int v) {wi32(add(o, Elf32_Ehdr_e_shoff_o), v);}

/*  Elf32_Half e_ehsize */
int see_e_ehsize(int o, int v) {wi16(add(o, Elf32_Ehdr_e_ehsize_o), v);}

/*  Elf32_Half e_shentsize */
int see_e_shentsize(int o, int v) {wi16(add(o, Elf32_Ehdr_e_shentsize_o), v);}

/*  Elf32_Half e_shnum */
int see_e_shnum(int o, int v) {wi16(add(o, Elf32_Ehdr_e_shnum_o), v);}

/*  Elf32_Half e_shstrndx */
int see_e_shstrndx(int o, int v) {wi16(add(o, Elf32_Ehdr_e_shstrndx_o), v);}

/* Elf32_Shdr accessors */

int sesh_sh_name(int o, int v) {wi32(add(o, Elf32_Shdr_sh_name_o), v);}

int sesh_sh_type(int o, int v) {wi32(add(o, Elf32_Shdr_sh_type_o), v);}

int sesh_sh_flags(int o, int v) {wi32(add(o, Elf32_Shdr_sh_flags_o), v);}

int sesh_sh_addr(int o, int v) {wi32(add(o, Elf32_Shdr_sh_addr_o), v);}

int sesh_sh_offset(int o, int v) {wi32(add(o, Elf32_Shdr_sh_offset_o), v);}

int sesh_sh_size(int o, int v) {wi32(add(o, Elf32_Shdr_sh_size_o), v);}

int sesh_sh_link(int o, int v) {wi32(add(o, Elf32_Shdr_sh_link_o), v);}

int sesh_sh_info(int o, int v) {wi32(add(o, Elf32_Shdr_sh_info_o), v);}

int sesh_sh_addralign(int o, int v) {wi32(add(o, Elf32_Shdr_sh_addralign_o), v);}

int sesh_sh_entsize(int o, int v) {wi32(add(o, Elf32_Shdr_sh_entsize_o), v);}

/* CString accessors */

int gcs_size(int o) {return ri32(add(o, CString_size_o));}
int scs_size(int o, int v) {wi32(add(o, CString_size_o), v);}

int gcs_data(int o) {return ri32(add(o, CString_data_o));}
int scs_data(int o, int v) {wi32(add(o, CString_data_o), v);}

int gcs_size_allocated(int o) {return ri32(add(o, CString_size_allocated_o));}
int scs_size_allocated(int o, int v) {wi32(add(o, CString_size_allocated_o), v);}

/* TokenSym accessors */

int atks_hash_next(int o) {return add(o, TokenSym_hash_next_o);}
int gtks_hash_next(int o) {return ri32(add(o, TokenSym_hash_next_o));}
int stks_hash_next(int o, int v) {wi32(add(o, TokenSym_hash_next_o), v);}

int atks_sym_identifier(int o) {return add(o, TokenSym_sym_identifier_o);}
int gtks_sym_identifier(int o) {return ri32(add(o, TokenSym_sym_identifier_o));}
int stks_sym_identifier(int o, int v) {wi32(add(o, TokenSym_sym_identifier_o), v);}

int gtks_tok(int o) {return ri32(add(o, TokenSym_tok_o));}
int stks_tok(int o, int v) {wi32(add(o, TokenSym_tok_o), v);}

int gtks_len(int o) {return ri32(add(o, TokenSym_len_o));}
int stks_len(int o, int v) {wi32(add(o, TokenSym_len_o), v);}

/* char str[1] */
int gtks_str(int o) {return add(o, TokenSym_str_o);}

/* TokenString accessors */

int gtkst_str(int o) {return ri32(add(o, TokenString_str_o));}
int stkst_str(int o, int v) {wi32(add(o, TokenString_str_o), v);}

int gtkst_len(int o) {return ri32(add(o, TokenString_len_o));}
int stkst_len(int o, int v) {wi32(add(o, TokenString_len_o), v);}

int gtkst_lastlen(int o) {return ri32(add(o, TokenString_lastlen_o));}
int stkst_lastlen(int o, int v) {wi32(add(o, TokenString_lastlen_o), v);}

int gtkst_allocated_len(int o) {return ri32(add(o, TokenString_allocated_len_o));}
int stkst_allocated_len(int o, int v) {wi32(add(o, TokenString_allocated_len_o), v);}

int gtkst_prev(int o) {return ri32(add(o, TokenString_prev_o));}
int stkst_prev(int o, int v) {wi32(add(o, TokenString_prev_o), v);}

int gtkst_prev_ptr(int o) {return ri32(add(o, TokenString_prev_ptr_o));}
int stkst_prev_ptr(int o, int v) {wi32(add(o, TokenString_prev_ptr_o), v);}

/* char alloc */
int gtkst_alloc(int o) {return ri32(add(o, TokenString_alloc_o));}
int stkst_alloc(int o, int v) {wi32(add(o, TokenString_alloc_o), v);}

/* AttributeDef accesors */

/* f->func_type */
int gad_f_func_type(int o) {return ri32(add(o, AttributeDef_f_func_type_o));}
int sad_f_func_type(int o, int v) {wi32(add(o, AttributeDef_f_func_type_o), v);}

int gad_section(int o) {return ri32(add(o, AttributeDef_section_o));}

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
  Section_sh_name_o = 12;
  Section_sh_num_o = 16;
  Section_sh_type_o = 20;
  Section_sh_flags_o = 24;
  Section_sh_info_o = 28;
  Section_sh_addralign_o = 32;
  Section_sh_entsize_o = 36;
  Section_sh_size_o = 40;
  Section_sh_addr_o = 44;
  Section_sh_offset_o = 48;
  Section_nb_hashed_syms_o = 52;
  Section_link_o = 56;
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
  CValue_str_size_o = 0;
  CValue_str_data_o = 4;
  CValue_tab_o = 0;
  sizeof_CValue = 8;

  CType_t_o = 0;
  CType_ref_o = 4;
  sizeof_CType = 8;

  Sym_v_o = 0;
  Sym_r_o = 4;
  Sym_c_o = 8;
  Sym_sym_scope_o = 12;
  Sym_f_func_type_o = 16;
  Sym_type_o = 20;
  Sym_next_o = 28;
  Sym_prev_o = 32;
  Sym_prev_tok_o = 36;
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

  Elf32_Ehdr_e_ident_o = 0;
  Elf32_Ehdr_e_type_o = 16;
  Elf32_Ehdr_e_machine_o = 18;
  Elf32_Ehdr_e_version_o = 20;
  Elf32_Ehdr_e_entry_o = 24;
  Elf32_Ehdr_e_phoff_o = 28;
  Elf32_Ehdr_e_shoff_o = 32;
  Elf32_Ehdr_e_flags_o = 36;
  Elf32_Ehdr_e_ehsize_o = 40;
  Elf32_Ehdr_e_phentsize_o = 42;
  Elf32_Ehdr_e_phnum_o = 44;
  Elf32_Ehdr_e_shentsize_o = 46;
  Elf32_Ehdr_e_shnum_o = 48;
  Elf32_Ehdr_e_shstrndx_o = 50;
  sizeof_Elf32_Ehdr = 52;

  sizeof_Elf32_Phdr = 32;

  Elf32_Rel_r_offset_o = 0;
  Elf32_Rel_r_info_o = 4;
  sizeof_Elf32_Rel = 8;

  Elf32_Shdr_sh_name_o = 0;
  Elf32_Shdr_sh_type_o = 4;
  Elf32_Shdr_sh_flags_o = 8;
  Elf32_Shdr_sh_addr_o = 12;
  Elf32_Shdr_sh_offset_o = 16;
  Elf32_Shdr_sh_size_o = 20;
  Elf32_Shdr_sh_link_o = 24;
  Elf32_Shdr_sh_info_o = 28;
  Elf32_Shdr_sh_addralign_o = 32;
  Elf32_Shdr_sh_entsize_o = 36;
  sizeof_Elf32_Shdr = 40;

  Elf32_Sym_st_name_o = 0;
  Elf32_Sym_st_value_o = 4;
  Elf32_Sym_st_size_o = 8;
  Elf32_Sym_st_info_o = 12;
  Elf32_Sym_st_other_o = 13;
  Elf32_Sym_st_shndx_o = 14;
  sizeof_Elf32_Sym = 16;

  CString_size_o = 0;
  CString_data_o = 4;
  CString_size_allocated_o = 8;
  sizeof_CString = 12;

  TokenSym_hash_next_o = 0;
  TokenSym_sym_identifier_o = 4;
  TokenSym_tok_o = 8;
  TokenSym_len_o = 12;
  TokenSym_str_o = 16;
  sizeof_TokenSym = 20;

  TokenString_str_o = 0;
  TokenString_len_o = 4;
  TokenString_lastlen_o = 8;
  TokenString_allocated_len_o = 12;
  TokenString_prev_o = 16;
  TokenString_prev_ptr_o = 20;
  TokenString_alloc_o = 24;
  sizeof_TokenString = 28;

  AttributeDef_f_func_type_o = 0;
  AttributeDef_section_o = 4;
  sizeof_AttributeDef = 8;

  sizeof_void = 4;
  sizeof_int = sizeof_void;

  /* stored in 'Sym->f.func_type' field */
  FUNC_NEW = 1; /*  1  ansi function prototype */
  FUNC_OLD = 2; /*  2  old function prototype */

  FUNC_PROLOG_SIZE = 9;


  VT_BYTE = 1;    /*   signed byte type */
  VT_INT = 3;    /*    3  integer type */
  VT_PTR = 5;    /*    5  pointer */
  VT_FUNC = 6;   /*    6 function type */
  VT_BTYPE = 15; /*    0x000f  mask for basic type */
  VT_UNSIGNED = 16;    /*  0x0010  unsigned type */
  VT_ARRAY = 64;       /*  0x0040  array type (also has VT_PTR) */
  VT_CONSTANT = 256;   /*  0x0100  const modifier */

  VT_VALMASK = 63;
  VT_CONST = 48;
  VT_LOCAL = 50;
  VT_CMP = 51;
  VT_LVAL = 256;
  VT_SYM = 512;
  VT_EXTERN = 4096; /*  0x00001000  extern definition */
  VT_STATIC = 8192; /*  0x00002000  static variable */
  VT_STORAGE = or(VT_EXTERN, VT_STATIC);
  VT_TYPE = not(VT_STORAGE);

  R_386_32 = 1;
  R_386_PC32 = 2;

  RC_INT = 1;

  RC_EAX = 4;
  RC_ECX = 16;
  RC_IRET = RC_EAX;

  TREG_EAX = 0;
  REG_IRET = TREG_EAX;

  SYM_STRUCT = 1073741824; /* 0x40000000 struct/union/enum symbol space */
  SYM_FIELD = 536870912; /*  0x20000000 struct/union field symbol space */
  SYM_FIRST_ANOM = 268435456; /* 0x10000000 first anonymous sym */

  SYM_POOL_NB = div_(8192, sizeof_Sym);

  SHF_WRITE = shl(1, 0); /* Writable */
  SHF_ALLOC = shl(1, 1);  /* Occupies memory during execution */
  SHF_EXECINSTR = shl(1, 2); /* Executable */
/* special flag to indicate that the section should not be linked to the other ones */
  SHF_PRIVATE = shl(1, 31); /* 0x80000000 */

  SHN_UNDEF = 0;  /* 0 Undefined section */
  SHN_ABS = 65521;     /* 0xfff1 Associated symbol is absolute */
  SHN_COMMON = 65522;  /* 0xfff2 Associated symbol is common */

  SHT_PROGBITS = 1; /* Program data */
  SHT_SYMTAB = 2;  /* Symbol table */
  SHT_STRTAB = 3; /* String table */
  SHT_RELA = 4; /* Relocation entries with addends */
  SHT_HASH = 5;  /* Symbol hash table */
  SHT_NOBITS = 8;  /* Program space with no data (bss) */
  SHT_REL = 9; /* Relocation entries, no addends */
  SHT_RELX = SHT_REL;

  STB_LOCAL = 0; /* Local symbol */
  STB_GLOBAL = 1; /* Global symbol */

  STT_OBJECT = 1; /*     1            Symbol is a data object */
  STT_FUNC = 2;   /*     2            Symbol is a code object */
  STT_FILE = 4; /* Symbol's name is file name */

  O_RDONLY = 0;
  O_BINARY = 0;

  IO_BUF_SIZE = 8192;

  CH_EOB = mkc('\\');

  TOK_FLAG_BOL = 1;
  TOK_FLAG_BOF = 2;

  /* all identifiers and strings have token above that */
  TOK_IDENT = 256;

  TOK_ALLOC_INCR = 512;  /* must be a power of two */
  TOK_MAX_SIZE = 4;  /* token max size in int unit when stored in string */

  TOK_STR = 185; /* 0xb9 pointer to string in tokc */
  TOK_PPNUM = 190; /* 0xbe preprocessor number */
  TOK_PPSTR = 191; /* 0xbf preprocessor string */

  TOK_CCHAR = 179; /* 0xb3 char constant in tokc */
  TOK_CINT = 181; /* 0xb5 number in tokc */

  TOK_EOF = sub(0, 1); /* (-1)  end of file */

  /* warning: the following compare tokens depend on i386 asm code */
  TOK_NE = 149; /* 0x95 */


  REL_SECTION_FMT = mks(".rel%s");

  reg_classes=calloc(1,mul(4, 5)); /* NB_REGS is 5 (sort of) */

  tok = 0;

  /* isidnum_table flags: */
  IS_SPC = 1;
  IS_ID = 2;
  IS_NUM = 4;

  PARSE_FLAG_PREPROCESS = 1; /* 0x0001 activate preprocessing */
  PARSE_FLAG_TOK_NUM = 2;  /*  0x0002 return numbers instead of TOK_PPNUM */
  PARSE_FLAG_LINEFEED = 4;  /*  0x0004 line feed is returned as a
                                        token. line feed is also
                                        returned at eof */
  PARSE_FLAG_SPACES = 16;  /*  0x0010 next() returns space tokens (for -E) */
  PARSE_FLAG_TOK_STR = 64; /*  0x0040 return parsed strings instead of TOK_PPSTR */



  init_c();
  init_reg_classes();
  init_globals();

  init_options();
  init_tccpp_globals();
  init_tccgen_globals();
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
    return leave(0);
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

int ELFW_ST_BIND(int val) {
  return and(shr(val, 4),15);
}

int ELFW_ST_TYPE(int val) {
  return and(val,15);
}

int ELFW_ST_INFO(int bind, int type) {
   return add(shl(bind, 4), and(type, 15));
}

int ELFW_ST_VISIBILITY(int o) {
    return and(o, 3);   /* ((o) & 0x03) */
}

int ELFW_R_INFO(int sym, int type) {
   return add(shl(sym, 8), and(type, 255));
}

int ELFW_R_SYM(int val) {
  return and(shr(val,8), 16777215); /* unsigned ((val) >> 8) */
}

int ELFW_R_TYPE(int val) {
  return and(val,255);
}

/* ------------------------------------------------------------------------- */
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

int free_section(int s) {
    tcc_free(gs_data(s));
}

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

/* At the end of compilation, convert any UNDEF syms to global, and merge
   with previously existing symbols */
int tccelf_end_file(int s1) {
    int s;
    int first_sym;
    int nb_syms;
    int tr;
    int i;
    int sr;
    int n;
    int rel;
    int rel_end;
    int sym;

    s = gts_symtab(s1);

    first_sym = div_(gs_sh_offset(s), sizeof_Elf32_Sym);
    nb_syms = sub(div_(gs_data_offset(s), sizeof_Elf32_Sym), first_sym);
    ss_data_offset(s, gs_sh_offset(s));
    ss_data_offset(gs_link(s), gs_sh_offset(gs_link(s)));
    ss_hash(s, gs_reloc(s));
    ss_reloc(s, 0);
    tr = tcc_mallocz(mul(nb_syms, 4));

    i = 0;
    while(lt(i, nb_syms)) {
        sym = add(gs_data(s), mul(add(first_sym, i), sizeof_Elf32_Sym));
        if (and(eq(ges_st_shndx(sym), SHN_UNDEF),
            eq(ELFW_ST_BIND(ges_st_info(sym)), STB_LOCAL))) {
            ses_st_info(sym, ELFW_ST_INFO(STB_GLOBAL,
                             ELFW_ST_TYPE(ges_st_info(sym))));
        }
        wi32(add(tr, mul(i, 4)),
             set_elf_sym(s, ges_st_value(sym), ges_st_size(sym), ges_st_info(sym),
             ges_st_other(sym), ges_st_shndx(sym), add(gs_data(gs_link(s)),
                                               ges_st_name(sym))));

        i = add(i, 1);
    }
    /* now update relocations */
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        sr = add(gts_sections(s1),mul(i, 4));
        if (and(eq(gs_sh_type(sr), SHT_RELX), eq(gs_link(sr), s))) {
            rel = add(gs_data(sr), gs_sh_offset(sr));
            rel_end = add(gs_data(sr), gs_data_offset(sr));
            while(lt(rel, rel_end)) {
                n = sub(ELFW_R_SYM(ger_r_info(rel)), first_sym);
                ser_r_info(rel, ELFW_R_INFO(ri32(add(tr, mul(n, 4))),
                                            ELFW_R_TYPE(ger_r_info(rel))));
                rel = add(rel, sizeof_Elf32_Rel);
            }
        }
        i = add(i, 1);
    }
    tcc_free(tr);
}

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

int new_symtab(int s1, int symtab_name, int sh_type, int sh_flags,
               int strtab_name, int hash_name, int hash_sh_flags) {
    int symtab;
    int strtab;
    int hash;
    int ptr;
    int nb_buckets;

    symtab = new_section(s1, symtab_name, sh_type, sh_flags);
    ss_sh_entsize(symtab, sizeof_Elf32_Sym);
    strtab = new_section(s1, strtab_name, SHT_STRTAB, sh_flags);
    put_elf_str(strtab, mks(""));
    ss_link(symtab, strtab);
    put_elf_sym(symtab, 0, 0, 0, 0, 0, 0);

    nb_buckets = 1;

    hash = new_section(s1, hash_name, SHT_HASH, hash_sh_flags);
    ss_sh_entsize(hash, 4);
    ss_hash(symtab, hash);
    ss_link(hash, symtab);

    ptr = section_ptr_add(hash, mul(add(add(2, nb_buckets), 1), 4));
    wi32(ptr, nb_buckets);
    wi32(add(ptr, 4), 1);
    memset(add(ptr, 2), 0, mul(add(nb_buckets, 1), 4));
    return symtab;
}

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

/* reserve at least 'size' bytes in section 'sec' from
   sec->data_offset. */
int section_ptr_add(int sec, int size) {
    int offset;
    offset = section_add(sec, size, 1);
    return add(gs_data(sec), offset);
}

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

/* rebuild hash table of section s */
/* NOTE: we do factorize the hash table code to go faster */
int rebuild_hash(int s, int nb_buckets) {
    int sym;
    int ptr;
    int hash;
    int nb_syms;
    int sym_index;
    int h;
    int strtab;

    strtab = gs_data(gs_link(s));
    nb_syms = div_(gs_data_offset(s), sizeof_Elf32_Sym);

    if (eq(nb_buckets, 0)) {
        nb_buckets = ri32(gs_data(gs_hash(s)));
    }

    ss_data_offset(gs_hash(s), 0);
    ptr = section_ptr_add(gs_hash(s), mul(add(add(2, nb_buckets), nb_syms), 4));
    wi32(ptr, nb_buckets);
    wi32(add(ptr, 4), nb_syms);
    ptr = add(ptr, 8);
    hash = ptr;
    memset(hash, 0, mul(add(nb_buckets, 1), 4));
    ptr = add(ptr, mul(add(nb_buckets, 1), 4));

    sym = add(gs_data(s), sizeof_Elf32_Sym);
    sym_index = 1;
    while(lt(sym_index, nb_syms)) {
        if (neq(ELFW_ST_BIND(ges_st_info(sym)), STB_LOCAL)) {
            h = mod(elf_hash(add(strtab, ges_st_name(sym))), nb_buckets);
            wi32(ptr, ri32(add(hash, mul(h, 4))));
            wi32(add(hash, mul(h, 4)), sym_index);
        } else {
            wi32(ptr, 0);
        }
        ptr = add(ptr, 4);
        sym = add(sym, sizeof_Elf32_Sym);
        sym_index = add(sym_index, 1);
    }
}

/* return the symbol number */
int put_elf_sym(int s, int value, int size,
    int info, int other, int shndx, int name) {
    int name_offset;
    int sym_index;
    int nbuckets;
    int h;
    int sym;
    int hs;
    int ptr;
    int base;

    sym = section_ptr_add(s, sizeof_Elf32_Sym);
    if (name) {
        if(ri8(name)) {
            name_offset = put_elf_str(gs_link(s), name);
        }
    } else {
        name_offset = 0;
    }
    /* XXX: endianness */
    ses_st_name(sym, name_offset);
    ses_st_value(sym, value);
    ses_st_size(sym, size);
    ses_st_info(sym, info);
    ses_st_other(sym, other);
    ses_st_shndx(sym, shndx);
    sym_index = div_(sub(sym, gs_data(s)), sizeof_Elf32_Sym);
    hs = gs_hash(s);
    if (hs) {
        ptr = section_ptr_add(hs, 4);
        base = gs_data(hs);
        /* only add global or weak symbols. */
        if (neq(ELFW_ST_BIND(info), STB_LOCAL)) {
            /* add another hashing entry */
            nbuckets = ri32(base);
            h = mod(elf_hash(add(gs_data(gs_link(s)), name_offset)),
                    nbuckets);
            wi32(ptr, ri32(add(base,mul(add(2, h), 4))));
            wi32(add(base,mul(add(2, h), 4)), sym_index);
            wi32(add(base, 4), add(ri32(add(base, 4)), 1));
            /* we resize the hash table */
            ss_nb_hashed_syms(hs, add(gs_nb_hashed_syms(hs), 1));
            if (gt(gs_nb_hashed_syms(hs), mul(2, nbuckets))) {
                rebuild_hash(s, mul(2, nbuckets));
            }
        } else {
            wi32(ptr, 0);
            wi32(add(base, 4),add(ri32(add(base, 4)), 1));
        }
    }
    return sym_index;
}

int find_elf_sym(int s, int name) {
    int sym;
    int hs;
    int nbuckets;
    int sym_index;
    int h;
    int name1;

    hs = gs_hash(s);
    if (eq(hs, 0)) {
        return 0;
    }
    nbuckets = ri32(gs_data(hs));
    h = mod(elf_hash(name), nbuckets);
    sym_index = ri32(add(gs_data(hs), mul(add(2, h), 4)));
    while (neq(sym_index, 0)) {
        sym = add(gs_data(s), mul(sizeof_Elf32_Sym, sym_index));
        name1 = add(gs_data(gs_link(s)), ges_st_name(sym));
        if (eq(strcmp(name, name1), 0)) {
            return sym_index;
        }
        sym_index = ri32(add(gs_data(hs), mul(add(add(2, nbuckets), sym_index), 4)));
    }
    return 0;
}

/* add an elf symbol : check if it is already defined and patch
   it. Return symbol index. NOTE that sh_num can be SHN_UNDEF. */
int set_elf_sym(int s, int value, int size,
                       int info, int other, int shndx, int name) {
    int sym_bind;
    int sym_index;
    int sym_type;
    int esym_bind;
    int sym_vis;
    int esym_vis;
    int new_vis;

    sym_bind = ELFW_ST_BIND(info);
    sym_type = ELFW_ST_TYPE(info);
    sym_vis = ELFW_ST_VISIBILITY(other);

    if (neq(sym_bind, STB_LOCAL)) {
        /* we search global or weak symbols */
        sym_index = find_elf_sym(s, name);
    }
    sym_index = put_elf_sym(s, value, size,
                                ELFW_ST_INFO(sym_bind, sym_type), other,
                                shndx, name);
    return sym_index;
}

/* put relocation */
int put_elf_reloca(int symtab, int s, int offset,
                            int type, int symbol, int addend) {
    int buf;
    int buf_size;
    int sr;
    int rel;

    buf_size=256;
    buf = tcc_mallocz(buf_size);

    sr = gs_reloc(s);
    if (eq(sr, 0)) {
        /* if no relocation section, create it */
        snprintf(buf, buf_size, REL_SECTION_FMT, gs_name(s));
        /* if the symtab is allocated, then we consider the relocation
           are also */
        sr = new_section(tcc_state, buf, SHT_RELX, gs_sh_flags(symtab));
        ss_sh_entsize(sr, sizeof_Elf32_Rel);
        ss_link(sr, symtab);
        ss_sh_info(sr, gs_sh_num(s));
        ss_reloc(s, sr);
    }
    rel = section_ptr_add(sr, sizeof_Elf32_Rel);
    ser_r_offset(rel, offset);
    ser_r_info(rel, ELFW_R_INFO(symbol, type));

    tcc_free(buf);
}

/* In an ELF file symbol table, the local symbols must appear below
   the global and weak ones. Since TCC cannot sort it while generating
   the code, we must do it after. All the relocation tables are also
   modified to take into account the symbol table sorting */
int sort_syms(int s1, int s) {
    int old_to_new_syms;
    int new_syms;
    int nb_syms;
    int i;
    int p;
    int q;
    int rel;
    int sr;
    int type;
    int sym_index;

    nb_syms = div_(gs_data_offset(s), sizeof_Elf32_Sym);
    new_syms = tcc_malloc(mul(nb_syms, sizeof_Elf32_Sym));
    old_to_new_syms = tcc_malloc(mul(nb_syms, 4));

    /* first pass for local symbols */
    p = gs_data(s);
    q = new_syms;
    i = 0;
    while(lt(i, nb_syms)) {
        if (eq(ELFW_ST_BIND(ges_st_info(p)), STB_LOCAL)) {
            wi32(add(old_to_new_syms, mul(i, 4)), div_(sub(q, new_syms), sizeof_Elf32_Sym));
            memcpy(q, p, sizeof_Elf32_Sym);
            q = add(q, sizeof_Elf32_Sym);
        }
        p = add(p, sizeof_Elf32_Sym);
        i = add(i, 1);
    }
    /* save the number of local symbols in section header */
    if(gs_sh_size(s)) {   /* this 'if' makes IDA happy */
        ss_sh_info(s, div_(sub(q, new_syms), sizeof_Elf32_Sym));
    }

    /* then second pass for non local symbols */
    p = gs_data(s);
    i = 0;
    while(lt(i, nb_syms)) {
        if (neq(ELFW_ST_BIND(ges_st_info(p)), STB_LOCAL)) {
            wi32(add(old_to_new_syms, mul(i, 4)), div_(sub(q, new_syms), sizeof_Elf32_Sym));
            memcpy(q, p, sizeof_Elf32_Sym);
            q = add(q, sizeof_Elf32_Sym);
        }
        p = add(p, sizeof_Elf32_Sym);
        i = add(i, 1);
    }

    /* we copy the new symbols to the old */
    memcpy(gs_data(s), new_syms, mul(nb_syms, sizeof_Elf32_Sym));
    tcc_free(new_syms);

    /* now we modify all the relocations */
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        sr = ri32(add(gts_sections(s1), mul(i, 4)));
        if (and(eq(gs_sh_type(sr), SHT_RELX), eq(gs_link(sr), s))) {
            rel = gs_data(sr);
            while(lt(rel, add(gs_data(sr), gs_data_offset(sr)))) {
                sym_index = ELFW_R_SYM(ger_r_info(rel));
                type = ELFW_R_TYPE(ger_r_info(rel));
                sym_index = ri32(add(old_to_new_syms, mul(sym_index, 4)));
                ser_r_info(rel, ELFW_R_INFO(sym_index, type));
                rel = add(rel, sizeof_Elf32_Rel);
            }
        }
        i = add(i, 1);
    }

    tcc_free(old_to_new_syms);
}

/* Allocate strings for section names and decide if an unallocated section
   should be output.
   NOTE: the strsec section comes last, so its size is also correct ! */
int alloc_sec_names(int s1, int strsec) {
    int i;
    int s;

    /* Allocate strings for section names */
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        s = ri32(add((gts_sections(s1)), (mul(i, 4))));
        ss_sh_size(s, gs_data_offset(s));
        if (or(gs_sh_size(s), and(gs_sh_flags(s), SHF_ALLOC))) {
            ss_sh_name(s, put_elf_str(strsec, gs_name(s)));
        }
        i = add(i, 1);
    }
    ss_sh_size(strsec, gs_data_offset(strsec));
    return 0;
}

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

/* Create an ELF file on disk.
   This function handle ELF specific layout requirements */
int tcc_output_elf(int s1, int f, int phnum, int phdr,
                           int file_offset, int sec_order) {
    int i;
    int shnum;
    int offset;
    int size;
    int s;
    int ehdr;
    int shdr;
    int sh;

    enter();
    ehdr = v_alloca(sizeof_Elf32_Ehdr);
    shdr = v_alloca(sizeof_Elf32_Shdr);

    shnum = gts_nb_sections(s1);

    memset(ehdr, 0, sizeof_Elf32_Ehdr);

    /* align to 4 */
    file_offset = and(add(file_offset, 3), sub(0, 4));

    /* fill header */
    wi8(gee_e_ident(ehdr), 127); /* ELFMAG0 0x7f Magic number byte 0 */
    wi8(add(gee_e_ident(ehdr), 1), mkc('E'));
    wi8(add(gee_e_ident(ehdr), 2), mkc('L'));
    wi8(add(gee_e_ident(ehdr), 3), mkc('F'));
    wi8(add(gee_e_ident(ehdr), 4), 1); /* ELFCLASS32 1 32-bit objects */
    wi8(add(gee_e_ident(ehdr), 5), 1); /* ELFDATA2LSB 1 2's complement, little endian */
    wi8(add(gee_e_ident(ehdr), 6), 1); /* EV_CURRENT Current version */

    see_e_type(ehdr, 1); /* ET_REL 1 Relocatable file */
    see_e_machine(ehdr, 3); /* EM_386 3 Intel 80386 */
    see_e_version(ehdr, 1); /* EV_CURRENT Current version */
    see_e_shoff(ehdr, file_offset);
    see_e_ehsize(ehdr, sizeof_Elf32_Ehdr);
    see_e_shentsize(ehdr, sizeof_Elf32_Shdr);
    see_e_shnum(ehdr, shnum);
    see_e_shstrndx(ehdr, sub(shnum, 1));

    fwrite(ehdr, 1, sizeof_Elf32_Ehdr, f);
    fwrite(phdr, 1, mul(phnum, sizeof_Elf32_Phdr), f);
    offset = add(sizeof_Elf32_Ehdr, mul(phnum, sizeof_Elf32_Phdr));

    sort_syms(s1, symtab_section);
    i = 1;
    while(lt(i, gts_nb_sections(s1))) {
        s = ri32(add(gts_sections(s1), mul(ri32(add(sec_order, mul(i, 4))),
                                           4)));
        if (neq(gs_sh_type(s), SHT_NOBITS)) {
            while (lt(offset, gs_sh_offset(s))) {
                fputc(0, f);
                offset = add(offset, 1);
            }
            size = gs_sh_size(s);
            if (size) {
                fwrite(gs_data(s), 1, size, f);
            }
            offset = add(offset, size);
        }
        i = add(i, 1);
    }

    /* output section headers */
    while (lt(offset, gee_e_shoff(ehdr))) {
        fputc(0, f);
        offset = add(offset, 1);
    }

    i = 0;
    while(lt(i, gts_nb_sections(s1))) {
        sh = shdr;
        memset(sh, 0, sizeof_Elf32_Shdr);
        s = ri32(add(gts_sections(s1),mul(i, 4)));
        if (s) {
            sesh_sh_name(sh, gs_sh_name(s));
            sesh_sh_type(sh, gs_sh_type(s));
            sesh_sh_flags(sh, gs_sh_flags(s));
            sesh_sh_entsize(sh, gs_sh_entsize(s));
            sesh_sh_info(sh, gs_sh_info(s));
            if (gs_link(s)) {
                sesh_sh_link(sh, gs_sh_num(gs_link(s)));
            }
            sesh_sh_addralign(sh, gs_sh_addralign(s));
            sesh_sh_addr(sh, gs_sh_addr(s));
            sesh_sh_offset(sh, gs_sh_offset(s));
            sesh_sh_size(sh, gs_sh_size(s));
        }
        fwrite(sh, 1, sizeof_Elf32_Shdr, f);
        i = add(i, 1);
    }

    return leave(0);
}

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

int tcc_output_file(int s, int filename) {
    int ret;
    ret = elf_output_file(s, filename);
    return ret;
}

/* end of tccelf.c */

/* start of tccpp.c */

int skip(int c) {
    next();
}

/* ------------------------------------------------------------------------- */

/* CString handling */
int cstr_realloc(int cstr, int new_size) {
    int size;

    size = gcs_size_allocated(cstr);
    if (lt(size, 8)) {
        size = 8; /* no need to allocate a too small first string */
    }
    while (lt(size, new_size)) {
        size = mul(size, 2);
    }
    scs_data(cstr, tcc_realloc(gcs_data(cstr), size));
    scs_size_allocated(cstr, size);
}

/* add a byte */
int cstr_ccat(int cstr, int ch) {
    int size;
    size = add(gcs_size(cstr), 1);
    if (gt(size, gcs_size_allocated(cstr))) {
        cstr_realloc(cstr, size);
    }
    wi32(add(gcs_data(cstr), sub(size, 1)), ch);
    scs_size(cstr, size);
}

int cstr_cat(int cstr, int str, int len) {
    int size;
    if (lte(len, 0)) {
        len = add(add(strlen(str), 1), len);
    }
    size = add(gcs_size(cstr), len);
    if (gt(size, gcs_size_allocated(cstr))) {
        cstr_realloc(cstr, size);
    }
    memmove(add(gcs_data(cstr), gcs_size(cstr)), str, len);
    scs_size(cstr, size);
}

int cstr_new(int cstr) {
    memset(cstr, 0, sizeof_CString);
}

/* free string and reset it to NULL */
int cstr_free(int cstr) {
    tcc_free(gcs_data(cstr));
    cstr_new(cstr);
}

/* reset string to empty */
int cstr_reset(int cstr) {
    scs_size(cstr, 0);
}

/* allocate a new token */
int tok_alloc_new(int pts, int str, int len) {
    int ts;
    int ptable;
    int i;

    /* expand token table if needed */
    i = sub(tok_ident, TOK_IDENT);
    if (eq((mod(i, TOK_ALLOC_INCR)), 0)) {
        ptable = tcc_realloc(table_ident, mul(add(i, TOK_ALLOC_INCR), 4));
        table_ident = ptable;
    }

    ts = tcc_realloc(0, add(sizeof_TokenSym, len));
    wi32(add(table_ident, mul(i, 4)), ts);
    stks_tok(ts, tok_ident);
    tok_ident = add(tok_ident, 1);
    stks_sym_identifier(ts, 0);
    stks_len(ts, len);
    stks_hash_next(ts, 0);
    memcpy(gtks_str(ts), str, len);
    wi8(add(gtks_str(ts), len), 0);
    wi32(pts, ts);
    return ts;
}

/* ------------------------------------------------------------------------- */

int TOK_HASH_FUNC(int h, int c) {
  return add(add(h, shl(h, 5)), add(and(shr(h, 27), 31), c));
}

/* find a token and add it if not found */
int tok_alloc(int str, int len) {
    int ts;
    int pts;
    int i;
    int h;

    h = 1; /* TOK_HASH_INIT */
    i = 0;
    while(lt(i, len)) {
        h = TOK_HASH_FUNC(h, (ri8(add(str, i))));
        i = add(i, 1);
    }
    h = and(h, sub(aTOK_HASH_SIZE, 1));

    pts = add(ahash_ident, mul(h, 4));
    while(1) {
        ts = ri32(pts);
        if (eq(ts, 0)) {
            break;
        }
        if (and(eq(gtks_len(ts), len), eq(memcmp(gtks_str(ts), str, len),0))) {
            return ts;
        }
        pts = atks_hash_next(ts);
    }
    return tok_alloc_new(pts, str, len);
}

int get_tok_str(int v, int cv) {
    int p;
    int i;
    int len;

    cstr_reset(acstr_buf);
    p = gcs_data(acstr_buf);

    if (lt(v, tok_ident)) {
        return gtks_str(ri32(add(table_ident, mul(sub(v, TOK_IDENT), 4))));
    } else if (gte(v, SYM_FIRST_ANOM)) {
        /* special name for anonymous symbol */
        sprintf(p, mks("L.%u"), sub(v, SYM_FIRST_ANOM));
    }
    return gcs_data(acstr_buf);
}

/* return the current character, handling end of block if necessary
   (but not stray) */
int handle_eob() {
    int bf;
    int len;

    bf = file;

    /* only tries to read if really end of buffer */
    if (gte(gbf_buf_ptr(bf), gbf_buf_end(bf))) {
        if (gte(gbf_fd(bf), 0)) {
            len = IO_BUF_SIZE;
            len = read(gbf_fd(bf), gbf_buffer(bf), len);
            if (lt(len, 0)) {
                len = 0;
            }
        } else {
            len = 0;
        }
        sbf_buf_ptr(bf, gbf_buffer(bf));
        sbf_buf_end(bf, add(gbf_buffer(bf), len));
        wi8(gbf_buf_end(bf), CH_EOB);
    }
    if (lt(gbf_buf_ptr(bf), gbf_buf_end(bf))) {
        return ri8(gbf_buf_ptr(bf));
    } else {
        sbf_buf_ptr(bf, gbf_buf_end(bf));
        return CH_EOF_;
    }
}

/* skip the stray and handle the \\n case. Output an error if
   incorrect char after the stray */
int handle_stray1(int p) {
    int c;

    sbf_buf_ptr(file, p);
    if (gte(p, gbf_buf_end(file))) {
        c = handle_eob();
        if (neq(c, mkc('\\'))) {
            return c;
        }
    }
}

/* handle just the EOB case, but not stray */
int PEEKC_EOB(int c1, int p1) {
    int c;
    int p;

    p = add(ri32(p1), 1);
    c = ri8(p);
    if (eq(c, mkc('\\'))) {
        sbf_buf_ptr(file, p);
        c = handle_eob();
        p = gbf_buf_ptr(file);
    }
    wi8(c1,c);
    wi32(p1,p);
}

/* handle the complicated stray case */
int PEEKC(int c1, int p1) {
    int c;
    int p;

    p = add(ri32(p1), 1);
    c = ri8(p);
    if (eq(c, mkc('\\'))) {
        c = handle_stray1(p);
        p = gbf_buf_ptr(file);
    }
    wi8(c1,c);
    wi32(p1,p);
}

/* C comments */
int parse_comment(int p) {
    int c;

    p = add(p, 1);
    while(1) {
        /* fast skip loop */
        while(1) {
            c = ri8(p);
            if (or(or(eq(c, mkc('\n')), eq(c, mkc('*'))), eq(c, mkc('\\')))) {
                break;
            }
            p = add(p, 1);
            c = ri8(p);
            if (or(or(eq(c, mkc('\n')), eq(c, mkc('*'))), eq(c, mkc('\\')))) {
                break;
            }
            p = add(p, 1);
        }
        /* now we can handle all the cases */
        if (eq(c, mkc('\n'))) {
            sbf_line_num(file, add(gbf_line_num(file), 1));
            p = add(p, 1);
        } else if (eq(c, mkc('*'))) {
            p = add(p, 1);
            while(1) {
                c = ri8(p);
                if (eq(c, mkc('*'))) {
                    p = add(p, 1);
                } else if (eq(c, mkc('/'))) {
                    p = add(p, 1);
                    return p;
                } else {
                    break;
                }
            }
        } else {
            /* stray, eob or eof */
            sbf_buf_ptr(file, p);
            c = handle_eob();
            p = gbf_buf_ptr(file);
            if (eq(c, mkc('\\'))) {
                p = add(p, 1);
            }
        }
    }
    p = add(p, 1);
    return p;
}

int set_idnum(int c, int val) {
    int prev;
    prev = ri8(add(aisidnum_table, sub(c, CH_EOF_)));
    wi8(add(aisidnum_table, sub(c, CH_EOF_)), val);
    return prev;
}

/* parse a string without interpreting escapes */
int parse_pp_string(int p, int sep, int str) {
    int c;
    int tc;
    int tp;

    enter();
    tc = v_alloca(4);
    tp = v_alloca(4);

    p = add(p, 1);
    while(1) {
        c = ri8(p);
        if (eq(c, sep)) {
            break;
        } else if (eq(c, mkc('\\'))) {
            sbf_buf_ptr(file, p);
            c = handle_eob();
            p = gbf_buf_ptr(file);
            if (eq(c, mkc('\\'))) {
                /* escape : just skip \[\r]\n */
                /* LJW HACK r and w function to avoid moving c and p to stack */
                wi8(tc, c); wi32(tp, p);
                PEEKC_EOB(tc, tp);
                c = ri32(tc); p = ri32(tp);
                if (str) {
                    cstr_ccat(str, mkc('\\'));
                    cstr_ccat(str, c);
                }
                p = add(p, 1);
            }
        } else {
            if (str) {
                cstr_ccat(str, c);
            }
            p = add(p, 1);
        }
    }
    p = add(p, 1);
    return leave(p);
}

/* token string handling */
int tok_str_new(int s) {
    stkst_str(s, 0);
    stkst_len(s, 0);
    stkst_lastlen(s, 0);
    stkst_allocated_len(s, 0);
}

int tok_str_alloc() {
    int str;
    str = tcc_realloc(0, sizeof_TokenString);
    tok_str_new(str);
    return str;
}

int tok_str_free_str(int str) {
    tcc_free(str);
}

int tok_str_free(int str) {
    tok_str_free_str(gtkst_str(str));
    tcc_free(str);
}

int tok_str_realloc(int s, int new_size) {
    int str;
    int size;

    size = gtkst_allocated_len(s);
    if (lt(size, 16)) {
        size = 16;
    }
    while (lt(size, new_size)) {
        size = mul(size, 2);
    }
    if (gt(size, gtkst_allocated_len(s))) {
        str = tcc_realloc(gtkst_str(s), mul(size, 4));
        stkst_allocated_len(s, size);
        stkst_str(s, str);
    }
    return gtkst_str(s);
}

int tok_str_add(int s, int t) {
    int len;
    int str;

    len = gtkst_len(s);
    str = gtkst_str(s);
    if (gte(len, gtkst_allocated_len(s))) {
        str = tok_str_realloc(s, add(len, 1));
    }
    wi32(add(str, mul(len, 4)),t);
    len = add(len, 1);
    stkst_len(s, len);
}

int begin_macro(int str, int alloc) {
    stkst_alloc(str, alloc);
    stkst_prev(str, macro_stack);
    stkst_prev_ptr(str, macro_ptr);
    macro_ptr = gtkst_str(str);
    macro_stack = str;
}

int end_macro() {
    int str;
    str = macro_stack;
    macro_stack = gtkst_prev(str);
    macro_ptr = gtkst_prev_ptr(str);
    tok_str_free(str);
}

int tok_str_add2(int s, int t, int cv) {
    int len;
    int str;
    int nb_words;

    stkst_lastlen(s, gtkst_len(s));
    len = gtkst_len(s);
    str = gtkst_str(s);

    /* allocate space for worst case */
    if (gte(add(len, TOK_MAX_SIZE), gtkst_allocated_len(s))) {
        str = tok_str_realloc(s, add(add(len, TOK_MAX_SIZE), 1));
    }
    wi32(add(str, mul(len, 4)), t);
    len = add(len, 1);
    if(eq(t, TOK_STR)) {
        /* Insert the string into the int array. */
        nb_words = add(1, div_(sub(add(gcv_str_size(cv), 4) ,1), 4));
        if (gte(add(len, nb_words), gtkst_allocated_len(s))) {
            str = tok_str_realloc(s, add(add(len, nb_words), 1));
        }
        wi32(add(str, mul(len, 4)), gcv_str_size(cv));
        memcpy(add(str, mul(add(len, 1), 4)), gcv_str_data(cv), gcv_str_size(cv));
        len = add(len, nb_words);
    }
    stkst_len(s, len);
}

/* add the current parse token in token string 's' */
int tok_str_add_tok(int s) {
    tok_str_add2(s, tok, atokc);
}

/* get a token from an integer array and increment pointer
   accordingly. we code it as a macro to avoid pointer aliasing. */
int TOK_GET(int t, int pp, int cv) {
    int n;
    int p;
    p = ri32(pp);

    wi32(t, ri32(p));
    p = add(p, 4);
    if(or(or(eq(t, TOK_STR), eq(t, TOK_PPNUM)), eq(t, TOK_PPSTR))) {
        scv_str_size(cv, ri32(p));
        p = add(p, 4);
        wi32(gcv_str_data(cv), p);
        p = add(p, mul(div_(sub(add(gcv_str_size(cv), sizeof_int), 1), sizeof_int), 4));
    }

    wi32(pp, p);
}

/* evaluate escape codes in a string. */
int parse_escape_string(int outstr, int buf, int is_long) {
    int c;
    int n;
    int p;

    p = buf;
    while(1) {
        c = ri8(p);
        if (eq(c, 0)) {
            break;
        }
        if (eq(c, mkc('\\'))) {
            p = add(p, 1);
            /* escape */
            c = ri8(p);
            if(eq(c, mkc('n'))) {
                c = mkc('\n');
            } else if(eq(c, mkc('r'))) {
                c = mkc('\r');
            } else if(eq(c, mkc('t'))) {
                c = mkc('\t');
            }
        }
        p = add(p, 1);
        cstr_ccat(outstr, c);
    }
    /* add a trailing '\0' */
    cstr_ccat(outstr, 0);
}

int parse_string(int s, int len) {
    int p;
    int is_long;
    int sep;
    int c;

    is_long=0;
    sep = ri8(s);
    s = add(s, 1);
    len = sub(len, 2);
    p = tcc_malloc(add(len, 1));
    memcpy(p, s, len);
    wi8(add(p, len), 0);

    cstr_reset(atokcstr);
    parse_escape_string(atokcstr, p, is_long);
    tcc_free(p);

    if (eq(sep, mkc('\''))) {
        tok = TOK_CCHAR;
        c = ri8(gcs_data(atokcstr));
        scv_i(atokc, c);
    } else {
        scv_str_size(atokc, gcs_size(atokcstr));
        scv_str_data(atokc, gcs_data(atokcstr));
        tok = TOK_STR;
    }
}

/* parse number in null terminated string 'p' and return it in the
   current token */
int parse_number(int p) {
    int b;
    int t;
    int s;
    int ch;
    int q;
    int n;
    int n1;

    /* number */
    q = atoken_buf;
    ch = ri8(p);
    p = add(p, 1);
    t = ch;
    ch = ri8(p);
    p = add(p, 1);
    wi8(q, t);
    q = add(q, 1);
    b = 10;
    if (eq(t, mkc('0'))) {
        if (or(eq(ch, mkc('x')), eq(ch, mkc('X')))) {
            q = sub(q, 1);
            ch = ri8(p);
            p = add(p, 1);
            b = 16;
        }
    }
    /* parse all digits. cannot check octal numbers at this stage
       because of floating point constants */
    while (1) {
        if (and(gte(ch, mkc('A')), lte(ch, mkc( 'F')))) {
            t = add(sub(ch, mkc('A')), 10);
        } else if (isnum(ch)) {
            t = sub(ch, mkc('0'));
        } else {
            break;
        }
        wi8(q, ch);
        q = add(q, 1);
        ch = ri8(p);
        p = add(p, 1);
    }

    /* integer number */
    wi8(q, 0);
    q = atoken_buf;
    if (and(eq(b, 10), (eq(ri8(q), mkc('0'))))) {
        b = 8;
        q = add(q, 1);
    }
    n = 0;
    while(1) {
        t = ri8(q);
        q = add(q, 1);
        /* no need for checks except for base 10 / 8 errors */
        if (eq(t, 0)) {
            break;
        } else if (gte(t, mkc('A'))) {
            t = add(sub(t, mkc('A')), 10);
        } else {
            t = sub(t, mkc('0'));
        }
        n1 = n;
        n = add(mul(n, b), t);
    }

    tok = TOK_CINT;
    tok = add(tok, 1); /* TOK_CU... */
    scv_i(atokc, n);
}

/* return next token without macro substitution */
int next_nomacro1() {
    int t;
    int c;
    int len;
    int ts;
    int p;
    int p1;
    int h;
    int redo_no_start;
    int pts;

    int tc;
    int tp;

    enter();
    tc = v_alloca(4);
    tp = v_alloca(4);

    p = gbf_buf_ptr(file);
    while(1) {
        redo_no_start = 0;
        c = ri8(p);
        if(or(eq(c, mkc(' ')), (eq(c, mkc('\t'))))) {
            tok = c;
            p = add(p, 1);
            while (and(ri8(add(aisidnum_table, sub(ri8(p), CH_EOF_))), IS_SPC)) {
                p = add(p, 1);
            }
            redo_no_start = 1;
        } else if(eq(c, mkc('\\'))){
            /* first look if it is in fact an end of buffer */
            c = handle_stray1(p);
            p = gbf_buf_ptr(file);
            if (neq(c, CH_EOF_)) {
                redo_no_start = 1;
            } else {
                tok = TOK_EOF;
            }
        } else if(eq(c, mkc('\n'))){
            sbf_line_num(file, add(gbf_line_num(file), 1));
            tok_flags = or(tok_flags, TOK_FLAG_BOL);
            p = add(p, 1);
            if (eq(0, and(parse_flags, PARSE_FLAG_LINEFEED))) {
                redo_no_start = 1;
            }
        }
        if(eq(redo_no_start, 0)) {
            break;
        }
    }

    c = ri8(p);
    if(isid(c)) {
        p1 = p;
        h = 1; /* TOK_HASH_INIT */
        h = TOK_HASH_FUNC(h, c);
        while (1) {
            p = add(p, 1);
            c = ri8(p);
            if(eq(p, 0)) {
                break;
            } else {
                if(eq(0, and(ri8(add(aisidnum_table, sub(c, CH_EOF_))), or(IS_ID,IS_NUM)))) {
                    break;
                }
            }
            h = TOK_HASH_FUNC(h, c);
        }
        len = sub(p, p1);
        if (neq(c, mkc('\\'))) {

            /* fast case : no stray found, so we have the full token
               and we have already hashed it */
            h = and(h, sub(aTOK_HASH_SIZE, 1));
            pts = add(ahash_ident, mul(h, 4));
            int token_found;
            while(1) {
                token_found = 0;
                ts = ri32(pts);
                if (eq(ts, 0)) {
                    break;
                }
                if (eq(gtks_len(ts), len)) {
                    if(eq(0, memcmp(gtks_str(ts), p1, len))) {
                        token_found = 1;
                        break;
                    }
                }
                pts = atks_hash_next(ts);
            }
            if(eq(token_found, 0)) {
                ts = tok_alloc_new(pts, p1, len);
            }
        } else {
            /* slower case */
            cstr_reset(atokcstr);
            cstr_cat(atokcstr, p1, len);
            p = sub(p, 1);
            wi8(tc, c); wi32(tp, p); /* LJW hack to avoid using & */
            PEEKC(tc, tp);
            c = ri32(tc); p = ri32(tp);
            while (and(ri8(add(aisidnum_table, sub(c, CH_EOF_))), or(IS_ID, IS_NUM))) {
                cstr_ccat(atokcstr, c);
                wi8(tc, c); wi32(tp, p); /* LJW hack to avoid using & */
                PEEKC(tc, tp);
                c = ri32(tc); p = ri32(tp);
            }
            ts = tok_alloc(gcs_data(atokcstr), gcs_size(atokcstr));
        }
        tok = gtks_tok(ts);
    } else if(isnum(c)){
        t = c;
        wi8(tc, c); wi32(tp, p); /* LJW hack to avoid using & */
        PEEKC(tc, tp);
        c = ri32(tc); p = ri32(tp);
        /* after the first digit, accept digits, alpha, '.' or sign if
           prefixed by 'eEpP' */
        cstr_reset(atokcstr);
        while(1) {
            cstr_ccat(atokcstr, t);
            if (eq(0, or(and(ri8(add(aisidnum_table, sub(c, CH_EOF_))), or(IS_ID, IS_NUM)),
                  or(eq(c, mkc('.')),
                  or(eq(c, mkc('+')), eq(c, mkc('-'))))))) {
                break;
            }
            t = c;
            wi8(tc, c); wi32(tp, p); /* LJW hack to avoid using & */
            PEEKC(tc, tp);
            c = ri32(tc); p = ri32(tp);
        }
        /* We add a trailing '\0' to ease parsing */
        cstr_ccat(atokcstr, 0);
        scv_str_size(atokc, gcs_size(atokcstr));
        scv_str_data(atokc, gcs_data(atokcstr));
        tok = TOK_PPNUM;
    } else if(or(eq(c, mkc('\'')), eq(c, mkc('\"')))){
        cstr_reset(atokcstr);
        cstr_ccat(atokcstr, c);
        p = parse_pp_string(p, c, atokcstr);
        cstr_ccat(atokcstr, c);
        cstr_ccat(atokcstr, 0);
        scv_str_size(atokc, gcs_size(atokcstr));
        scv_str_data(atokc, gcs_data(atokcstr));
        tok = TOK_PPSTR;
    } else if(eq(c, mkc('='))){
        wi8(tc, c); wi32(tp, p); /* LJW hack to avoid using & */
        PEEKC(tc, tp);
        c = ri32(tc); p = ri32(tp);
        tok = mkc('=');
    } else if(eq(c, mkc('/'))){
        wi8(tc, c); wi32(tp, p); /* LJW hack to avoid using & */
        PEEKC(tc, tp);
        c = ri32(tc); p = ri32(tp);
        if (eq(c, mkc('*'))) {
            p = parse_comment(p);
            /* comments replaced by a blank */
            tok = mkc(' ');
            sbf_buf_ptr(file, p);
            return leave(0);
        }
    } else if(or(or(eq(c, mkc('(')), eq(c, mkc(')'))),
              or(or(eq(c, mkc('{')), eq(c, mkc('}'))),
              or(eq(c, mkc(',')), eq(c, mkc(';')))))){
        tok = c;
        p = add(p, 1);
    }
    tok_flags = 0;
    sbf_buf_ptr(file, p);

    return leave(0);
}

/* return next token without macro substitution. Can read input from
   macro_ptr buffer */
int next_nomacro_spc() {
    int ttok;
    int tmacro_ptr;
    enter();

    ttok = v_alloca(4);
    tmacro_ptr = v_alloca(4);

    if (macro_ptr) {
        tok = ri32(macro_ptr);
        if (tok) {
            wi32(ttok, tok);
            wi32(tmacro_ptr, macro_ptr);
            TOK_GET(ttok, tmacro_ptr, atokc);
            tok = ri32(ttok);
            macro_ptr = ri32(tmacro_ptr);
        }
    } else {
        next_nomacro1();
    }

    return leave(0);
}

/* LJW FIXME why was the re-write so complex */
int next_nomacro() {
    int t;
    int t2;
     while(1) {
         next_nomacro_spc();
        t = neq(0, lt(tok, 256));
        if(t) {
            t2 = neq(0, and(ri8(add(aisidnum_table, sub(tok, CH_EOF_))), IS_SPC));
        } else {
            t2 = 0;
        }
        if(eq(0, and(t, t2))) {
             break;
         }
     }
}

/* return next token with macro substitution */
int next() {
    int redo;
    while(1) {
        redo = 0;
        if (and(parse_flags, PARSE_FLAG_SPACES)) {
            next_nomacro_spc();
        } else {
            next_nomacro();
        }

        if (macro_ptr) {
            if (eq(tok, 0)) {
                /* end of macro or unget token string */
                end_macro();
                redo = 1;
            }
        }
        if(eq(redo, 0)) {
            break;
        }
    }
    /* convert preprocessor tokens into C tokens */
    if (eq(tok, TOK_PPNUM)) {
        if(and(parse_flags, PARSE_FLAG_TOK_NUM)) {
            parse_number(gcv_str_data(atokc));
        }
    } else if(eq(tok, TOK_PPSTR)) {
        if (and(parse_flags, PARSE_FLAG_TOK_STR)) {
            parse_string(gcv_str_data(atokc), sub(gcv_str_size(atokc), 1));
        }
    }
}

/* push back current token and set current token to 'last_tok'. Only
   identifier case handled for labels. */
int unget_tok(int last_tok) {
    int str;
    str = tok_str_alloc();
    tok_str_add2(str, tok, atokc);
    tok_str_add(str, 0);
    begin_macro(str, 1);
    tok = last_tok;
}

int preprocess_start(int s1, int is_asm) {
    int acstr;
    int i;

    enter();
    acstr = v_alloca(sizeof_CString);

    vtop = sub(avstack, sizeof_SValue);

    set_idnum(mkc('$'), 0);
    set_idnum(mkc('.'), 0);

    cstr_new(acstr);
    cstr_cat(acstr, mks("\""), sub(0,1));
    cstr_cat(acstr, gbf_filename(file), sub(0,1));
    cstr_cat(acstr, mks("\""), 0);

    cstr_reset(acstr);
    cstr_free(acstr);

    parse_flags = 0;
    tok_flags = or(TOK_FLAG_BOL, TOK_FLAG_BOF);

    return leave(0);
}

int tccpp_new(int s) {
    int i;
    int c;
    int tmp;

    /* might be used in error() before preprocess_start() */

    /* init isid table */
    i = CH_EOF_;
    while(lt(i, 128)) {
        if(is_space(i)) {
            c = IS_SPC;
        } else if(isid(i)) {
            c = IS_ID;
        } else if(isnum(i)) {
            c = IS_NUM;
        } else {
            c = 0;
        }
        set_idnum(i, c);
        i = add(i, 1);
    }

    i = 128;
    while(lt(i, 256)) {
        set_idnum(i, IS_ID);
        i = add(i, 1);
    }

    memset(ahash_ident, 0, mul(aTOK_HASH_SIZE, sizeof_void));
    cstr_new(acstr_buf);
    cstr_realloc(acstr_buf, aSTRING_MAX_SIZE);

    /* define keywords, FIXME improve this */
    tok_ident = TOK_IDENT;
    TOK_INT    = tok_ident; tmp=mks("int");    tok_alloc(tmp, strlen(tmp));
    TOK_IF     = tok_ident; tmp=mks("if");     tok_alloc(tmp, strlen(tmp));
    TOK_ELSE   = tok_ident; tmp=mks("else");   tok_alloc(tmp, strlen(tmp));
    TOK_WHILE  = tok_ident; tmp=mks("while");  tok_alloc(tmp, strlen(tmp));
    TOK_BREAK  = tok_ident; tmp=mks("break");  tok_alloc(tmp, strlen(tmp));
    TOK_RETURN = tok_ident; tmp=mks("return"); tok_alloc(tmp, strlen(tmp));
}

int tccpp_delete(int s) {
    int i;
    int n;

    /* free tokens */
    n = sub(tok_ident, TOK_IDENT);
    i = 0;
    while(lt(i, n)) {
        tcc_free(ri32(add(table_ident, mul(i, 4))));
        i = add(i, 1);
    }
    tcc_free(table_ident);
    table_ident = 0;

    /* free static buffers */
    cstr_free(atokcstr);
    cstr_free(acstr_buf);
}

/* ------------------------------------------------------------------------- */

/* end of tccpp.c */

/* start of tcc.h functions */

/* space excluding newline */
int is_space(int ch) {
    return or(or(eq(ch, mkc(' ')), eq(ch, mkc('\t'))), eq(ch, mkc('\r')));
}

int isid(int c) {
    return or(or(and(gte(c, mkc('a')), lte(c, mkc('z'))),
                 and(gte(c, mkc('A')), lte(c, mkc('Z')))),
              eq(c, mkc('_')));
}

int isnum(int c) {
    return and(gte(c, mkc('0')), lte(c, mkc('9')));
}

int read16le(int p) {
    return ri16(p);
}

int write16le(int p, int x) {
    wi16(p, x);
}

int read32le(int p) {
  return ri32(p);
}

int write32le(int p, int x) {
    wi32(p, x);
}

/* end of tcc.h functions */

/* start of tccgen.c */

/* 1 */
/* ------------------------------------------------------------------------- */
int tccgen_compile(int s1) {
    cur_text_section = 0;
    funcname = mks("");
    anon_sym = SYM_FIRST_ANOM;
    section_sym = 0;

    /* define some often used types */
    sct_t(aint_type, VT_INT);
    sct_t(afunc_old_type, VT_FUNC);
    sct_ref(afunc_old_type, sym_push(SYM_FIELD, aint_type, 0, 0));
    ssym_f_func_type(gct_ref(afunc_old_type), FUNC_OLD);

    /* an elf symbol of type STT_FILE must be put so that STB_LOCAL
       symbols can be safely used */
    put_elf_sym(symtab_section, 0, 0,
                ELFW_ST_INFO(STB_LOCAL, STT_FILE), 0,
                SHN_ABS, gbf_filename(file));

    parse_flags = or(or(PARSE_FLAG_PREPROCESS, PARSE_FLAG_TOK_NUM), PARSE_FLAG_TOK_STR);
    next();
    decl(VT_CONST);
    return 0;
}

/* 2 */
/* ------------------------------------------------------------------------- */
int elfsym(int s) {
  if (eq(s, 0)) {
    if(eq(gsym_c(s), 0)) {
      return 0;
    }
  }
  return add(gs_data(symtab_section), mul(gsym_c(s), sizeof_Elf32_Sym));
}

/* apply storage attributes to Elf symbol */
int update_storage(int sym) {
    int esym;
    int sym_bind;
    int old_sym_bind;

    esym = elfsym(sym);
    if (eq(esym, 0)) {
        return;
    }

    if (and(gct_t(gsym_type(sym)), VT_STATIC)) {
        sym_bind = STB_LOCAL;
    } else {
        sym_bind = STB_GLOBAL;
    }
    old_sym_bind = ELFW_ST_BIND(ges_st_info(esym));
}

/* 3 */
/* ------------------------------------------------------------------------- */
/* update sym->c so that it points to an external symbol in section
   'section' with value 'value' */
int put_extern_sym2(int sym, int sh_num,
                            int value, int size,
                            int can_add_underscore) {
    int sym_type;
    int sym_bind;
    int info;
    int other;
    int t;
    int esym;
    int name;
    int buf1;

    enter();
    buf1 = v_alloca(256);

    if (eq(gsym_c(sym), 0)) {
        name = get_tok_str(gsym_v(sym), 0);
        t = gct_t(gsym_type(sym));
        if (eq(and(t, VT_BTYPE), VT_FUNC)) {
            sym_type = STT_FUNC;
        } else {
            sym_type = STT_OBJECT;
        }
        if (and(t, VT_STATIC)) {
            sym_bind = STB_LOCAL;
        } else {
            sym_bind = STB_GLOBAL;
        }
        other = 0;
        info = ELFW_ST_INFO(sym_bind, sym_type);
        ssym_c(sym, put_elf_sym(symtab_section, value, size, info, other, sh_num, name));
    } else { 
        esym = elfsym(sym);
        ses_st_value(esym, value);
        ses_st_size(esym, size);
        ses_st_shndx(esym, sh_num);
    }
    update_storage(sym);

    return leave(0);
}

/* 4 */
int put_extern_sym(int sym, int section, int value, int size) {
    int sh_num;
    if(section) {
        sh_num = gs_sh_num(section);
    } else {
        sh_num = SHN_UNDEF;
    }
    put_extern_sym2(sym, sh_num, value, size, 1);
}

/* 5 */
/* add a new relocation entry to symbol 'sym' in section 's' */
int greloca(int s, int sym, int offset, int type, int addend) {
    int c;

    c = 0;

    if (sym) {
        if (eq(0, gsym_c(sym))) {
            put_extern_sym(sym, 0, 0, 0);
        }
        c = gsym_c(sym);
    }

    /* now we can add ELF relocation info */
    put_elf_reloca(symtab_section, s, offset, type, c, addend);
}

/* 6 */
int greloc(int s, int sym, int offset, int type) {
    greloca(s, sym, offset, type, 0);
}

/* 7 */
/* ------------------------------------------------------------------------- */
/* symbol allocator */
int __sym_malloc() {
    int sym_pool;
    int sym;
    int last_sym;
    int i;

    sym_pool = tcc_malloc(mul(SYM_POOL_NB, sizeof_Sym));
    dynarray_add(asym_pools, anb_sym_pools, sym_pool);

    last_sym = sym_free_first;
    sym = sym_pool;
    i = 0;
    while(lt(i, SYM_POOL_NB)) {
        ssym_next(sym, last_sym);
        last_sym = sym;
        sym = add(sym, sizeof_Sym);
        i = add(i, 1);
    }
    sym_free_first = last_sym;
    return last_sym;
}

/* 8 */
int sym_malloc() {
    int sym;
    sym = sym_free_first;
    if (eq(sym, 0)) {
        sym = __sym_malloc();
    }
    sym_free_first = gsym_next(sym);
    return sym;
}

/* 9 */
int sym_free(int sym) {
    ssym_next(sym, sym_free_first);
    sym_free_first = sym;
}

/* 10 */
/* push, without hashing */
int sym_push2(int ps, int v, int t, int c) {
    int s;

    s = sym_malloc();
    memset(s, 0, sizeof_Sym);
    ssym_v(s, v);
    sct_t(gsym_type(s), t);
    ssym_c(s, c);
    /* add in stack */
    ssym_prev(s, ri32(ps));
    wi32(ps, s);
    return s;
}

/* 11 */
/* find an identifier */
int sym_find(int v) {
    v = sub(v, TOK_IDENT);
    if (or(lt(v, 0), gte(v, sub(tok_ident, TOK_IDENT)))) {
        return 0;
    }
    return gtks_sym_identifier(ri32(add(table_ident, mul(v, 4))));
}

/* 12 */
/* push a given symbol on the symbol stack */
int sym_push(int v, int type, int r, int c) {
    int s;
    int ps;
    int ts;

    if (ri32(alocal_stack)) {
        ps = alocal_stack;
    } else {
        ps = aglobal_stack;
    }
    s = sym_push2(ps, v, gct_t(type), c);
    sct_ref(gsym_type(s), gct_ref(type));
    ssym_r(s, r);
    /* don't record fields or anonymous symbols */
    /* XXX: simplify */
    if (and(eq(0, and(v, SYM_FIELD)), lt(and(v, not(SYM_STRUCT)), SYM_FIRST_ANOM))) {
        /* record symbol in token array */
        ts = ri32(add(table_ident, mul(sub(and(v, not(SYM_STRUCT)), TOK_IDENT), 4)));
        ps = atks_sym_identifier(ts);
        ssym_prev_tok(s, ri32(ps));
        wi32(ps, s);
        ssym_sym_scope(s, local_scope);
    }
    return s;
}

/* 13 */
/* push a global identifier */
int global_identifier_push(int v, int t, int c) {
    int s;
    int ps;
    s = sym_push2(aglobal_stack, v, t, c);
    /* don't record anonymous symbol */
    if(lt(v, SYM_FIRST_ANOM)) {
        ps = atks_sym_identifier(ri32(add(table_ident, mul(sub(v, TOK_IDENT), 4))));
        /* modify the top most local identifier, so that
           sym_identifier will point to 's' when popped */
        while (neq(ri32(ps), 0)) {
            if(eq(0, gsym_sym_scope(ri32(ps)))) {
                break;
            }
            ps = asym_prev_tok(ri32(ps));
        }
        ssym_prev_tok(s, ri32(ps));
        wi32(ps, s);
    }
    return s;
}

/* 14 */
/* pop symbols until top reaches 'b'.  If KEEP is non-zero don't really
   pop them yet from the list, but do remove them from the token array.  */
int sym_pop(int ptop, int b, int keep) {
    int s;
    int ss;
    int ps;
    int ts;
    int v;

    s = ri32(ptop);
    while(neq(s, b)) {
        ss = gsym_prev(s);
        v = gsym_v(s);
        /* remove symbol in token array */
        /* XXX: simplify */
        if (and(eq(0, and(v, SYM_FIELD)), lt(and(v, not(SYM_STRUCT)), SYM_FIRST_ANOM))) {
            ts = ri32(add(table_ident, mul(sub(and(v, not(SYM_STRUCT)), TOK_IDENT), 4)));
            ps = atks_sym_identifier(ts);
            wi32(ps, gsym_prev_tok(s));
        }
        if (eq(0, keep)) {
            sym_free(s);
        }
        s = ss;
    }
    if (eq(0, keep)) {
        wi32(ptop, b);
    }
}

/* ------------------------------------------------------------------------- */
/* 15 */

int vsetc(int type, int r, int vc) {
    vtop = add(vtop, sizeof_SValue);
    memmove(gsv_type(vtop), type, sizeof_CType);
    ssv_r(vtop, r);
    memmove(gsv_c(vtop), vc, sizeof_CValue);
    ssv_sym(vtop, 0);
}

/* 16 */
int vswap() {
    int tmp;
    enter();
    tmp = v_alloca(sizeof_SValue);
    memmove(tmp, vtop, sizeof_SValue);
    memmove(vtop, sub(vtop, sizeof_SValue), sizeof_SValue);
    memmove(sub(vtop, sizeof_SValue), tmp, sizeof_SValue);
    return leave(0);
}

/* 17 */
/* pop stack value */
int vpop() {
    int v;
    v = and(gsv_r(vtop), VT_VALMASK);
    vtop = sub(vtop, sizeof_SValue);
}

/* 18 */
/* push integer constant */
int vpushi(int v) {
    int cval;
    enter();
    cval = v_alloca(sizeof_CValue);
    scv_i(acv_i(cval), v);
    vsetc(aint_type, VT_CONST, cval);
    return leave(0);
}

/* 19 */
int vset(int type, int r, int v) {
    int cval;
    enter();
    cval = v_alloca(sizeof_CValue);
    scv_i(cval, v);
    vsetc(type, r, cval);
    return leave(0);
}

/* 20 */
/* push a symbol value of TYPE */
int vpushsym(int type, int sym) {
    int cval;
    enter();
    cval = v_alloca(sizeof_CValue);
    scv_i(cval, 0);
    vsetc(type, or(VT_CONST, VT_SYM), cval);
    ssv_sym(vtop, sym);
    return leave(0);
}

/* 21 */
/* Return a static symbol pointing to a section */
int get_sym_ref(int type, int sec, int offset, int size) {
    int v;
    int sym;

    v = anon_sym;
    anon_sym = add(anon_sym, 1);
    sym = global_identifier_push(v, or(gct_t(type), VT_STATIC), 0);
    sct_ref(gsym_type(sym), gct_ref(type));
    ssym_r(sym, or(VT_CONST, VT_SYM));
    put_extern_sym(sym, sec, offset, size);
    return sym;
}

/* 22 */
/* define a new external reference to a symbol 'v' of type 'u' */
int external_global_sym(int v, int type, int r) {
    int s;

    s = sym_find(v);
    if (eq(0, s)) {
        /* push forward reference */
        s = global_identifier_push(v, or(gct_t(type), VT_EXTERN), 0);
        sct_ref(gsym_type(s), gct_ref(type));
        ssym_r(s, or(or(r, VT_CONST), VT_SYM));
    }
    return s;
}

/* 23 */
/* Merge some type attributes.  */
int patch_type(int sym, int type) {
    int static_proto;
    if (eq(0, and(gct_t(type), VT_EXTERN))) {
        sct_t(gsym_type(sym), and(gct_t(gsym_type(sym)), not(VT_EXTERN)));
    }

    if (eq(and(gct_t(gsym_type(sym)), VT_BTYPE), VT_FUNC)) {
        static_proto = and(gct_t(gsym_type(sym)), VT_STATIC);

        if (eq(0, and(gct_t(type), VT_EXTERN))) {
            /* put complete type, use static from prototype */
            sct_t(gsym_type(sym), or(and(gct_t(type), not(VT_STATIC)), static_proto));
            sct_ref(gsym_type(sym), gct_ref(type));
        }
    }
}

/* 24 */
/* Merge some storage attributes.  */
int patch_storage(int sym, int ad, int type) {
    if (type) {
        patch_type(sym, type);
    }
    update_storage(sym);
}

/* 25 */
/* define a new external reference to a symbol 'v' */
int external_sym(int v, int type, int r, int ad) {
    int s;
    s = sym_find(v);
    if (eq(0, s)) {
        /* push forward reference */
        s = sym_push(v, type, or(or(r, VT_CONST), VT_SYM), 0);
        sct_t(gsym_type(s), or(gct_t(gsym_type(s)), VT_EXTERN));
        ssym_sym_scope(s, 0);
    } else {
        patch_storage(s, ad, type);
    }
    return s;
}

/* 26 */
/* save registers up to (vtop - n) stack entry */
int save_regs(int n) {
    int p;
    int p1;

    p = avstack;
    p1 = sub(vtop, mul(n, sizeof_SValue));
    while(lte(p, p1)) {
        save_reg(gsv_r(p));
        p = add(p, sizeof_SValue);
    }
}

/* 27 */
/* save r to the memory stack, and mark it as being free */
int save_reg(int r) {
    save_reg_upstack(r, 0);
}

/* 28 */
/* save r to the memory stack, and mark it as being free,
   if seen up to (vtop - n) stack entry */
int save_reg_upstack(int r, int n) {
    int l;
    int saved;
    int size;
    int align;
    int p;
    int p1;
    int sv;
    int type;

    enter();
    sv = v_alloca(sizeof_SValue);
    align= v_alloca(4);

    r = and(r, VT_VALMASK);
    if (gte(r, VT_CONST)) {
        return leave(0);
    }

    /* modify all stack values */
    saved = 0;
    l = 0;
    p = avstack;
    p1 = sub(vtop, mul(n, sizeof_SValue));
    while(lte(p, p1)) {
        if (eq(and(gsv_r(p), VT_VALMASK), r)) {
            /* must save value on stack if not already done */
            if (eq(0, saved)) {
                r = and(gsv_r(p), VT_VALMASK);
                /* store register in the stack */
                type = gsv_type(p);
                if (and(gsv_r(p), VT_LVAL)) {
                    type = aint_type;
                }
                size = type_size(type, align);
                loc = and(sub(loc, size), sub(0, ri32(align)));
                sct_t(gsv_type(sv), gct_t(type));
                ssv_r(sv, or(VT_LOCAL, VT_LVAL));
                scv_i(gsv_c(sv), loc);
                store(r, sv);

                l = loc;
                saved = 1;
            }
            /* mark that stack entry as being saved on the stack */
            ssv_r(p, or(lvalue_type(gct_t(gsv_type(p))), VT_LOCAL));
            scv_i(gsv_c(p), l);
        }
        p = add(p, sizeof_SValue);
    }

    leave(0);
}

/* 29 */
/* find a free register of class 'rc'. If none, save one register */
int get_reg(int rc) {
    int r;
    int notfound;
    int p;

    /* find a free register */
    r = 0;
    while(lt(r, 5)) { /* NB_REGS is 5 (sort of) */
        notfound = 0;
        if (and(ri32(add(reg_classes, mul(r,4))), rc)) {
            p = avstack;
            while(lte(p, vtop)) {
                if (eq(and(gsv_r(p), VT_VALMASK), r)) {
                    notfound = 1;
                    break;
                }
                p = add(p, sizeof_SValue);
            }
            if(eq(notfound, 0)) {
                return r;
            }
        }
        r = add(r, 1);
    }
}

/* 30 */
/* store vtop a register belonging to class 'rc'. lvalues are
   converted to values. Cannot be used if cannot be converted to
   register value (such as structures). */
int gv(int rc) {
    int r;
    int t1;
    int t;

    r = and(gsv_r(vtop), VT_VALMASK);
    /* need to reload if:
       - constant
       - lvalue (need to dereference pointer)
       - already a register, but not in the right class */
    if (or(or(gte(r, VT_CONST),
              and(gsv_r(vtop), VT_LVAL)),
              eq(0, and(ri32(add(reg_classes, mul(r, 4))), rc)))) {
        r = get_reg(rc);
        if (and(gsv_r(vtop), VT_LVAL)) {
            /* lvalue of scalar type : need to use lvalue type
               because of possible cast */
            t = gct_t(gsv_type(vtop));
            t1 = t;
            sct_t(gsv_type(vtop), t);
            load(r, vtop);
            /* restore wanted type */
            sct_t(gsv_type(vtop), t1);
        } else {
            /* one register type load */
            load(r, vtop);
        }
    }
    ssv_r(vtop, r);
    return r;
}

/* 31 */
/* Generate value test
 *
 * Generate a test for any value (jump, comparison and integers) */
int gvtst(int inv, int t) {
    int v;

    v = and(gsv_r(vtop), VT_VALMASK);
    vpushi(0);
    gen_op(TOK_NE);
    if (eq(and(gsv_r(vtop), or(or(VT_VALMASK, VT_LVAL), VT_SYM)), VT_CONST)) {
        vtop = sub(vtop, sizeof_SValue);
        return t;
    }
    return gtst(inv, t);
}

/* 33 */
/* handle integer constant optimizations and various machine
   independent opt */
int gen_opic(int op) {
    int v1;
    int v2;
    int c1;
    int c2;

    v1 = sub(vtop, sizeof_SValue);
    v2 = vtop;
    c1 = eq(and(gsv_r(v1), or(or(VT_VALMASK, VT_LVAL), VT_SYM)), VT_CONST);
    c2 = eq(and(gsv_r(v2), or(or(VT_VALMASK, VT_LVAL), VT_SYM)), VT_CONST);

    if (and(c1, c2)) {
        scv_i(gsv_c(v1), 1);
        vtop = sub(vtop, sizeof_SValue);
    } else {
        gen_opi(op);
    }
}

/* 34 */
/* generic gen_op: handles types problems */
int gen_op(int op) {
    int t;
    int type1;
    enter();
    type1 = v_alloca(sizeof_CType);

    /* integer operations */
    t = VT_INT;
    vswap();
    sct_t(type1, t);
    sct_ref(type1, 0);
    gen_cast(type1);
    vswap();
    gen_cast(type1);
    gen_opic(op);
    /* relational op: the result is an int */
    sct_t(gsv_type(vtop), VT_INT);
    return leave(0);
}

/* 35 */
int gen_cast(int type) {
    int sbt;
    int dbt;
    int c;
    int p;
    int m;

    dbt = and(gct_t(type), or(VT_BTYPE, VT_UNSIGNED));
    sbt = and(gct_t(gsv_type(vtop)), or(VT_BTYPE, VT_UNSIGNED));

    if (neq(sbt, dbt)) {
        c = eq(and(gsv_r(vtop), or(or(VT_VALMASK, VT_LVAL), VT_SYM)), VT_CONST);
        p = eq(and(gsv_r(vtop), or(or(VT_VALMASK, VT_LVAL), VT_SYM)), or(VT_CONST, VT_SYM));
        if (c) {
            if(eq(and(dbt, VT_BTYPE), VT_BYTE)) {
                m = 255;
            } else {
                m = sub(0, 1); /* 0xffffffff */
            }
            scv_i(gsv_c(vtop), and(gcv_i(gsv_c(vtop)), m));
            if (eq(0, and(dbt, VT_UNSIGNED))) {
                /* LJW FIXME shr might go wrong because of sign extension */
                scv_i(gsv_c(vtop), or(gcv_i(gsv_c(vtop)), sub(0, and(gcv_i(gsv_c(vtop)), add(shr(m, 1), 1)))));
            }
        }
    }
    memmove(gsv_type(vtop), type, sizeof_CType);
}

/* 36 */
/* return type size as known at compile time. Put alignment at 'a' */
int type_size(int type, int a) {
    int s;
    int bt;
    int ts;

    bt = and(gct_t(type), VT_BTYPE);
    if (eq(bt, VT_PTR)) {

        s = gct_ref(type);
        ts = type_size(gsym_type(s), a);

        return mul(ts, gsym_c(s));
    } else if(eq(bt, VT_INT)) {
        wi32(a, 4);
        return 4;
    } else {
        /* char, void, function, _Bool */
        wi32(a, 1);
        return 1;
    }
}

/* 37 */
/* return the pointed type of t */
int pointed_type(int type) {
    return gsym_type(gct_ref(type));
}

/* 38 */
/* modify type so that its it is a pointer to type. */
int mk_pointer(int type) {
    int s;
    s = sym_push(SYM_FIELD, type, 0, sub(0, 1));
    sct_t(type, or(VT_PTR, and(gct_t(type), VT_STORAGE)));
    sct_ref(type, s);
}

/* 39 */
/* verify type compatibility to store vtop in 'dt' type, and generate
   casts if needed. */
int gen_assign_cast(int dt) {
    gen_cast(dt);
}

/* 40 */
/* store vtop in lvalue pushed on stack */
int vstore() {
    int r;

    gen_assign_cast(gsv_type(sub(vtop, sizeof_SValue)));
    r = gv(RC_INT);  /* generate value */
    store(r, sub(vtop, sizeof_SValue));
    vswap();

    /* NOT vpop() because on x86 it would flush the fp stack */
    vtop = sub(vtop, sizeof_SValue);
}

/* 41 */
/* return 0 if no type declaration. otherwise, return the basic type
   and skip it.
 */
int parse_btype(int type, int ad) {
    int type_found;
    int typespec_found;
    int s;

    memset(ad, 0, sizeof_AttributeDef);
    type_found = 0;
    typespec_found = 0;
    sct_ref(type, 0);

    while(1) {
        if(eq(tok, TOK_INT)) {
            next();
            typespec_found = 1;
        } else {
            if (typespec_found) {
                break;
            }
            s = sym_find(tok);
            break;
        }
        type_found = 1;
    }

    sct_t(type, VT_INT);
    return type_found;
}

/* 42 */
/* convert a function parameter type (array to pointer and function to
   function pointer) */
int convert_parameter_type(int pt) {
    /* remove const qualifiers (XXX: const could be used
       to indicate a const function parameter */
    sct_t(pt, and(gct_t(pt), not(VT_CONSTANT)));
    /* array must be transformed to pointer according to ANSI C */
    sct_t(pt, and(gct_t(pt), not(VT_ARRAY)));
}

/* 43 */
int post_type(int type, int ad, int storage) {
    int n;
    int l;
    int align;
    int plast;
    int s;
    int first;
    int ad1;
    int pt;

    enter();
    /* FIXME there is some bug with the v_alloca function should be 4 */
    ad1 = v_alloca(16);
    align = v_alloca(16);
    first = v_alloca(16);
    n = v_alloca(16);
    pt = v_alloca(sizeof_CType);

    if (eq(tok, mkc('('))) {
        /* function type, or recursive declarator (return if so) */
        next();
        if (eq(tok, mkc(')'))) {
          l = 0;
        } else {
            parse_btype(pt, ad1);
            l = FUNC_NEW;
        }
        wi32(first, 0);
        plast = first;
        if (l) {
            while(1) {
                /* read param name and compute offset */
                type_decl(pt, ad1, n);
                type_size(pt, align);
                convert_parameter_type(pt);
                s = sym_push(or(ri32(n), SYM_FIELD), pt, 0, 0);
                wi32(plast, s);
                plast = asym_next(s);
                if (eq(tok, mkc(')'))) {
                    break;
                }
                skip(mkc(','));
                parse_btype(pt, ad1);
            }
        } else {
            /* if no parameters, then old type prototype */
            l = FUNC_OLD;
        }
        skip(mkc(')'));
        /* NOTE: const is ignored in returned type as it has a special
           meaning in gcc / C++ */
        sct_t(type, and(gct_t(type), not(VT_CONSTANT)));
        /* we push a anonymous symbol which will contain the function prototype */
        sad_f_func_type(ad, l);
        s = sym_push(SYM_FIELD, type, 0, 0);
        ssym_f_func_type(s, gad_f_func_type(ad));
        ssym_next(s, ri32(first));
        sct_t(type, VT_FUNC);
        sct_ref(type, s);
    }
    return leave(1);
}

/* 44 */
/* Parse a type declarator (except basic type), and return the type
   in 'type'.
   'type' should contain the basic type. 'ad' is the
   attribute definition of the basic type. It can be modified by
   type_decl().  If this (possibly abstract) declarator is a pointer chain
   it returns the innermost pointed to type (equals *type, but is a different
   pointer), otherwise returns type itself, that's used for recursive calls.  */
int type_decl(int type, int ad, int v) {
    int post;
    int ret;
    int storage;

    /* recursive type, remove storage bits first, apply them later again */
    storage = and(gct_t(type), VT_STORAGE);
    sct_t(type, and(gct_t(type), not(VT_STORAGE)));
    post = type;
    ret = type;

    /* type identifier */
    wi32(v, tok);
    next();
    post_type(post, ad, storage);
    sct_t(type, or(gct_t(type), storage));
    return ret;
}

/* 45 */
/* compute the lvalue VT_LVAL_xxx needed to match type t. */
int lvalue_type(int t) {
    return VT_LVAL;
}

/* 46 */
/* pass a parameter to a function and do type checking and casting */
int gfunc_param_typed(int func, int arg) {
    int func_type;
    int type;

    enter();
    type = v_alloca(sizeof_CType);

    func_type = gsym_f_func_type(func);
    if (neq(func_type, FUNC_OLD)) {
        memmove(type, gsym_type(arg), sizeof_CType);
        /* need to do that to avoid false warning */
        sct_t(type, and(gct_t(type), not(VT_CONSTANT)));
        gen_assign_cast(type);
    }

    leave(0);
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
    } else if(eq(tok, 182)) { /* TOK_CUINT = 182 bodge, maybe hash issue */
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

/* 48 */
int expr_eq() {
    int t;

    unary();
    if (eq(tok, mkc('='))) {
        t = tok;
        next();
        expr_eq();
        vstore();
    }
}

/* 49 */
int gexpr() {
    expr_eq();
}

/* 50 */
int gfunc_return(int func_type) {
    gv(RC_IRET);
    /* NOT vpop() because on x86 it would flush the fp stack */
    vtop = sub(vtop, sizeof_SValue);
}

/* 52 */
/* store a value or an expression directly in global data or in local array */
int init_putv(int type, int sec, int c) {
    int bt;
    int ptr;
    int dtype;
    int size;
    int align;

    enter();
    dtype = v_alloca(sizeof_CType);
    align = v_alloca(4);

    memmove(dtype, type, sizeof_CType);
    /* need to do that to avoid false warning */
    sct_t(dtype, and(gct_t(dtype), not(VT_CONSTANT)));

    /* XXX: not portable */
    /* XXX: generate error if incorrect relocation */
    gen_assign_cast(dtype);
    bt = and(gct_t(type), VT_BTYPE);

    size = type_size(type, align);
    section_reserve(sec, add(c, size));
    ptr = add(gs_data(sec), c);

    wi8(ptr, or(ri8(ptr), gcv_i(gsv_c(vtop))));
    vtop = sub(vtop, sizeof_SValue);

    leave(0);
}

/* 53 */
/* 't' contains the type and storage info. 'c' is the offset of the
   object in section 'sec'. If 'sec' is NULL, it means stack based
   allocation. 'first' is true if array '{' must be read (multi
   dimension implicit array init handling). 'size_only' is true if
   size only evaluation is wanted (only for arrays). */
int decl_initializer(int type, int sec, int c, int first, int size_only) {
    int len;
    int n;
    int nb;
    int i;
    int size1;
    int align1;
    int s;
    int f;
    int t1;
    int cstr_len;
    int ch;

    enter();
    align1 = v_alloca(4);

    s = gct_ref(type);
    n = gsym_c(s);
    t1 = pointed_type(type);
    size1 = type_size(t1, align1);

    /* only parse strings here if correct type (otherwise: handle
       them as ((w)char *) expressions */
    if (or(eq(and(gct_t(t1), VT_BTYPE), VT_INT),
        and(eq(tok, TOK_STR), eq(and(gct_t(t1), VT_BTYPE), VT_BYTE)))) {
        len = 0;
        while (eq(tok, TOK_STR)) {

            cstr_len = gcv_str_size(atokc);
            cstr_len = sub(cstr_len, 1);
            nb = cstr_len;
            if (eq(0, size_only)) {
                memcpy(add(add(gs_data(sec), c), len), gcv_str_data(atokc), nb);
            }
            len = add(len, nb);
            next();
        }
        /* only add trailing zero if enough storage (no
           warning in this case since it is standard) */
        if (or(lt(n, 0), lt(len, n))) {
            if (eq(0, size_only)) {
                vpushi(0);
                init_putv(t1, sec, add(c, mul(len, size1)));
            }
            len = add(len, 1);
        }
        len = mul(len, size1);
    }

    /* patch type size if needed, which happens only for array types */
    if (lt(n, 0)) {
        if(eq(size1, 1)) {
            ssym_c(s, len);
        } else {
            ssym_c(s, div_(sub(add(len, size1), 1), size1));
        }
    }

    return leave(0);
}

/* 55 */
/* parse a function defined by symbol 'sym' and generate its code in
   'cur_text_section' */
int gen_function(int sym) {
    ind = gs_data_offset(cur_text_section);
    /* NOTE: we patch the symbol size later */
    put_extern_sym(sym, cur_text_section, ind, 0);
    funcname = get_tok_str(gsym_v(sym), 0);
    func_ind = ind;
    /* push a dummy symbol to enable local sym storage */
    sym_push2(alocal_stack, SYM_FIELD, 0, 0);
    local_scope = 1; /* for function parameters */
    gfunc_prolog(gsym_type(sym));
    local_scope = 0;
    rsym = 0;
    block(0, 0, 0);
    gsym(rsym);
    gfunc_epilog();
    ss_data_offset(cur_text_section, ind);
    /* reset local stack */
    local_scope = 0;
    sym_pop(alocal_stack, 0, 0);
    /* end of function */
    /* patch symbol size */
    ses_st_size(elfsym(sym), sub(ind, func_ind));
    cur_text_section = 0;
    funcname = mks(""); /* for safety */
    sct_t(afunc_vt, 0); /* for safety */
    ind = 0; /* for safety */
}

/* 54 */
/* parse an initializer for type 't' if 'has_init' is non zero, and
   allocate space in local or global data space ('r' is either
   VT_LOCAL or VT_CONST). If 'v' is non zero, then an associated
   variable 'v' of scope 'scope' is declared before initializers
   are parsed. If 'v' is zero, then a reference to the new object
   is put in the value stack. If 'has_init' is 2, a special parsing
   is done to handle string constants. */
int decl_initializer_alloc(int type, int ad, int r,
                                   int has_init, int v, int scope) {
    int size;
    int align;
    int addr;
    int init_str;
    int sec;
    int flexible_array;
    int sym;

/* FIXME there is a stack allocation bug somewhere causing crashes */
    enter();
    align = v_alloca(16); /* in theory 4 should work but there is a bug */

    init_str = 0;
    sym = 0;
    flexible_array = 0;
    int oldreloc_offset;

    size = type_size(type, align);
    /* If unknown size, we must evaluate it before
       evaluating initializers because
       initializers can generate global data too
       (e.g. string pointers or ISOC99 compound
       literals). It also simplifies local
       initializers handling */
    if (or(lt(size, 0), and(flexible_array, has_init))) {
        /* get all init string */
        init_str = tok_str_alloc();
        /* only get strings */
        while (eq(tok, TOK_STR)) {
            tok_str_add_tok(init_str);
            next();
        }
        tok_str_add(init_str, sub(0, 1));
        tok_str_add(init_str, 0);
        unget_tok(0);

        /* compute size */
        begin_macro(init_str, 1);
        next();
        decl_initializer(type, 0, 0, 1, 1);
        /* prepare second initializer parsing */
        macro_ptr = gtkst_str(init_str);
        next();

        /* if still unknown size, error */
        size = type_size(type, align);
    }

    if (eq(and(r, VT_VALMASK), VT_LOCAL)) {
        sec = 0;
        loc = and(sub(loc, size), sub(0, ri32(align)));
        addr = loc;
        if (v) {
            sym = sym_push(v, type, r, addr);
        } else {
            /* push local reference */
            vset(type, r, addr);
        }
    } else {

        /* allocate symbol in corresponding section */
        sec = gad_section(ad);
        if (eq(0, sec)) {
            if (has_init) {
                sec = data_section;
            } else {
                sec = bss_section;
            }
        }

        addr = section_add(sec, size, ri32(align));

        if (v) {
            if (eq(0, sym)) {
                sym = sym_push(v, type, or(r, VT_SYM), 0);
                patch_storage(sym, ad, 0);
            }
            /* Local statics have a scope until now (for
               warnings), remove it here.  */
            ssym_sym_scope(sym, 0);
            /* update symbol definition */
            put_extern_sym(sym, sec, addr, size);
        } else {
            /* push global reference */
            sym = get_sym_ref(type, sec, addr, size);
            vpushsym(type, sym);
            ssv_r(vtop, or(gsv_r(vtop), r));
        }

    }

    if (has_init) {
        oldreloc_offset = 0;
        decl_initializer(type, sec, addr, 1, 0);
    }

    /* restore parse state if needed */
    if (init_str) {
        end_macro();
        next();
    }

    return leave(0);
}

/* 56 */
/* 'l' is VT_LOCAL or VT_CONST to define default storage type, or VT_CMP
   if parsing old style parameter decl list (and FUNC_SYM is set then) */
int decl0(int l, int is_for_loop_init, int func_sym) {
    int v;
    int has_init;
    int r;
    int type;
    int btype;
    int sym;
    int ad;

    enter();
    type = v_alloca(sizeof_CType);
    btype = v_alloca(sizeof_CType);
    ad = v_alloca(mul(2,sizeof_AttributeDef)); /* FIXME shouldn't have to double */
    v = v_alloca(4);

    while (1) {
        if (eq(0, parse_btype(btype, ad))) {
                break;
        }
        while (1) { /* iterate thru each declaration */
            memmove(type, btype, sizeof_CType);
            type_decl(type, ad, v);
            if (eq(and(gct_t(type), VT_BTYPE), VT_FUNC)) {
                /* if old style function prototype, we accept a
                   declaration list */
                sym = gct_ref(type);
                if (and(eq(gsym_f_func_type(sym), FUNC_OLD), eq(l, VT_CONST))) {
                    decl0(VT_CMP, 0, sym);
                }
            }

            if (eq(tok, mkc('{'))) {
                /* put function symbol */
                sym = external_global_sym(ri32(v), type, 0);
                sct_t(type, and(gct_t(type), not(VT_EXTERN)));
                patch_storage(sym, ad, type);

                /* compute text section */
                cur_text_section = gad_section(ad);
                if (eq(0, cur_text_section)) {
                    cur_text_section = text_section;
                }
                gen_function(sym);
                break;
            } else {
                    r = 0;
                    if (eq(and(gct_t(type), VT_BTYPE), VT_FUNC)) {
                        /* external function definition */
                        /* specific case for func_call attribute */
                        ssym_f_func_type(gct_ref(type), gad_f_func_type(ad));
                    } else if (eq(0, and(gct_t(type), VT_ARRAY))) {
                        /* not lvalue if array */
                        r = or(r, lvalue_type(gct_t(type)));
                    }
                    has_init = eq(tok, mkc('='));
                    if (or(or(and(and(gct_t(type), VT_EXTERN),
                                  or(eq(0,has_init), neq(l, VT_CONST))),
                                  eq(and(gct_t(type), VT_BTYPE), VT_FUNC)),
                             ((and(and(gct_t(type), VT_ARRAY),
                                      and(gct_t(type), VT_STATIC)))))) {
                        /* external variable or function */
                        /* NOTE: as GCC, uninitialized global static
                           arrays of null size are considered as
                           extern */
                        sct_t(type, or(gct_t(type), VT_EXTERN));
                        sym = external_sym(ri32(v), type, r, ad);
                    } else {
                        r = or(r, l);
                        if (eq(l, VT_CONST)) {
                            /* uninitialized global variables may be overridden */
                            sct_t(type, or(gct_t(type), VT_EXTERN));
                        }
                        decl_initializer_alloc(type, ad, r, has_init, ri32(v), l);
                    }
                if (neq(tok, mkc(','))) {
                    skip(mkc(';'));
                    break;
                }
            }
        }
    }
    return leave(0);
}


/* 57 */
int decl(int l) {
    decl0(l, 0, 0);
}

/* ------------------------------------------------------------------------- */

/* end of tccgen.c */

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
