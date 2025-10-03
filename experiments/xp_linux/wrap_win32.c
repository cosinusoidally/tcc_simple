#include <stdio.h>
#include <windows.h>

int syscall(x) {
  printf("sycall function not impl %d\n", x);
  exit(1);
}

int mmap(base, size) {
  printf("mmap not impl base: 0x%x size: 0x%x\n", base, size);
  exit(1);
}

#include "test_tracee.c"
