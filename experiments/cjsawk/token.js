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

nc();

while(to < f.length) {
  eat_whitespace();
  print(ln+ " " + ch + " " +String.fromCharCode(ch));
  nc();
}
