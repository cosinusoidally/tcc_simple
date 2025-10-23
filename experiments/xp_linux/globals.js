function elf_base() {
  return 0x8048000;
}

function base_address() {
  return 0x4000000;
}

function global_data() {
  return add(base_address(), 0x20000);
}

function globals(x) {
  return add(global_data(), mul(4,x));
}

function regs_data() {
  return globals(0);
}

function host_call_fn() {
  return globals(9);
}

function host_params() {
  return globals(10);
}

function host_stdout() {
  return globals(18);
}
