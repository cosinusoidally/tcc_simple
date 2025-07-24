function mkc(c) {
  return c;
}

function mkC(s) {
  return ri8(s);
}

function mks(s) {
  return s;
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

/* wrapper for sub since sub is a builtin word in AWK */
function SUB(a, b) {
  return sub(a, b);
}

/* wrapper for and since sub is a builtin word in AWK */
function AND(a, b) {
  return and(a, b);
}

function init_runtime() {
/* dummy */
}
