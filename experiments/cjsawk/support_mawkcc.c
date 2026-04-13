var support_byte;

function mkc(c) {
  return c;
}

function mkC(s) {
  return ri8(s);
}

function init_support() {
  support_byte = brk(1);
  return 0;
}

function init_runtime() {
  return 0;
}

function streq(a, b) {
  return streq_(a, b, 0);
}

function streq_(a, b, i) {
  while(1) {
    if(ne(ri8(add(a, i)), ri8(add(b, i)))) {
      return 0;
    }
    if(eq(ri8(add(a, i)), 0)) {
      return 1;
    }
    i = add(i, 1);
  }
}

function strlen(s) {
  return strlen_(s, 0);
}

function strlen_(s, i) {
  while(ne(ri8(add(s, i)), 0)) {
    i = add(i, 1);
  }
  return i;
}

function v_fopen(name, mode) {
  if(streq(mode, mks("w"))) {
    return open(name, 577, 420);
  }
  return open(name, 0, 0);
}

function v_fgetc(fd) {
  if(ne(read(fd, support_byte, 1), 1)) {
    return sub(0, 1);
  }
  return ri8(support_byte);
}

function v_fputs(s, fd) {
  return write(fd, s, strlen(s));
}

function fputc(c, fd) {
  wi8(support_byte, c);
  return write(fd, support_byte, 1);
}

function v_fclose(fd) {
  return close(fd);
}

function v_calloc(count, size) {
  return v_calloc_(mul(count, size), 0, 0);
}

function v_calloc_(n, p, i) {
  p = brk(n);
  i = 0;
  while(lt(i, n)) {
    wi8(add(p, i), 0);
    i = add(i, 1);
  }
  return p;
}

function v_free(p) {
  return 0;
}

function v_exit(status) {
  return exit(status);
}

function chmod(path, mode) {
  return 0;
}
