extern int stdout;

int wrap_syscall2() {
  printf("wrap_syscall2 called\n");
  return 0;
}

int wrap_syscall() {
  printf("wrap_syscall called\n");
  return 0;
}

main(){
  char *a="Test syscall\n";
  fputs("hello world\n", stdout);
  printf("a: %d\n",a);
  printf("stdout: %d\n", stdout);
  syscall(65535, 0, a, strlen(a));
  syscall(65536, wrap_syscall2);
  syscall(65534, wrap_syscall);
  fputs("test_trap\n", stdout);
  syscall(65533);
  fputs("more\n", stdout);
  return 0;
}
