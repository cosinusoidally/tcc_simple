#include <stdio.h>
#include <windows.h>

int syscall(x) {
  printf("sycall function not impl %d\n", x);
  exit(1);
}

int mmap() {
  printf("mmap not impl\n");
  exit(1);
}

#include "test_tracee.c"
