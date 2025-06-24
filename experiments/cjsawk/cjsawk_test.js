// f=read("test.c","binary");
// f=read("../m2min_v3/m2min_v3.js","binary");
f=read("./artifacts/m2min_v3.c","binary");

var dbg;

if(dbg!=true) {
  dbg = false;
}

if(dbg == true) {
  dprint = print;
} else {
  dprint = function (){};
}

function mks(s) {
  // dummy impl
  return s;
}

load("cjsawk.js");

main();
