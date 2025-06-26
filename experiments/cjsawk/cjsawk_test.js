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

load("cjsawk.js");

init_support();
main();
// print(brk_ptr);
// print(globals_list.len);
 print(hold_string.capacity);
