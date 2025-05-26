var a; /* this is a comment */
var c;

function foo() {
  var b; /* multiline
            comment
            this is multiline */
  a = 24;
  b = add(1,a);
  c = mkc('c');
  return b;
}
