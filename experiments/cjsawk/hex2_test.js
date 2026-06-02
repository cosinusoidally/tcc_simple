// f=read("test.c","binary");
// f=read("../m2min_v3/m2min_v3.js","binary");

var dbg;

if(dbg!=true) {
  dbg = false;
}

if(dbg == true) {
  dprint = print;
} else {
  dprint = function (){};
}

load("support.js");

function v_fgetc(f) {
  return fgetc(f);
}

function mks_alt(s) {
  // dummy impl
  return s;
}

/* not used, kept for reference */
function HACK_string_escape(s) {
  var r;
  /* FIXME remove this hack */
  if(dbg) {
    print("string_escape: "+mk_js_string(s));
    print("string_escape2:"+mk_js_string(string_escape(s)));
  }
  r = ('"' + JSON.parse("["+mk_js_string(s)+"]")[0]);
  if(r!==mk_js_string(string_escape(s))) {
    print("string_escape3:"+r);
    throw "string error";
  }
  return mks(r);
}

/* calloc wrapper */
function v_calloc(a,b){
  return calloc(a,b);
}

function v_free(a) {
  return free(a);
}

function v_fopen(a, b) {
  return fopen(a, b);
}

function v_fputs(a, b) {
  return fputs(a, b);
}

function v_exit(a) {
  exit(a);
}

function v_fclose(f) {
  return fclose(f);
}

function init_runtime() {
/* dummy */
}

load("hex2.js");

var fname;

function open2(pathname, flags, mode) {
  pathname = mk_js_string(pathname);
  if(dbg) {
    print("open name:" + pathname + " flag: "+flags+" mode: "+mode);
  }
  if((flags ===0 ) && (mode === 0)){
      if(pathname.split(":")[0]==="vfs") {
        in_file = [vfs_open(pathname), 0];
      } else {
        in_file=[read(pathname, "binary"), 0];
      }
      return in_file_num;
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

open = open2;

function chmod() {
  /* dummy */
}

var extra_args;

function go() {
  var args;
  if(!extra_args) {
    extra_args = "";
  }
  if(!fname) {
    fname="artifacts/builds/hex2_test/cjsawk3.exe-0.hex2";
  }
  args=mk_args("cjsawk.exe " + fname + " ./artifacts/blah.hex2" + extra_args);
  main(args[0], args[1]);
/* FIXME should not use nodejs specific code */
  print(new Buffer(out_file).toString("base64"));
//  print(gen_out());
// print(brk_ptr);
// print(globals_list.len);
// print(hold_string.capacity);
}

go();
