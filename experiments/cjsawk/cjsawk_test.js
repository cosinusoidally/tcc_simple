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


function mks_alt(s) {
  // dummy impl
  return s;
}

mks = function(s) {
  var r;
  /* round trip onto virtual c heap */
  r = mk_c_string(s);
  r = mk_js_string(r);
  return r;
}

mks_ = mk_c_string;
mks = mk_c_string;

load("cjsawk.js");

init_support();
main();
// print(brk_ptr);
// print(globals_list.len);
