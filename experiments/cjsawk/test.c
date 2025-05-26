var a; /* this is a comment */
var c;

var d;

function foo() {
  var b; /* multiline
            comment
            this is multiline */
  a = 24;
  b = add(1,a);
  c = mkc('c');
  d = mks("hello world");
  return b;
}
