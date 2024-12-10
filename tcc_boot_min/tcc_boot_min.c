int init_runtime(){
  foo=mks("hello world");
  puts(foo);
  printf(mks("blah: %d\n"),add(1,2));

  init_c();
}

int main(argc, argv){
  puts(mks("tcc_boot_min main"));
  return main_(argc,argv);
}
