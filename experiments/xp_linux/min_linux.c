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

init_globals() {
  base_address = 64 * 1024 * 1024;
  data_area = base_address+0x20000;
  regs_data = data_area;
  host_puts = data_area +(4*9);
//  wi32(host_puts, puts);
}

int load_boot(filename) {
  int f;
  int o;
  f = fopen(filename, "rb");
  fclose(f);
}

int main(int argc, char **argv) {
  int res = 0;
  init_globals();
  res = mmap(base_address, 512*1024*1024, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, 0, 0);
  if(res != base_address) {
    puts("mmap error\n");
    exit(1);
  }
  return 0;
}
