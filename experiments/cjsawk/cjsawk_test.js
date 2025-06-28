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

function string_escape(s) {
  var c;
  var t;
  t = ra_new();
  while(1) {
    c = ri8(s);
    if(eq(0,c)) {
      break;
    }
    s = add(s, 1);
  }
  return ra_data_g(t);
}

function HACK_string_escape(s) {
  /* FIXME remove this hack */
  if(dbg) {
    print("string_escape: "+mk_js_string(s));
    print("string_escape2:"+mk_js_string(string_escape(s)));
  }
  return mks('"' + JSON.parse("["+mk_js_string(s)+"]")[0]);
}

load("cjsawk.js");

init_support();
main();
print(gen_out());
// print(brk_ptr);
// print(globals_list.len);
// print(hold_string.capacity);
