function mkc(c) {
  return c;
}

function mks(s) {
  return s;
}

int ri8(int o) {
        char *h = 0;
        return h[o] & 0xFF;
}

int wi8(int o,int v) {
        char *h = 0;
        h[o]=v;
        return;
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
  puts("support_runtime called");
}

extern int d2;

function init_runtime() {
  d2=0;
  puts("init_runtime called");
}
