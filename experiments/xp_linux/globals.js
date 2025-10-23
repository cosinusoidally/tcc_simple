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
