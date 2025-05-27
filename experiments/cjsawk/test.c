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
  return b;
}

function test2() {
  return 1;
}
