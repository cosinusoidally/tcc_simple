NULL = 0;
FALSE = false;
TRUE = true;

load("support_libc.js");
load("support_alu.js");

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

function vfs_open(pathname) {
  var t = [];
  var t1;
  if(pathname==="vfs://dummy.c") {
    t1 = read("dummy_head.c", "binary");
    for(var i=0;i<t1.length;i++) {
      t.push(t1[i]);
    }

    t1 = read("simple_support_js.c", "binary");
    for(var i=0;i<t1.length;i++) {
      t.push(t1[i]);
    }

    t1 = read("m2min_v3.js", "binary");
    for(var i=0;i<t1.length;i++) {
      t.push(t1[i]);
    }

    t1 = read("simple_support_js_m2.js", "binary");
    for(var i=0;i<t1.length;i++) {
      t.push(t1[i]);
    }

    t1 = read("simple_support_js_m2_prims.c", "binary");
    for(var i=0;i<t1.length;i++) {
      t.push(t1[i]);
    }

    return t;
  } else {
    print("unsupported virtual file");
    err();
  }
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
      if(pathname.split(":")[0]==="vfs") {
        in_file = [vfs_open(pathname), 0];
      } else {
        in_file=[read(pathname, "binary"), 0];
      }
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
  print("exit: "+value + " ln: "+ln);
  err();
}

function mkc(c) {
  return c.charCodeAt(0) & 0xFF;
}

function mkC(s) {
  return mkc(s[0]);
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

var heap_size=16*1024*1024;
var heap=new Uint8Array(heap_size);

function wi8(o,v){
  if(v===undefined){
    print("wrong use of wi8");
    err();
  }
  heap[o]=v & 0xFF;
};

function ri8(o,dummy){
  if(dummy!==undefined){
    print("wrong use of ri8");
    err();
  }
  return heap[o] & 0xFF;
};

function gen_out(){
  if(out_file[out_file.length-1]=== mkc("\n")){
   out_file.pop();
  }
  return out_file.map(function(x){return String.fromCharCode(x)}).join("");
}
