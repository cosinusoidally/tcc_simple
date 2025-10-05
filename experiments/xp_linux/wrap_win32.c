#include <stdio.h>
#include <windows.h>

#include "test_tracee.c"

int trap_ready = 0;

LONG CALLBACK TopLevelHandler(EXCEPTION_POINTERS* info)
{
  int eip, esp;
//  printf("Executed toplevelhandler, Exception: %X\n", info->ExceptionRecord->ExceptionCode); //print any other exceptions we encounter
  eip = info->ContextRecord->Eip;
  esp = info->ContextRecord->Esp;


//  printf("eip: 0x%x esp: 0x%x\n",eip, esp);
//  printf("instruction: %x\n",ri8(eip));
  if((ri8(eip)==0xCD)) {
    /* first try and patch in int wrapper */
    int eip_wrap = eip - 10;
    if(ri32(eip_wrap) == 0x90909090 &&
       ri32(eip_wrap + 4) == 0x90909090 &&
       ri32(eip_wrap + 8) == 0x80CD9090) {
      printf("installing wrapper stub 0x%x\n", eip_wrap);
      int syscall_wrap_alt_addr = &regs_data[8];
      printf("syscall_wrap_alt 0x%x\n", ri32(syscall_wrap_alt_addr));
      printf("before: ");
      for(int i = 0; i< 12;i++) {
        printf("0x%x ",ri8(eip_wrap+i));
      }
      printf("\n");
//    wi32(eip_wrap, 0x58505850);
//    wi32(eip_wrap + 8, 0x58505850);
//    wi32(eip_wrap + 4, 0x80CD5850);
      wi32(eip_wrap, 0x15FF6090);
      wi32(eip_wrap + 4, syscall_wrap_alt_addr);
      wi32(eip_wrap + 8, 0x9020C483);
      printf("after:  ");
      for(int i = 0; i< 12;i++) {
        printf("0x%x ",ri8(eip_wrap+i));
      }
      printf("\n");
    }
    int int_num = ri8(eip+1);
//    printf("in interrupt 0x%x\n", int_num);
    eip = eip + 2;
    info->ContextRecord->Eip = (int)wrap_syscall;
    esp = esp - 4;
    info->ContextRecord->Esp = esp;
//    printf("eip new: 0x%x\n", eip);
    wi32(esp, eip);
    regs_data[0] = info->ContextRecord->Eax;
    regs_data[1] = info->ContextRecord->Ebx;
    regs_data[2] = info->ContextRecord->Ecx;
    regs_data[3] = info->ContextRecord->Edx;
    regs_data[4] = info->ContextRecord->Esi;
    regs_data[5] = info->ContextRecord->Edi;
    regs_data[6] = info->ContextRecord->Ebp;
    return EXCEPTION_CONTINUE_EXECUTION;
  } else {
    printf("not interrupt\n");
    return EXCEPTION_CONTINUE_SEARCH;
  }
}

int syscall(x) {
  if(x == 65534) {
    if(trap_ready) {
      return;
    } else {
      printf("installing trap\n");
      SetUnhandledExceptionFilter(TopLevelHandler);
      trap_ready = 1;
      return;
    }
  } else if(x == 65533) {
    // ignore
    return;
  } else if(x == 4) {
//      printf("sycall write missing impl %d\n", x);
/* temp hacky code */
    int fd = regs_data[1];
    int buf = regs_data[2];
    int count = regs_data[3];
    write(fd, buf, count);
    return 0;
  } else {
      printf("sycall function missing impl %d\n", x);
  }
  exit(1);
}

int mmap(base, size) {
  int r;
  printf("mmap not impl base: 0x%x size: 0x%x\n", base, size);
  r = VirtualAlloc((LPVOID)base, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  printf("r: 0x%x\n",r);
  return r;
}

