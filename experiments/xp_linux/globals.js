function args_base() {
  return 0x8045800;
}

function elf_base() {
  return 0x8048000;
}

function base_address() {
  return 0x4000000;
}

function file_descriptors() {
  return add(base_address(), add(0x20000, 0x200));
}

function sizeof_file_descriptor() {
  return 8;
}

function gfds() {
  return add(base_address(),0x01000010);
}

function sizeof_gfd() {
  return 8;
}

function filename_array() {
  return add(base_address(), 0x0200000);
}

function filename_size() {
  return 1024;
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
/*  return globals(9); */
  return 0x4020024; /* must hard code for now, see host_call asm */
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

function int2str_tmp() {
  return globals(21);
}

function brk_ptr() {
  return globals(40);
}

function next_fd() {
  return globals(41);
}

function next_filenum() {
  return globals(42);
}

function file_addr() {
  return globals(43);
}

function command_file() {
  return globals(44);
}

function char_buf() {
  return globals(45);
}

function get_reg(x) {
  return ri32(add(regs_data(), mul(x,4)));
}

function set_param(i, v) {
  wi32(add(host_params(), mul(i, 4)), v);
}

function gfd_get_file_length(filenum) {
  return ri32(add(gfds(), add(mul(filenum, sizeof_gfd()),4)));
}

function gfd_set_file_length(filenum, len) {
  wi32(add(gfds(), add(mul(filenum, sizeof_gfd()), 4)), len);
}

function gfd_get_file_addr(filenum) {
  return ri32(add(gfds(), mul(filenum, sizeof_gfd())));
}

function gfd_set_file_addr(filenum, addr) {
  wi32(add(gfds(), mul(filenum, sizeof_gfd())), addr);
}

function gfn_get_filename(fn) {
  return add(filename_array(), mul(fn, filename_size()));
}


function fd_set_filenum(fd, filenum) {
  wi32(add(file_descriptors(), mul(fd, sizeof_file_descriptor())), filenum);
}

function fd_get_filenum(fd) {
  return ri32(add(file_descriptors(), mul(fd, sizeof_file_descriptor())));
}

function fd_set_file_offset(fd, o) {
  wi32(add(file_descriptors(), add(mul(fd, sizeof_file_descriptor()), 4)), o);
}

function fd_get_file_offset(fd) {
  return ri32(add(file_descriptors(), add(mul(fd, sizeof_file_descriptor()),4)));
}
