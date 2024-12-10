
/*
(gdb) ptype /o TCCState
type = struct TCCState {
      0      |       4     Elf32_Addr text_addr;
      4      |       4     int has_text_addr;
      8      |       4     unsigned int section_align;
     12      |       4     Section **sections;
     16      |       4     int nb_sections;
     20      |       4     Section **priv_sections;
     24      |       4     int nb_priv_sections;
     28      |       4     Section *symtab;
     32      |       4     struct filespec **files;
*/
int gts_files(int o) {
  ri32(add(o, TCCState_files_o));
}

/*
     36      |       4     int nb_files;
     40      |       4     int filetype; */
int sts_filetype(int o,int v) {
  wi32(add(o, TCCState_filetype_o), v);
}
/*
     44      |       4     char *outfile;
*/

int gts_outfile(int o) {
  ri32(add(o, TCCState_outfile_o));
}

/*
     48      |       4     int argc;
     52      |       4     char **argv;

                                total size (bytes):   56
                             }
*/


/* struct filespec */

int gfs_type(int o) {
  return ri8(add(o,filespec_type_o));
}

int gfs_name(int o) {
  return add(o,filespec_name_o);
}

/*
                                total size (bytes):    2
                             }
*/


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

  init_c();
}

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
