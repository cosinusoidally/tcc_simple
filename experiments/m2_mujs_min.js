function mkc(c) {
  return c.charCodeAt(0) & 0xFF;
}

function mks(s){
  var r;
  r = malloc(s.length + 1);
  for(var i =0; i<s.length; i++){
    wi8(r + i, s.charCodeAt(i));
  }
  return r;
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

function dlopen(x,y) {
  return ffi(_dlopen, str_adr(x), y);
}

function dlsym(x,y) {
  return ffi(_dlsym, x, str_adr(y));
}

libc=dlopen("libc.so.6", RTLD_LAZY);

libc_open_ptr=dlsym(libc, "open");

function libc_open(pathname, flags, mode) {
  var f = ffi_wrap(libc_open_ptr, pathname, flags, mode);
  if(and(eq(flags, 0), eq(mode, 0))) {
    mode = "rb";
  } else if(and(eq(flags, 577), eq(mode, 384))) {
    mode = "wb";
  } else {
    print("invalid mode");
    exit(1);
  }
  f=libc_fdopen(f, mode);
  return f;
}

libc_fdopen_ptr=dlsym(libc, "fdopen");

function libc_fdopen(fd, mode) {
  return ffi_wrap(libc_fdopen_ptr, fd, mode);
}


libc_fgetc_ptr=dlsym(libc, "fgetc");

function libc_fgetc(f) {
  var r = ffi_wrap(libc_fgetc_ptr, f);
  return r;
}

libc_fputc_ptr=dlsym(libc, "fputc");

function libc_fputc(c, f) {
  return ffi_wrap(libc_fputc_ptr, c, f);
}

libc_fclose_ptr=dlsym(libc, "fclose");

/* note we are calling it close rather than fclose */
function libc_close(f) {
  return ffi_wrap(libc_fclose_ptr, f);
}

var brk_ptr;

brk_ptr = mul(128, 1024);

function brk(addr) {
  addr = or(addr,0);
  if(eq(addr, 0)){
    return brk_ptr;
  } else {
    brk_ptr = addr;
    return addr;
  }
}

function exit(value) {
  err();
}

function real_addr(o) {
  return add(heap, o);
}

function open(pathname, flags, mode) {
  pathname = real_addr(pathname);
  return libc_open(pathname, flags, mode);
}

close = libc_close;
fgetc = libc_fgetc;
fputc = libc_fputc;

load("simple_support_js_m2.js");
load("M2_simple_asm.js");

/* override with builtins for memory read-write */
ri32 = _ri32;
wi32 = _wi32;

argc = 3;
argv = malloc(mul(argc, 4));
wi32(argv, mks("./artifacts/M2_simple_asm_m2.exe"));
wi32(add(argv, 4), mks("./artifacts/M2_simple_asm_m2.c"));
wi32(add(argv, 8), mks("../../tcc_simple/experiments/artifacts/M2_simple_asm_orig.M1"));

main(argc, argv);
