f=read("test.c","binary");

to=0;
ts=0;
te=0;
ch=f[0];

ln=1;

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
      (ch>=c_a) && (ch<=c_z)) || (ch == c__)) {
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

function f_str() {
  var i;
  var b = [];
  for(i=ts; i < te ; i = i + 1) {
    b.push(f[i]);
  }
  print("f_str " +b.map(function(x){return String.fromCharCode(x)}).join(""));
}

function get_id() {
  ts = to - 1;
  while(is_num() || is_id()) {
    te = to;
    nc();
  }
  f_str();
}

function get_num() {
  ts = to - 1;
  while(is_num()) {
    te = to;
    nc();
  }
  f_str();
}

function get_other() {
  ts = to - 1;
  te = to;
  nc();
  f_str();
}

function get_char() {
  ts = to - 1;
  nc();
  nc();
  te = to;
  nc();
  f_str();
}

nc();

while(to < f.length) {
  eat_whitespace();
  eat_comment();
  if(is_id()) {
    get_id();
  }
  if(is_num()) {
    get_num();
  }
  if(is_other()) {
    get_other();
  }
  if(is_char()) {
    get_char();
  }
/*
  print(ln+ " " + ch + " " +String.fromCharCode(ch));
  nc();
*/
}
