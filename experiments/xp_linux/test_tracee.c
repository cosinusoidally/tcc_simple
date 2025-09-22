extern int stdout;

main(){
  fputs("hello world\n", stdout);
  syscall(65535,"Test syscall\n");
  return 0;
}
