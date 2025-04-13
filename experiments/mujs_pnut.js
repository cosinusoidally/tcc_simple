function mkc(c) {
  return rm8(str_adr(c));
}

function mks(s){
  var i;
  var r;
  var len;
  var p;
  len = str_len(s);
  p = str_adr(s);
  r = malloc(add(len, 1));
  for(i =0; lt(i, len); i = add(i, 1)){
    wi8(add(r, i), rm8(add(p, i)));
  }
  wi8(add(r, i), 0);
  return r;
}

function ffi_wrap(a0,a1,a2,a3,a4,a5,a6,a7,a8) {
  if(is_string(a1)) { a1 = str_adr(a1); }
  if(is_string(a2)) { a2 = str_adr(a2); }
  if(is_string(a3)) { a3 = str_adr(a3); }
  if(is_string(a4)) { a4 = str_adr(a4); }
  if(is_string(a5)) { a5 = str_adr(a5); }
  if(is_string(a6)) { a6 = str_adr(a6); }
  if(is_string(a7)) { a7 = str_adr(a7); }
  if(is_string(a8)) { a8 = str_adr(a8); }

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

load("support.js");
load("pnut.js");

/* override with builtins for memory read-write */
ri32 = _ri32;
wi32 = _wi32;

argc = 4;
argv = malloc(mul(argc, mul(4,argc)));
wi32(argv, mks("./artifacts/M2_simple_asm_m2.exe"));
wi32(add(argv, 4), mks("./pnut.c"));
wi32(add(argv, 8), mks("-o"));
wi32(add(argv, 12), mks("../artifacts/out.exe"));

main(argc, argv);
