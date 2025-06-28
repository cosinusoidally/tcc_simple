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
  var c2;
  var t;
  t = ra_new();
  ra_push8(t, mkc('"'));
  s = add(s,1);
  while(1) {
    c = ri8(s);
    if(eq(mkc('"'),c)) {
      break;
    }
    if(eq(c, mkc('\\'))) {
      c2 = escape_lookup(s);
      if(neq(c, c2)) {
        s = add(s, 1);
        c = c2;
      } else {
      }
    }
    ra_push8(t, c);
    s = add(s, 1);
  }
  return ra_data_g(t);
}

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

load("cjsawk.js");

init_support();
main();
print(gen_out());
// print(brk_ptr);
// print(globals_list.len);
// print(hold_string.capacity);
