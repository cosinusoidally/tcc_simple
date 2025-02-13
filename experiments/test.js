function dlopen(x,y) {
  return ffi(_dlopen, str_adr(x), y);
}

function dlsym(x,y) {
  return ffi(_dlsym, x, str_adr(y));
}

function ffi_wrap(a0,a1,a2,a3,a4,a5,a6,a7,a8) {
  if(typeof a1 === "string") { a1 = str_adr(a1); }
  if(typeof a2 === "string") { a2 = str_adr(a2); }
  if(typeof a3 === "string") { a3 = str_adr(a3); }
  if(typeof a4 === "string") { a4 = str_adr(a4); }
  if(typeof a5 === "string") { a5 = str_adr(a5); }
  if(typeof a6 === "string") { a6 = str_adr(a6); }
  if(typeof a7 === "string") { a7 = str_adr(a7); }
  if(typeof a8 === "string") { a8 = str_adr(a8); }

  return ffi(a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

libc=dlopen("libc.so.6", RTLD_LAZY);

_puts=dlsym(libc, "puts");

function puts(x) {
  return ffi_wrap(_puts, x);
}

print("got here");

ffi(_puts,str_adr("Hello world"));


puts("Hello world again");

libc_open_ptr=dlsym(libc, "open");

function libc_open(pathname, flags, mode) {
  return ffi_wrap(libc_open_ptr, pathname, flags, mode);
}

/* note were are aliasing fgetc to getc since we are using open */
libc_fgetc_ptr=dlsym(libc, "fgetc");

function libc_fgetc(fd) {
  return ffi_wrap(libc_fgetc_ptr, fd);
}

f=libc_open("README", 0, 0);

print("file: "+f);

print("fgetc: "+ libc_fgetc(f));
