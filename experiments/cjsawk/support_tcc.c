function mkc(c) {
  return c;
}

function mks(s) {
  return s;
}

function ri8(o) {
  puts("ri8 not impl");
  error();
}

function wi8(o, v) {
  puts("wi8 not impl");
  error();
}

function v_fopen(a, b) {
 return fopen(a, b);
}

function v_fgetc(f) {
  return fgetc(f);
}

function v_fputs(s, f) {
  return fputs(s, f);
}

function v_fclose(f) {
  return fclose(f);
}

function v_calloc(a, b) {
  return calloc(a, b);
}

function v_free(a) {
  free(a);
}

function v_exit(v) {
  exit(v);
}

function init_support() {
  puts("support_runtime not impl");
  error();
}

function init_runtime() {
  puts("init_runtime not impl");
  error();
}
