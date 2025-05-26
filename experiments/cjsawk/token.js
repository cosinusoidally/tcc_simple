f=read("test.c","binary");

to=0;
ts=0;
te=0;
ch=f[0];

ln=1;

function nc() {
  ch=f[to];
  to=to+1;
}

function is_whitespace() {
  if((ch == 32) || (ch == 9)) {
    return true;
  }

  if(ch == 10) {
    ln = ln + 1;
    return true;
  }

  return false;
}

function eat_whitespace() {
  while(is_whitespace()) {
    nc();
  }
}

var c_a = "a".charCodeAt(0);
var c_z = "z".charCodeAt(0);
var c_A = "A".charCodeAt(0);
var c_Z = "Z".charCodeAt(0);
var c__ = "_".charCodeAt(0);

function is_id() {
  if(((ch>=c_a) && (ch<=c_z) ||
      (ch>=c_a) && (ch<=c_z)) || (ch == c__)) {
    return true;
  }
  return false;
}

function is_num() {
  return false;
}

function f_str() {
  var i;
  for(i=ts; i < te ; i = i + 1) {
    print("f_str" +" "+ f[i]);
  }
}

function get_id() {
  ts = to - 1;
  while(is_num() || is_id()) {
    te = to;
    nc();
  }
  f_str();
}

nc();

while(to < f.length) {
  eat_whitespace();
  if(is_id()) {
    get_id();
  }
  if(is_num()) {
    get_num();
  }
  print(ln+ " " + ch + " " +String.fromCharCode(ch));
  nc();
}
