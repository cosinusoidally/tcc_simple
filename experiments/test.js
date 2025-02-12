function dlopen(x,y) {
  return ffi(_dlopen, str_adr(x), y);
}

function dlsym(x,y) {
  return ffi(_dlsym, x, str_adr(y));
}

libc=dlopen("libc.so.6", RTLD_LAZY);

_puts=dlsym(libc, "puts");

print("got here");

ffi(_puts,str_adr("Hello world"));
