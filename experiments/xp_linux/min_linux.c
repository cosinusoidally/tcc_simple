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
int host_call_fn;
int host_params;
int host_stdout;

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

int set_reg(x, v) {
  wi32(regs_data+ (x*4), v);
}

dump_regs() {
  printf("wrap_syscall eax: %d ebx: %d ecx: %d edx: %d esi: %d edi: %d ebp: %d\n", get_reg(0), get_reg(0),get_reg(1),get_reg(2),get_reg(3),get_reg(4),get_reg(5),get_reg(6));
}

int wrap_syscall_alt(edi, esi, ebp, esp, ebx, edx, ecx, eax) {
  set_reg(0, eax);
  set_reg(1, ebx);
  set_reg(2, ecx);
  set_reg(3, edx);
  set_reg(4, esi);
  set_reg(5, edi);
  set_reg(6, ebp);
  return wrap_syscall();
}

int wrap_syscall() {
  int r;
  int n;
  trap_syscalls_off();
  n = get_reg(0);
  dump_regs();
  r = syscall(get_reg(0), get_reg(1), get_reg(2),get_reg(3),get_reg(4),get_reg(5), get_reg(6));
  trap_syscalls_on();
  return r;
}

int load_boot(filename) {
  int f;
  int o;
  int size;
  f = fopen(filename, "rb");
  o = elf_base;
  while(size = fread(o, 1, 4096, f)) {
    o = o + size;
  }
  fclose(f);
  return o - elf_base;
}

init_globals() {
  base_address = 0x4000000; /* 64MB */
  data_area = base_address+0x20000;
  regs_data = data_area;
  host_call_fn = data_area + (4*9);
  host_params = host_call_fn + 4;
  host_stdout = host_params + (4*8);
}

int get_param(x) {
  return ri32(host_params+(4*x));
}

int host_callback() {
  int i;
  int n;
  int r;
  n = get_param(0);
  if(n == 1) {
    trap_syscalls_off();
    puts("trap_syscalls_off");
  } else if(n == 2) {
    puts("trap_syscalls_on");
    trap_syscalls_on();
  } else if(n == 3) {
    trap_syscalls_off();
    puts(get_param(1));
    trap_syscalls_on();
  } else if(n == 4) {
    trap_syscalls_off();
    r = fwrite(get_param(1), get_param(2), get_param(3), get_param(4));
    trap_syscalls_on();
  } else if(n == 5) {
    trap_syscalls_off();
    printf("fopen path: %s mode: %s\n", get_param(1), get_param(2));
    r = fopen(get_param(1), get_param(2));
    trap_syscalls_on();
  } else if(n == 6) {
    trap_syscalls_off();
    printf("fclose: %d\n", get_param(1));
    r = fclose(get_param(1));
    trap_syscalls_on();
  } else if(n == 7) {
    trap_syscalls_off();
    r = fread(get_param(1), get_param(2), get_param(3),get_param(4));
    trap_syscalls_on();
  } else if(n == 8) {
    trap_syscalls_off();
    printf("exit not impl\n");
    exit(1);
  } else {
    trap_syscalls_off();
    printf("Invalid host call: %d\n", n);
    exit(1);
  }
  return r;
}

init_runtime() {
  wi32(host_call_fn, host_callback);
  wi32(host_stdout, stdout);
  set_reg(8, wrap_syscall_alt);
  printf("load_size: %d\n", load_boot("artifacts/xp_linux.exe"));
}

run_process() {
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

/*
  printf("host_call_fn: 0x%x\n", host_call_fn);
  printf("host_params: 0x%x\n", host_params);
  printf("host_stdout: 0x%x\n", host_stdout);
  asm("call *0x4020024");
*/
  run_process();
  return 0;
}
