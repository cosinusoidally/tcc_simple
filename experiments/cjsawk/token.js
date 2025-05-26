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
var c_A;
var c_Z;
var c__;

function is_id() {
  if((ch>=c_a) && (ch<=c_z)) {
    return true;
  }
  return false;
}

function is_num() {
  return false;
}

function get_id() {

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
