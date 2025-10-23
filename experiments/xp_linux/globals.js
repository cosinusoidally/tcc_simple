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

function host_stdout_addr() {
  return globals(18);
}

function host_stdout() {
  return ri32(host_stdout_addr());
}

function syscall_hook() {
  return globals(19);
}

function reloc_entrypoint_ptr() {
/*  return globals(20); */
  return 0x4020050; /* hard coded for now, see enter_reloc asm */
}

function get_reg(x) {
  return ri32(add(regs_data(), mul(x,4)));
}

function set_param(i, v) {
  wi32(add(host_params(), mul(i, 4)), v);
}
