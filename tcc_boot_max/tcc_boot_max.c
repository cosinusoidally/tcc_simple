/* tcc_boot_max */

/* Section accessors */

int gs_data(int o) {return ri32(add(o, Section_data_o));}

int gs_data_allocated(int o) {return ri32(add(o, Section_data_allocated_o));}

int init_runtime(){
  puts("init_runtime");

  Section_data_o = 4;
  Section_data_allocated_o = 8;
  sizeof_Section = 76;

  init_c();

  init_globals();
}

/* start of i386-gen.c */

/* XXX: make it faster ? */
int g(int c) {
    int ind1;
    if (nocode_wanted) {
        return;
    }
    ind1 = add(ind, 1);
    if (gt(ind1, gs_data_allocated(cur_text_section))) {
        section_realloc(cur_text_section, ind1);
    }
    wi8(add(gs_data(cur_text_section), ind), c);
    ind = ind1;
}

/* end of i386-gen.c */
