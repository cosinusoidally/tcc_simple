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

f=read("./artifacts/m2min_v3.c","binary");

v_o = 0;

f_len=f.length;

function v_getc() {
  var r;
  r=f[v_o];
  v_o=v_o + 1;
  return r;
}


function mks_alt(s) {
  // dummy impl
  return s;
}

function HACK_string_escape(s) {
  /* FIXME remove this hack */
  return mks('"' + JSON.parse("["+mk_js_string(s)+"]")[0]);
}

load("cjsawk.js");

init_support();
main();
print(gen_out());
// print(brk_ptr);
// print(globals_list.len);
// print(hold_string.capacity);
