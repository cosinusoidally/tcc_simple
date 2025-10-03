#include <windows.h>
#include <stdio.h>

int regs_data[8];

int ri32(o) {
  int *h;
  h = (int *)o;
  return h[0];
}

int wi32(o,v) {
  int *h;
  h =(int*)o;
  h[0]=v;
}

int ri8(o) {
  char *h;
  h = (char *)o;
  return h[0] & 255;
}


int wi8(o,v) {
  char *h;
  h = (char*)o;
  h[0]=v;
}

int elf_base=0x8048000;

int ext_call() {
asm("mov $0x8048054, %eax");
asm(".byte 255");
asm(".byte 224");
}

int dump_regs() {
  printf("dump_regs eax: %d ebx: %d ecx: %d edx: %d esi: %d edi: %d ebp: %d\n", regs_data[0], regs_data[1], regs_data[2], regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
}

int vm_write() {
//  printf("vm_write not impl\n");
//  dump_regs();
  int fd = regs_data[1];
  int buf = regs_data[2];
  int count = regs_data[3];
  fputc(ri8(buf), stdout);
  return count;
}

int vm_exit() {
  int error_code = regs_data[1];
  printf("vm_exit: %d\n", error_code);
  exit(0); // swallow exit codes
}

int wrap_syscall() {
  int r;
  int n;
//  printf("wrap_syscall called\n");
  n = regs_data[0];
  if(n == 4) {
    r = vm_write();
  } else if(n == 1) {
    r = vm_exit();
  } else {
    printf("unsupported syscall: %d\n", n);
    exit(1);
  }
  return r;
}

LONG CALLBACK TopLevelHandler(EXCEPTION_POINTERS* info)
{
  int eip, esp;
//  printf("Executed toplevelhandler, Exception: %X\n", info->ExceptionRecord->ExceptionCode); //print any other exceptions we encounter
  eip = info->ContextRecord->Eip;
  esp = info->ContextRecord->Esp;
//  printf("eip: 0x%x esp: 0x%x\n",eip, esp);
//  printf("instruction: %x\n",ri8(eip));
  if(ri8(eip)==0xCD) {
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

load_file(realname, virtualname) {
  printf("load_file: %s virtualname: %s\n", realname, virtualname);
  int f = (int)fopen((char const *)realname, "rb");
  int c;
  int i = 0;
  while((c = fgetc((struct _iobuf*)f)) != -1) {
    wi8(elf_base+i, c);
    i = i + 1;
  }
  printf("file_length: %d\n", i);
  fclose((struct _iobuf*)f);
}

main(){
  int r;
  int size = 512*1024*1024;
  int i;
  int mem_base=elf_base-(1024*1024);
  mem_base = 64*1024*1024;
  printf("Hello world\n");
  r= (int)VirtualAlloc((LPVOID)mem_base, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  SetUnhandledExceptionFilter(TopLevelHandler);
  int out=0;
  out=ri32(elf_base);
  printf("r: 0x%x out: %x\n",r, out);
  load_file((int)"artifacts/hello.exe",(int)"blah");
/*
  wi8(elf_base,0xB8);
  wi32(elf_base+1,0xDEADBEEF);
  wi8(elf_base+5,0xCD);
  wi8(elf_base+6,0x80);
  wi8(elf_base+7,0xC3);
*/
  out = ri32(elf_base);
  printf("r: 0x%x out: %x\n",r, out);
  int foo = ext_call();
  printf("got here %x\n", foo);
/*
asm {
int 0x80
}
*/
  return;
}
