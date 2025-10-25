#include <stdio.h>

int PROT_READ=1;
int PROT_WRITE=2;
int PROT_EXEC=4;
int MAP_ANONYMOUS=32;
int MAP_PRIVATE=2;
int MAP_FIXED=0x10;

int get_stdout() {
  return stdout;
}

int main(int argc, char **argv) {
  int res = 0;
  int filename;
  res = mmap(base_address(), 512*1024*1024, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, 0, 0);
  if(res != base_address()) {
    puts("mmap error\n");
    exit(1);
  }
  if(argc > 1) {
    printf("opening command file %s\n", argv[1]);
    filename = argv[1];
  } else {
    printf("opening default command file new_test.list\n");
    filename = "new_test.list";
  }
  init_runtime(filename);
  run_process();
  return 0;
}
