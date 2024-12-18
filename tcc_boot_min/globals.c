/* =============== start globals.c ======================================== */

int foo;

int TCCState_sections_o;
int TCCState_nb_sections_o;
int TCCState_priv_sections_o;
int TCCState_nb_priv_sections_o;
int TCCState_symtab_o;
int TCCState_files_o;
int TCCState_nb_files_o;
int TCCState_filetype_o;
int TCCState_outfile_o;
int sizeof_TCCState;

int filespec_type_o;
int filespec_name_o;
int sizeof_filespec;

int Section_data_offset_o;
int Section_data_o;
int Section_data_allocated_o;
int Section_sh_name_o;
int Section_sh_num_o;
int Section_sh_type_o;
int Section_sh_flags_o;
int Section_sh_info_o;
int Section_sh_addralign_o;
int Section_sh_entsize_o;
int Section_sh_size_o;
int Section_sh_addr_o;
int Section_sh_offset_o;
int Section_nb_hashed_syms_o;
int Section_link_o;
int Section_reloc_o;
int Section_hash_o;
int Section_name_o;
int sizeof_Section;

int SValue_type_o;
int SValue_r_o;
int SValue_c_o;
int SValue_sym_o;
int sizeof_SValue;

int CValue_i_o;
int sizeof_CValue;

int CType_t_o;
int CType_ref_o;
int sizeof_CType;

int Sym_v_o;
int Sym_type_o;
int Sym_next_o;
int sizeof_Sym;

int BufferedFile_buf_ptr_o;
int BufferedFile_buf_end_o;
int BufferedFile_fd_o;
int BufferedFile_prev_o;
int BufferedFile_line_num_o;
int BufferedFile_filename_o;
int BufferedFile_buffer_o;
int sizeof_BufferedFile;
int sizeof_BufferedFile_filename;

int TCCOption_name_o;
int TCCOption_index_o;
int TCCOption_flags_o;
int sizeof_TCCOption;

int Elf32_Ehdr_e_ident_o;
int Elf32_Ehdr_e_type_o;
int Elf32_Ehdr_e_machine_o;
int Elf32_Ehdr_e_version_o;
int Elf32_Ehdr_e_entry_o;
int Elf32_Ehdr_e_phoff_o;
int Elf32_Ehdr_e_shoff_o;
int Elf32_Ehdr_e_flags_o;
int Elf32_Ehdr_e_ehsize_o;
int Elf32_Ehdr_e_phentsize_o;
int Elf32_Ehdr_e_phnum_o;
int Elf32_Ehdr_e_shentsize_o;
int Elf32_Ehdr_e_shnum_o;
int Elf32_Ehdr_e_shstrndx_o;
int sizeof_Elf32_Ehdr;

int sizeof_Elf32_Phdr;

int Elf32_Rel_r_offset_o;
int Elf32_Rel_r_info_o;
int sizeof_Elf32_Rel;

int Elf32_Shdr_sh_name_o;
int Elf32_Shdr_sh_type_o;
int Elf32_Shdr_sh_flags_o;
int Elf32_Shdr_sh_addr_o;
int Elf32_Shdr_sh_offset_o;
int Elf32_Shdr_sh_size_o;
int Elf32_Shdr_sh_link_o;
int Elf32_Shdr_sh_info_o;
int Elf32_Shdr_sh_addralign_o;
int Elf32_Shdr_sh_entsize_o;
int sizeof_Elf32_Shdr;

int Elf32_Sym_st_name_o;
int Elf32_Sym_st_value_o;
int Elf32_Sym_st_size_o;
int Elf32_Sym_st_info_o;
int Elf32_Sym_st_other_o;
int Elf32_Sym_st_shndx_o;
int sizeof_Elf32_Sym;

int CString_size_o;
int CString_data_o;
int CString_size_allocated_o;
int sizeof_CString;

int TokenSym_hash_next_o;
int TokenSym_sym_identifier_o;
int TokenSym_tok_o;
int TokenSym_len_o;
int TokenSym_str_o;
int sizeof_TokenSym;

int sizeof_void;

int tcc_state;

int FUNC_PROLOG_SIZE;

int ind;
int loc;

int VT_VALMASK;
int VT_SYM;
int VT_CONST;
int VT_LOCAL;
int VT_LVAL;
int VT_CMP;

int R_386_32;
int R_386_PC32;

int func_sub_sp_offset;

int RC_INT;

int vtop;

int reg_classes;

int RC_EAX;
int RC_ECX;
int RC_IRET;

int SYM_FIELD;

int SHF_ALLOC;
int SHF_EXECINSTR;
int SHF_PRIVATE;
int SHF_WRITE;

int SHN_UNDEF;
int SHN_COMMON;

int SHT_HASH;
int SHT_NOBITS;
int SHT_PROGBITS;
int SHT_REL;
int SHT_RELA;
int SHT_RELX;
int SHT_STRTAB;
int SHT_SYMTAB;

int STB_LOCAL;
int STB_GLOBAL;

int aglobal_stack;
int alocal_stack;

int O_RDONLY;
int O_BINARY;

int file;

int CH_EOB;

int IO_BUF_SIZE;

int TOK_FLAG_BOL;
int TOK_FLAG_BOF;

int tok_flags;

int TCC_OPTION_c;
int TCC_OPTION_o;
int TCC_OPTION_nostdinc;
int TCC_OPTION_HAS_ARG;
int TCC_OPTION_NOSEP;

int tcc_options;

/* predefined sections */
int text_section;
int data_section;
int bss_section;
int common_section;

int cur_text_section; /* current section where function code is generated */
/* symbol sections */
int symtab_section;

int REL_SECTION_FMT;

int table_ident;

int tok_ident;

/* keywords */

int TOK_INT;
int TOK_IF;
int TOK_ELSE;
int TOK_WHILE;
int TOK_BREAK;
int TOK_RETURN;

/* end of keywords */

int TOK_IDENT;

int TOK_ALLOC_INCR;

int TOK_HASH_SIZE_;
int hash_ident_;

/* =============== end globals.c ========================================== */

