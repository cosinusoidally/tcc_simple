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

int main(int argc, char **argv) {
  init_globals();
  return 0;
}
