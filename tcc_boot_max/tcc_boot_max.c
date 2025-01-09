/* tcc_boot_max */

/* Section accessors */

int gs_data(int o) {return ri32(add(o, Section_data_o));}

int gs_data_allocated(int o) {return ri32(add(o, Section_data_allocated_o));}

int init_globals() {
  puts("init_globals");
}

int init_runtime(){
  puts("init_runtime");

  Section_data_o = 4;
  Section_data_allocated_o = 8;
  sizeof_Section = 76;

  init_globals();
}

/* start of i386-gen.c */

/* end of i386-gen.c */
