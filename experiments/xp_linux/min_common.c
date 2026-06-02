#define var int
#define function int

#include "globals.js"

function mul(a, b) {
  return a*b;
}

function add(a, b) {
  return a+b;
}

int wi32(int o,int v) {
        int *h;
        h = o;
        h[0]=v;
        return 0;
}

int ri8(o) {
  char *heap = 0;
  return heap[o] & 255;
}

int ri32(o) {
  int *h;
  h = o;
  return h[0];
}

int wrap_syscall();

int trap_syscalls_on() {
  syscall(65534, wrap_syscall, regs_data());
}

int trap_syscalls_off() {
  syscall(65533);
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

typedef int (* FUNC)(void);

int wrap_syscall() {
  int r;
  int n;
  int hook;
  if(hook = ri32(syscall_hook())) {
//    printf("calling syscall_hook 0x%x\n", hook);
    return ((FUNC)hook)();
//    printf("syscall_hook result: 0x%x\n", r);
  }
  trap_syscalls_off();
  printf("Shouldn't get here\n");
  exit(1);
}

int load_boot(filename) {
  int f;
  int o;
  int size;
  f = fopen(filename, "rb");
  o = elf_base();
  while(size = fread(o, 1, 4096, f)) {
    o = o + size;
  }
  fclose(f);
  return o - elf_base();
}

int get_param(x) {
  return ri32(host_params()+(4*x));
}

int host_callback() {
  int i;
  int n;
  int r;
  n = get_param(0);
  if(n == 1) {
    trap_syscalls_off();
  } else if(n == 2) {
    trap_syscalls_on();
  } else if(n == 3) {
    puts(get_param(1));
  } else if(n == 4) {
    r = fwrite(get_param(1), get_param(2), get_param(3), get_param(4));
  } else if(n == 5) {
    printf("fopen path: %s mode: %s\n", get_param(1), get_param(2));
    r = fopen(get_param(1), get_param(2));
  } else if(n == 6) {
    printf("fclose: %d\n", get_param(1));
    r = fclose(get_param(1));
  } else if(n == 7) {
    r = fread(get_param(1), get_param(2), get_param(3),get_param(4));
  } else if(n == 8) {
    printf("exit called\n");
    exit(get_param(1));
  } else {
    trap_syscalls_off();
    printf("Invalid host call: %d\n", n);
    exit(1);
  }
  return r;
}

init_runtime(filename) {
  wi32(host_call_fn(), host_callback);
  wi32(host_stdout_addr(), get_stdout());
  set_reg(8, wrap_syscall_alt);
  printf("load_size: %d\n", load_boot("artifacts/xp_linux.exe"));
  wi32(command_file(), fopen(filename, "rb"));
  if(!ri32(command_file())) {
    printf("error could not open command file\n");
    exit(1);
  }
}

run_process() {
  /* set up stack pointer */
  asm("mov $0x8045800,%esp");
  /* this is a jmp to the entrypoint, stored in elf_base + 0x18 */
  asm("jmp *0x8048018");
}

int PROT_READ=1;
int PROT_WRITE=2;
int PROT_EXEC=4;
int MAP_ANONYMOUS=32;
int MAP_PRIVATE=2;
int MAP_FIXED=0x10;

int v_main(int argc, int argv) {
  int res = 0;
  int filename;
  res = mmap(base_address(), 512*1024*1024, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, 0, 0);
  if(res != base_address()) {
    puts("mmap error\n");
    exit(1);
  }
  if(argc > 1) {
    filename = ri32(argv+4);
    printf("opening command file %s\n", filename);
  } else {
    printf("opening default command file new_test.list\n");
    filename = "new_test.list";
  }
  init_runtime(filename);
  run_process();
  return 0;
}
