var a; /* this is a comment */
var c;

var d;

function foo(arg1, arg2) {
  var b; /* multiline
            comment
            this is multiline */
  var l2;
  a = 24;
  b = add(1,a);
  c = mkc('c');
  d = mks("hello world");
  arg1 = 7;
  arg2 = 1234;
  return b;
}

function test2() {
  return 1;
}

function test3() {
  var woo;
  var var2;
  woo=8;
  var2=37;
}
