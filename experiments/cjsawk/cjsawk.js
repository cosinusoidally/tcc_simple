f=read("test.c","binary");
// f=read("../m2min_v3/m2min_v3.js","binary");

to=0;
ts=0;
te=0;
ch=f[0];

ln=1;

var current_count;

var tt;

tt_identifier = "identifier";
tt_number = "number";
tt_char = "char";
tt_string = "string";
tt_other = "other";

var tok;

function nc() {
  ch=f[to];
  to=to+1;
  if(ch == 10) {
    ln = ln + 1;
  }
}

function is_whitespace() {
  if((ch == 32) || (ch == 9)) {
    return true;
  }

  if(ch == 10) {
    return true;
  }

  return false;
}

function eat_whitespace() {
  while(is_whitespace()) {
    nc();
  }
}

function is_comment() {
  if(ch == c_fs) {
    return true;
  }
  return false;
}

var c_fs = "//".charCodeAt(0);
var c_star = "*".charCodeAt(0);

function eat_comment() {
  if(ch == c_fs) {
    nc();
    if(ch == c_star) {
      nc();
    } else {
      return;
    }
    while(1) {
      while(ch != c_star) {
        nc();
      }
      nc();
      if(ch == c_fs) {
        nc();
        return;
      }
    }
  }
}

var c_a = "a".charCodeAt(0);
var c_z = "z".charCodeAt(0);
var c_A = "A".charCodeAt(0);
var c_Z = "Z".charCodeAt(0);
var c__ = "_".charCodeAt(0);
var c_0 = "0".charCodeAt(0);
var c_9 = "9".charCodeAt(0);

function is_id() {
  if(((ch>=c_a) && (ch<=c_z) ||
      (ch>=c_A) && (ch<=c_Z)) || (ch == c__)) {
    return true;
  }
  return false;
}

function is_num() {
  if((ch>=c_0) && (ch<=c_9)) {
    return true;
  }
  return false;
}


var c_comma = ",".charCodeAt(0);
var c_semi = ";".charCodeAt(0);
var c_lparen = "(".charCodeAt(0);
var c_rparen = ")".charCodeAt(0);
var c_lcurly = "{".charCodeAt(0);
var c_rcurly = "}".charCodeAt(0);
var c_eq = "=".charCodeAt(0);

function is_other() {
  if((ch==c_comma) || (ch==c_semi) || (ch==c_lparen) || (ch==c_rparen) ||
     (ch==c_lcurly) || (ch==c_rcurly) || (ch==c_eq)) {
    return true;
  }
  return false;
}

var c_ch = "'".charCodeAt(0);

function is_char() {
  if(ch==c_ch) {
    return true;
  }
  return false;
}

var c_str = "\"".charCodeAt(0);

function is_string() {
  if(ch==c_str) {
    return true;
  }
  return false;
}

function f_str() {
  var i;
  var b = [];
  for(i=ts; i < te ; i = i + 1) {
    b.push(f[i]);
  }
  b = b.map(function(x){return String.fromCharCode(x)}).join("");
  print("tt: " + tt + " f_str " + b);
  tok = b;
}

function get_id() {
  tt = tt_identifier;
  ts = to - 1;
  while(is_num() || is_id()) {
    te = to;
    nc();
  }
  f_str();
}

function get_num() {
  tt = tt_number;
  ts = to - 1;
  while(is_num()) {
    te = to;
    nc();
  }
  f_str();
}

function get_other() {
  tt = tt_other;
  ts = to - 1;
  te = to;
  nc();
  f_str();
}

function get_char() {
  tt = tt_char;
  ts = to - 1;
  nc();
  nc();
  te = to;
  nc();
  f_str();
}

function get_string() {
  tt = tt_string;
  ts = to - 1;
  nc();
  while(ch != c_str) {
    nc();
  }
  te = to;
  nc();
  f_str();
}


function nt() {
  if(to >= f.length) {
    return false;
  }

  while(is_whitespace() || is_comment()) {
    if(is_whitespace()) {
      eat_whitespace();
    } else if(is_comment()) {
      eat_comment();
    }
  }

  if(is_id()) {
    get_id();
  } else if(is_num()) {
    get_num();
  } else if(is_other()) {
    get_other();
  } else if(is_char()) {
    get_char();
  } else if(is_string()) {
    get_string();
  } else {
    print("unsupported char");
    print("line: " + ln + " char: " + ch + " " +String.fromCharCode(ch));
    throw "error";
  }
  return true;
}

function emit(s, l) {
  l.push(s);
}

globals_list = [];

function declare_global(t) {
  print("declare_global: " +t);
  emit(":GLOBAL_" , globals_list);
  emit(t, globals_list);
  emit("\nNULL\n", globals_list);
}

strings_list = [];

output_list = [];

function emit_out(s) {
  output_list.push(s);
}

function collect_arguments() {
  var args = [];
  nt();
  while(tok != ")") {
    nt();
  }
}

function declare_function(t) {
  print("declare_function: " +t);
  current_count = 0;
  collect_arguments();
  nt();
  if(tok == ";") {
    print("function_prototype skip");
  } else if(tok == "{") {
    print("function_body");
    emit_out(":FUNCTION_");
    emit_out(t);
    emit_out("\n");
  }
}

function program() {
  var ltok;

  nc();

  while(nt()) {
    nt(); ltok = tok;
    nt();
    if(tok == ";") {
      declare_global(ltok);
    } else if(tok == "(") {
      declare_function(ltok);
    }
  }
}

program();
print("\n# Core program");
print(output_list.join(""));
print("# Program global variables");
print(globals_list.join(""));
print("# Program strings");
print(strings_list.join(""));
print("ELF_end");
