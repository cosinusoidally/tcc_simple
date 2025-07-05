// f=read("test.c","binary");
// f=read("../m2min_v3/m2min_v3.js","binary");

var dbg;

var fname;

if(dbg!=true) {
  dbg = false;
}

if(dbg == true) {
  dprint = print;
} else {
  dprint = function (){};
}

load("support.js");

if(fname === undefined) {
  fname = "./artifacts/m2min_v3.c";
}

f=read(fname, "binary");

v_o = 0;

f_len=f.length;

function v_getc() {
  var r;
  if(v_o >=f.length) {
    return -1;
  }
  r=f[v_o];
  v_o=v_o + 1;
  return r;
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

load("cjsawk.js");

function go() {
  var args;
  args=mk_args("cjsawk.exe ./artifacts/m2min_v3.c ./artifacts/blah.M1");
  main(args[0], args[1]);
  print(gen_out());
// print(brk_ptr);
// print(globals_list.len);
// print(hold_string.capacity);
}

go();
