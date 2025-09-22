extern int stdout;

main(){
  char *a="Test syscall\n";
  fputs("hello world\n", stdout);
  printf("a: %d\n",a);
  printf("stdout: %d\n", stdout);
  syscall(65535, 0, a, strlen(a));
  fputs("more\n", stdout);
  return 0;
}
