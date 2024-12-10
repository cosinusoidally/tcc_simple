int init_runtime(){
  foo="hello world";
  puts(foo);
  printf("blah: %d\n",add(1,2));
}

int main(argc, argv){
  puts("tcc_boot_min main");
  return main_(argc,argv);
}
