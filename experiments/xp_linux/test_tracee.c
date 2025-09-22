extern int stdout;

int syscall_no = 0;
int regs_data[8];

int wrap_syscall2() {
  printf("wrap_syscall2 called\n");
  return 0;
}

int wrap_syscall() {
  char *b="wrap_syscall called\n";
  syscall(65533);
  printf("wrap_syscall %d %s\n", regs_data[0], regs_data[1]);
  syscall(65534, wrap_syscall, regs_data);
  return 0;
}

main(){
  char *a="Test syscall\n";
  char *b="test_trap\n";
  fputs("hello world\n", stdout);
  printf("a: %d\n",a);
  printf("stdout: %d\n", stdout);
  syscall(65535, 0, a, strlen(a));
  syscall(65536, wrap_syscall2);
  syscall(65534, wrap_syscall, regs_data);
  syscall(4, 0, b, strlen(b));
  syscall(65533);
  fputs("more\n", stdout);
  return 0;
}
