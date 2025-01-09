/* tcc_boot_max */

int init_globals() {
  puts("init_globals");
}

int init_runtime(){
  puts("init_runtime");
  init_globals();
}
