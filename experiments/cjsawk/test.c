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

function test4() {
  var a;
  a = 0;
  while(lt(a, 100)) {
    a = add(a,1);
  }
  while(lt(a, 200)) {
    a = add(a,1);
  }
  if(eq(a, 200)) {
    return 1;
  } else {
    return 2;
  }
}

function test5() {
  var foo;
  foo = 0;
  if(eq(foo, 200)) {
    return 1;
  } else if(lt(foo,200)){
    return 4;
  } else {
    return 2;
  }
}

function test6 () {

  while(1) {
    break;
  }

  while(1) {
    if(1) {
      break;
    }
    while(1) {
      if(2) {
        break;
      }
    }
    break;
  }
}

function test7() {
  var a;
  a = mkc('\\');
  a = mkc('\n');
  a = mkc('\'');
}

/* add funtion to test asm handling */
function add(a, b) {
/*      return a + b; */
        asm(
                "lea_eax,[ebp+DWORD] %-4"
                "mov_eax,[eax]"
                "push_eax"
                "lea_eax,[ebp+DWORD] %-8"
                "mov_eax,[eax]"
                "pop_ebx"
                "add_eax,ebx"
                "ret"
        );
}
