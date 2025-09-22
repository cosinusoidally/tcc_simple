extern int stdout;

int syscall_no = 0;
int regs_data[8];

int wrap_syscall2() {
  printf("wrap_syscall2 called\n");
  return 0;
}

int wrap_syscall() {
  char *b="wrap_syscall called\n";
  int r;
  syscall(65533);
  printf("wrap_syscall eax: %d ebx: %d ecx: %d edx: %d esi: %d edi: %d ebp: %d\n", regs_data[0], regs_data[1], regs_data[2], regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
  r = syscall(regs_data[0],regs_data[1],regs_data[2],regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
  syscall(65534, wrap_syscall, regs_data);
  return r;
}

main(){
  char *a="Test syscall\n";
  char *b="test_trap blah\n";
  fputs("hello world\n", stdout);
  printf("a: %d\n",a);
  printf("stdout: %d\n", stdout);
  syscall(65535, 0, a, strlen(a));
  syscall(65536, wrap_syscall2);
  syscall(65534, wrap_syscall, regs_data);
  syscall(4, 0, b, strlen(b));
  syscall(4, 0, a, strlen(a));
  syscall(65533);
  fputs("more\n", stdout);
  return 0;
}
