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
  var f = ffi_wrap(libc_open_ptr, pathname, flags, mode);
  if((mode == 0) && (flags == 0)) {
    mode = "rb";
  } else {
    print("invalid mode");
    exit(1);
  }
  f=libc_fdopen(f, "rb");
  return f;
}

libc_fdopen_ptr=dlsym(libc, "fdopen");

function libc_fdopen(fd, mode) {
  return ffi_wrap(libc_fdopen_ptr, fd, mode);
}


libc_fgetc_ptr=dlsym(libc, "fgetc");

function libc_fgetc(f) {
  return ffi_wrap(libc_fgetc_ptr, f);
}

libc_fclose_ptr=dlsym(libc, "fclose");

/* note we are calling it close rather than fclose */
function libc_close(f) {
  return ffi_wrap(libc_fclose_ptr, f);
}

f=libc_open("README", 0, 0);
//f2=libc_open("artifacts/out.M1", 577, 384);

print("file: "+f);

while((c = libc_fgetc(f)) >= 0) {
  print("fgetc: "+ String.fromCharCode(c));
}

libc_close(f);
