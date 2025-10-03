#include <stdio.h>
#include <windows.h>

int syscall(x) {
  printf("sycall function not impl %d\n", x);
  exit(1);
}

int mmap(base, size) {
  int r;
  printf("mmap not impl base: 0x%x size: 0x%x\n", base, size);
  r = VirtualAlloc((LPVOID)base, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  printf("r: 0x%x\n",r);
  exit(1);
}

#include "test_tracee.c"
