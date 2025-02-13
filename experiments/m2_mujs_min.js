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

libc_open_ptr=dlsym(libc, "open");

function libc_open(pathname, flags, mode) {
  pathname = mk_js_string(pathname);
  var f = ffi_wrap(libc_open_ptr, pathname, flags, mode);
  if((flags == 0) && (mode == 0)) {
    mode = "rb";
  } else if((flags == 577) && (mode == 384)) {
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

/* HACK split this read function off into a separate mujs specific file */

_read = read;

read = function(x, y) {
  var t;
  var f;
  var i;
  t = _read(x);
  if(y==="binary") {
    f = [];
    for(i = 0; i<t.length;i++) {
      f.push(t.charCodeAt(i));
    }
  } else {
    f = t;
  }
  return f;
}


load("simple_support_js_m2.js");
load("M2_simple_asm.js");

/* override with builtins for memory read-write */
ri32 = _ri32;
wi32 = _wi32;

var dbg;

function fgetc(f) {
  var eax;
  if(f === in_file_num) {
    if(in_file[1] < in_file[0].length) {
      eax = in_file[0][in_file[1]];
      if(dbg) {
        print("fgetc: "+String.fromCharCode(eax));
      }
      in_file[1]=in_file[1]+1;
    } else {
      if(dbg) {
        print("fgetc: EOF");
      }
      eax = -1;
    }
  } else {
    print("fgetc wrong file descriptor");
    throw "fgetc";
  }
  return eax;
}

function fputc(c, stream) {
//  print("fputc(" +c+", "+stream+")");
  out_file.push(c);
}

var in_file;
in_file_num = 5;

var out_file;
out_file_num = 6;

function open(pathname, flags, mode) {
  pathname = mk_js_string(pathname);
  if(dbg) {
    print("open name:" + pathname + " flag: "+flags+" mode: "+mode);
  }
  if((flags ===0 ) && (mode === 0)){
    if(in_file === undefined) {
      in_file=[read(pathname, "binary"), 0];
      return in_file_num;
    } else {
      print("in_file already loaded");
      throw "open";
    }
  } else if((flags === 577 ) && (mode === 384)){
    if(out_file === undefined) {
      out_file = [];
      return out_file_num;
    } else {
      print("out_file already loaded");
      throw "open";
    }
  } else {
    throw "open";
  }
  err();
}

function close(fd) {
  if(dbg) {
    print("close("+fd+")");
  }
  return 0;
}

var brk_ptr = 128*1024;

function brk(addr) {
  addr = addr |0;
  if(addr===0){
    return brk_ptr;
  } else {
    brk_ptr = addr;
    return addr;
  }
}

function exit(value) {
  err();
}

function mkc(c) {
  return c.charCodeAt(0) & 0xFF;
}

function mk_c_string(s){
  var r;
  r = malloc(s.length + 1);
  for(var i =0; i<s.length; i++){
    wi8(r + i, s.charCodeAt(i));
  }
  return r;
}

function mk_js_string(o){
  var c;
  var s = [];
  var i=0;
  while((c=ri8(o+i)) !==0){
    s.push(String.fromCharCode(c));
    i=i+1;
  }
  return s.join("");
}

mks = mk_c_string;


function mk_args(s){
  var argc;
  s=s.split(" ");
  argc = s.length;
  argv = malloc(argc * 4);
  for(var i = 0; i < argc ; i++){
    wi32(argv+(4*i), mk_c_string(s[i]));
  }
  return [argc, argv];
}

function gen_out(){
  if(out_file[out_file.length-1]=== mkc("\n")){
   out_file.pop();
  }
  return out_file.map(function(x){return String.fromCharCode(x)}).join("");
}

open = libc_open;
close = libc_close;
fgetc = libc_fgetc;
fputc = libc_fputc;

try {
  argc_argv = mk_args("./artifacts/M2_simple_asm_m2.exe ./artifacts/M2_simple_asm_m2.c ../../tcc_simple/experiments/artifacts/M2_simple_asm_orig.M1")

  argv = argc_argv[1];
  argc = argc_argv[0];
  main(argc, argv);
  print(gen_out());
} catch (e){
  print(e.stack);
  print(e.message);
}
