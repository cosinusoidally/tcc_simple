#include <stdio.h>
#include <windows.h>

int trap_ready = 0;

int wrap_syscall();

LONG CALLBACK TopLevelHandler(EXCEPTION_POINTERS* info)
{
  int eip, esp, i;
  eip = info->ContextRecord->Eip;
  esp = info->ContextRecord->Esp;

  if((ri8(eip)==0xCD)) {
    /* first try and patch in int wrapper */
    int eip_wrap = eip - 10;
    if(ri32(eip_wrap) == 0x90909090 &&
       ri32(eip_wrap + 4) == 0x90909090 &&
       ri32(eip_wrap + 8) == 0x80CD9090) {
      printf("installing wrapper stub 0x%x\n", eip_wrap);
      int syscall_wrap_alt_addr = regs_data()+(4*8);
      printf("syscall_wrap_alt 0x%x\n", ri32(syscall_wrap_alt_addr));
      printf("before: ");
      for(i = 0; i< 12;i++) {
        printf("0x%x ",ri8(eip_wrap+i));
      }
      printf("\n");
      wi32(eip_wrap, 0x15FF6090);
      wi32(eip_wrap + 4, syscall_wrap_alt_addr);
      wi32(eip_wrap + 8, 0x9020C483);
      printf("after:  ");
      for(i = 0; i< 12;i++) {
        printf("0x%x ",ri8(eip_wrap+i));
      }
      printf("\n");
    }
    eip = eip + 2;
    info->ContextRecord->Eip = (int)wrap_syscall;
    esp = esp - 4;
    info->ContextRecord->Esp = esp;
    wi32(esp, eip);
    set_reg(0, info->ContextRecord->Eax);
    set_reg(1, info->ContextRecord->Ebx);
    set_reg(2, info->ContextRecord->Ecx);
    set_reg(3, info->ContextRecord->Edx);
    set_reg(4, info->ContextRecord->Esi);
    set_reg(5, info->ContextRecord->Edi);
    set_reg(6, info->ContextRecord->Ebp);
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

int get_stdout() {
  return stdout;
}

int main(argc, argv) {
  return v_main(argc, argv);
}
