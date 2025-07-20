function mks(s) {
  print "mks not impl"
  exit 1
}

function mkC(s) {
  print "mkC not impl"
  exit 1
}

function add(a, b) {
  print "add not impl"
  exit 1
}

function SUB(a, b) {
  print "SUB not impl"
  exit 1
}

function neq(a, b) {
  print "neq not impl"
  exit 1
}

function and(a, b) {
  print "and not impl"
  exit 1
}

function or(a, b) {
  print "or not impl"
  exit 1
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

function eq(a, b) {
  print "eq not impl"
  exit 1
}

function mul(a, b) {
  print "mul not impl"
  exit 1
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

function v_malloc(size \
, old_malloc) {
#  print "malloc: " size;
  if(eq(NULL, _brk_ptr)) {
    _brk_ptr = brk(0);
    _malloc_ptr = _brk_ptr;
  }

  if(lt(_brk_ptr, add(_malloc_ptr, size))) {
    _brk_ptr = brk(add(_malloc_ptr, size));
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
#  init_or_tt();
#  init_and_tt();
#  init_mkc();
  brk_ptr = 128*1024;
  in_file_num = 5;
  out_file_num = 6;

#  init_fast_or();
#  init_fast_and();
}
