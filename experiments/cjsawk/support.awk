function mks(s) {
  print "mks not impl"
  exit 1
}

function mkC(s) {
  print "mkC not impl"
  exit 1
}

function add(a,b) {
  a = OR(a, 0);
  b = OR(b, 0);
  return a + b;
}

function SUB(a, b) {
  print "SUB not impl"
  exit 1
}

function neq(a, b) {
  print "neq not impl"
  exit 1
}

function uint32_to_int32(x){
  x=int(x);
  if(x>2147483647) {
    x=x-4294967296;
  }
  return x;
}

function to_uint32(x) {
#  print("to_uint32 x: " x);
  x=int(x);
  if((x<0)){
    if(x<-2147483648) {
      print("to_uint32 less than -2147483648 not supported");
      exit 1;
    }
    x=4294967296+x;
#    print("to_uint32 x: " _or(x,0));
  }
  if(x>4294967295) {
    print "to_uint32 too big";
  }
  return x;
}

function do_bitwise(a, b, tt \
, ba \
, bb \
, i \
, r \
, t \
, v) {
  r=0;
#  print "a: " a;
#  print "b: " b;
  for(i=0;i<32;i++){
    r=r*0.5;
    ba=(a % 2);
    bb=(b % 2);
    t = ba+bb;
#    print "t: " t;
    v=tt[t];
#    print ba " " bb " " v;
    a=a-ba; a=a * 0.5;
    b=b-bb; b=b * 0.5;
    r=r+(v*2147483648);
  }
#  print "r: " r;
  return uint32_to_int32(r);
}

function and(a, b) {
  print "and not impl"
  exit 1
}

function fast_or(a, b) {
  print "fast_or not impl"
  exit 1
}

function init_or_tt(){
  or_tt[0]=0;
  or_tt[1]=1;
  or_tt[2]=1;
}

function slow_or(a,b \
, r) {
  a=to_uint32(a);
  b=to_uint32(b);

  r=do_bitwise(a, b, or_tt);
#  print "r: " r;
  return r;
}

function OR(a, b) {
  if(use_fast_or){
    return fast_or(a,b);
  } else {
    return slow_or(a,b);
  }
}

function or(a, b) {
  return OR(a, b);
}

function shl(a, b) {
  print "shl not impl"
  exit 1
}

function shr(a, b) {
  print "shr not impl"
  exit 1
}

function gt(a, b) {
  print "gt not impl"
  exit 1
}

function gte(a, b) {
  print "gte not impl"
  exit 1
}

function lt(a, b) {
  print "lt not impl"
  exit 1
}

function lte(a, b) {
  print "lte not impl"
  exit 1
}

function eq(a, b){
  a = OR(a, 0);
  b = OR(b, 0);
  return OR((a == b), 0);
}

function mul(a, b) {
  a = OR(a,0);
  b = OR(b,0);
  return a*b;
}

function div(a, b) {
  print "div not impl"
  exit 1
}

function mod(a, b) {
  print "mod not impl"
  exit 1
}

function wi8(a, b) {
  print "wi8 not impl"
  exit 1
}

function ri8(a) {
  print "ri8 not impl"
  exit 1
}

function v_brk(addr) {
  addr = or(addr,0);
  if(addr==0){
    return brk_ptr;
  } else {
    brk_ptr = addr;
    return addr;
  }
}

function v_malloc(size \
, old_malloc) {
#  print "malloc: " size;
  if(eq(NULL, _brk_ptr)) {
    _brk_ptr = v_brk(0);
    _malloc_ptr = _brk_ptr;
  }

  if(lt(_brk_ptr, add(_malloc_ptr, size))) {
    _brk_ptr = v_brk(add(_malloc_ptr, size));
    if(eq(SUB(0,1), _brk_ptr)) {
      return 0;
    }
  }

  old_malloc = _malloc_ptr;
  _malloc_ptr = add(_malloc_ptr, size);
#  print("malloc old_malloc: " old_malloc);
  return old_malloc;
}

function v_memset(ptr, value, num \
, s) {
  s = ptr;
#  print "memset ptr: " ptr " value: " value " num: " num;
  while(lt(0, num)) {
#    print "num: " num;
    wi8(s, value);
    s = add(s, 1);
    num = SUB(num, 1);
  }
}

function v_calloc(nmemb, size \
, ret) {
#  print "calloc nmemb: " nmemb " size: " size;
  ret = v_malloc(mul(nmemb, size));
  if(eq(NULL, ret)) {
    return NULL;
  }
  v_memset(ret, 0, mul(nmemb, size));
  return ret;
}

function v_free(a) {
  print "v_free not impl"
  exit 1
}

function v_exit(a) {
  print "v_exit not impl"
  exit 1
}

function v_fclose(a) {
  print "v_fclose not impl"
  exit 1
}

function v_fopen(a, b) {
  print "v_fopen not impl"
  exit 1
}

function v_fgetc(a) {
  print "v_fgetc not impl"
  exit 1
}

function v_fputs(a, b) {
  print "v_fputs not impl"
  exit 1
}

function init_support() {
  print "init_support dummy"
}

function init_runtime() {
  print "init_runtime";
  stdin = 0;
  stdout = 1;
  stderr = 2;

  NULL = 0;
  FALSE = 0;
  TRUE = 1;
  init_or_tt();
#  init_and_tt();
#  init_mkc();
  brk_ptr = 128*1024;
  in_file_num = 5;
  out_file_num = 6;

#  init_fast_or();
#  init_fast_and();
}
