#include <stdio.h>

int regs_data;

int PROT_READ=1;
int PROT_WRITE=2;
int PROT_EXEC=4;
int MAP_ANONYMOUS=32;
int MAP_PRIVATE=2;
int MAP_FIXED=0x10;

int base_address;
int data_area;
int host_puts;

int elf_base = 0x8048000;

int wi32(int o,int v) {
        int *h;
        h = o;
        h[0]=v;
        return 0;
}

int ri32(o) {
  int *h;
  h = o;
  return h[0];
}

int wrap_syscall();

int trap_syscalls_on() {
  syscall(65534, wrap_syscall, regs_data);
}

int trap_syscalls_off() {
  syscall(65533);
}

int get_reg(x) {
  return ri32(regs_data+ (x*4));
}

int wrap_syscall_alt(edi, esi, ebp, esp, ebx, edx, ecx, eax) {
  printf("in wrap_syscall_alt\n");
  printf("eax\t\t0x%x\n", eax);
  printf("ecx\t\t0x%x\n", ecx);
  printf("edx\t\t0x%x\n", edx);
  printf("ebx\t\t0x%x\n", ebx);
  printf("esp\t\t0x%x\n", esp);
  printf("ebp\t\t0x%x\n", ebp);
  printf("esi\t\t0x%x\n", esi);
  printf("edi\t\t0x%x\n", edi);
  exit(1);
/*
  regs_data[0] = eax;
  regs_data[1] = ebx;
  regs_data[2] = ecx;
  regs_data[3] = edx;
  regs_data[4] = esi;
  regs_data[5] = edi;
  regs_data[6] = ebp;
*/
  return wrap_syscall();
}

int wrap_syscall() {
  int r;
  int n;
//  printf("wrap_syscall eax: %d ebx: %d ecx: %d edx: %d esi: %d edi: %d ebp: %d\n", regs_data[0], regs_data[1], regs_data[2], regs_data[3], regs_data[4], regs_data[5], regs_data[6]);
  trap_syscalls_off();
  n = get_reg(0);
  printf("wrap_syscall eax: %d\n", n);
  printf("unsupported syscall: %d\n",n);
  exit(1);
}

int load_boot(filename) {
  int f;
  int o;
  int size;
  f = fopen(filename, "rb");
  o = elf_base;
  while(size = fread(o, 1, 1024*1024, f)) {
    o = o + size;
  }
  fclose(f);
  return o - elf_base;
}

init_globals() {
  base_address = 64 * 1024 * 1024;
  data_area = base_address+0x20000;
  regs_data = data_area;
  host_puts = data_area + (4*9);
}

init_runtime() {
  wi32(host_puts, puts);
  printf("load_size: %d\n", load_boot("../cjsawk/artifacts/builds/hello/hello.exe"));
}

run_process() {
  trap_syscalls_on();
  /* set up stack pointer */
  asm("mov $0x8045800,%esp");
  /* this is a jmp to the entrypoint, stored in elf_base + 0x18 */
  asm("jmp *0x8048018");
}

int main(int argc, char **argv) {
  int res = 0;
  init_globals();
  res = mmap(base_address, 512*1024*1024, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, 0, 0);
  if(res != base_address) {
    puts("mmap error\n");
    exit(1);
  }
  init_runtime();
  run_process();
  return 0;
}
