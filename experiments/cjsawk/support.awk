function mks(s \
, i \
, l \
, cs \
, r) {
  r = string_cache[s];
  if(r) {
    return r;
  }
  l = length(s);
  r = v_malloc(l+1);
  split(s, cs, "");
#  print "mks s: " s " length: " length(s);

  for(i =0; i<l; i++){
    wi8(r + i, mkC(cs[i+1]));
  }
  string_cache[s]=r;
  return r;
}

function mkC(a \
, c) {
  c=mkc_table[a];
  if(c==""){
    print("mkc char not defined: "a);
    exit 1;
  }
#  print("mkc: " a " charcode: " c);
  return c;
}

function join(a,j \
, i \
, res \
) {
  i=1;
  if(j!="") {
    print "can only joing with the empty string"
    exit 1;
  }
  while(a[i]!=""){
    # FIXME this is quadratic probably not an issue but not sure if can fix in
    # awk
    res = res a[i];
    i=i+1;
  }
  print "joined to: " res;
  return res;
}

function append_in_line(l \
, chars \
, i \
){
  i=1;
  split(l, chars,"");
  while(chars[i]!=""){
    in_data[in_off++]=mkC(chars[i]);
    i=i+1;
  }
  in_data[in_off++]=mkC("\n");
#  print "\n";
}

function mk_awk_str(s \
, reta \
, i \
, ret) {
  i=1;
  while(heap[s]!=0) {
    reta[i]=charcode_to_str(ri8(s));
    i=i+1;
    s=s+1;
  }
  ret=join(reta,"");
  return ret;
}

function add(a,b) {
  a = OR(a, 0);
  b = OR(b, 0);
  return a + b;
}

function SUB(a, b) {
  a = OR(a, 0);
  b = OR(b, 0);
  return a - b;
}

function neq(a,b,c){
  a = OR(a, 0);
  b = OR(b, 0);
  return OR((a != b), 0);
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

function init_fast_and(){
  if(use_fast_and) {
    print "init fast and";
    for(a=0;a<256;a=a+1) {
      for(b=0;b<256;b=b+1) {
        and_cache[(256*a)+b]=slow_and(a,b);
      }
    }
  }
}

function cache_and(a,b) {
  return and_cache[(256*a+b)];
}

function fast_and(a,b \
, t1, t2, r, v) {
  and_count++;

  v=1/256;
  a=to_uint32(a);
  b=to_uint32(b);

# fast path for single byte and
  if((a < 256) && (b < 256)) {
    return cache_and(a,b);
  }

  t1 = a % 256;
  t2 = b % 256;
  r = cache_and(t1,t2);

  a=(a-t1)*v;
  b=(b-t2)*v;

  t1 = a % 256;
  t2 = b % 256;
  r = r + 256*cache_and(t1,t2);

  a=(a-t1)*v;
  b=(b-t2)*v;

  t1 = a % 256;
  t2 = b % 256;
  r = r + 65536*cache_and(t1,t2);

  a=(a-t1)*v;
  b=(b-t2)*v;

  t1 = a % 256;
  t2 = b % 256;
  r = r + 16777216*cache_and(t1,t2);

  return uint32_to_int32(r);
}

function init_and_tt(){
  and_tt[0]=0;
  and_tt[1]=0;
  and_tt[2]=1;
}

function slow_and(a,b \
, r) {
  a=to_uint32(a);
  b=to_uint32(b);

  r=do_bitwise(a, b, and_tt);
#  print "r: " r;
  return r;
}

function AND(a, b) {
  if(use_fast_and){
    return fast_and(a,b);
  } else {
    return slow_and(a,b);
  }
}

function init_fast_or(){
  if(use_fast_or) {
    print "init fast or";
    for(a=0;a<256;a=a+1) {
      for(b=0;b<256;b=b+1) {
        or_cache[(256*a)+b]=slow_or(a,b);
      }
    }
  }
}

function cache_or(a,b) {
  return or_cache[(256*a+b)];
}

function fast_or(a,b \
, t1, t2, r, v) {
  or_count++;

  v=1/256;
  a=to_uint32(a);
  b=to_uint32(b);

# fast path for single byte or
  if((a < 256) && (b < 256)) {
    return cache_or(a,b);
  }

  t1 = a % 256;
  t2 = b % 256;
  r = cache_or(t1,t2);

  a=(a-t1)*v;
  b=(b-t2)*v;

  t1 = a % 256;
  t2 = b % 256;
  r = r + 256*cache_or(t1,t2);

  a=(a-t1)*v;
  b=(b-t2)*v;

  t1 = a % 256;
  t2 = b % 256;
  r = r + 65536*cache_or(t1,t2);

  a=(a-t1)*v;
  b=(b-t2)*v;

  t1 = a % 256;
  t2 = b % 256;
  r = r + 16777216*cache_or(t1,t2);

  return uint32_to_int32(r);
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

function pow(a, b,    r) {
  if(b == 0) {
    return 1;
  }
  r = 1;
  while(b > 0) {
    r = r * a;
    b = b - 1;
  }
# Math disabled builds of busybox awk don't support power
#  return a ^ b;
#  print "ret a: " a " b: " b " r: " r
  return r;
}

function shl(a,b \
, p \
, ret) {
  a = to_uint32(a);
  if(b<0){
    print("shl cannot be a negative shift");
  }
  b = to_uint32(b);
#  p = 2 ^ b;
  p = pow(2, b);
  ret = AND(a*p,4294967295);
#  print "shl a: " a " b: " b " multiplier: " p " ret: " ret;
  return ret;
}

function shr(a,b \
, p \
, ret){
  a = to_uint32(a);
  if(b<0){
    print("shr cannot be a negative shift");
  }
  b = to_uint32(b);
#  p = 2 ^ b;
  p = pow(2, b);
  ret=int(a/p);
#  print "shr a: " a " b: " b " multiplier: " p " ret: " ret;
  return ret;
}

function gt(a,b,c){
  a = OR(a, 0);
  b = OR(b, 0);
  return OR((a > b), 0);
}

function gte(a, b) {
  a = OR(a, 0);
  b = OR(b, 0);
  return OR((a >= b), 0);
}

function lt(a,b) {
  a = OR(a, 0);
  b = OR(b, 0);
  return OR((a < b), 0);
}

function lte(a, b) {
  a = OR(a, 0);
  b = OR(b, 0);
  return OR((a <= b), 0);
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
  a = OR(a, 0);
  b = OR(b, 0);
  return OR((a / b), 0);
}

function mod(a, b) {
  a = OR(a, 0);
  b = OR(b, 0);
  return a % b;
}

function wi8(o,v){
  if(v==""){
    print("wrong use of wi8");
    exit 1;
  }
  heap[o] = AND(v, 255);
}

function ri8(o, dummy){
  if(dummy!=""){
    print("wrong use of ri8");
    exit 1;
  }
  return AND(heap[o], 255);
}

function v_brk(addr) {
  addr = OR(addr,0);
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
# noop
  return 0;
}

function v_exit(a) {
  print "v_exit not impl"
  exit 1
}

function v_close(fd) {
  print("close(" fd ")");
  return 0;
}

function v_fclose(stream \
, err) {
        err = v_close(stream);
        return err;
}

function v_open(pathname, flags, mode \
, myline \
) {
  pathname = mk_awk_str(pathname);
  print("open name: " pathname " flag: " flags " mode: " mode);

  if((flags == 0 ) && (mode == 0)){
    print "open for read";
    if(in_file == "") {
      print "opening in_file";
      while((getline < pathname)) {
        append_in_line($0);
      }
      in_len=in_off;
      in_off=0;
#      print_infile();
      return in_file_num;
    } else {
      print("ERROR in_file already loaded");
      exit 1;
    }
  } else if((flags == 577 ) && (mode == 384)){
    print "opening for write";
    out_filename = pathname;
    return out_file_num;
  } else {
    print "open unsuported mode";
  }
  exit 1;
}

function v_fopen(filename, mode \
, filename_s \
, mode_s \
, f) {
  filename_s=mk_awk_str(filename);
  mode_s=mk_awk_str(mode);
  print("fopen filename: " filename_s " mode: " mode_s);

  if(eq(mkC("w"), ri8(mode))) {
    # 577 is O_WRONLY|O_CREAT|O_TRUNC, 384 is 600 in octal
    f = v_open(filename, 577 , 384);
  } else {
    # Everything else is a read
    f = v_open(filename, 0, 0);
  }

  # Negative numbers are error codes */
  if(gt(0, f)) {
    return 0;
  }
  return f;

  print "fopen not impl"
  exit
}

function v_fgetc(f \
, eax) {
  if(f == in_file_num) {
    if(in_off < in_len) {
      eax = in_data[in_off];
#      if(dbg) {
#        print("fgetc: "+String.fromCharCode(eax));
#      }
#      in_file[1]=in_file[1]+1;

#      print "fgetc f:" f " c: " eax " is: " charcode_to_str(eax);
      in_off=in_off+1;
      if(eax == 10) {
        print "fgetc newline: " (my_line_num++);
      }
    } else {
#      if(dbg) {
#        print("fgetc: EOF");
#      }
      eax = -1;
      print "here2 " f;
    }
  } else {
    print("fgetc wrong file descriptor");
    exit 1;
  }
  return eax;
}

function v_fputc(c, stream) {
#  print "fputc c: " c " stream: " stream;
  if(out_filename != "") {
#    print "fputc filename: " out_filename;
    printf("%c", c) >> out_filename;
  }
}

function v_fputs(si, f) {
  while(neq(0, ri8(si))) {
    v_fputc(ri8(si), f);
    si = add(si, 1);
  }
}

function init_support() {
  print "init_support dummy"
}

function init_mkC( \
i, \
c, \
t) {
  print("init_mkc");
  for(i=0;i<256;i++){
    t=sprintf("%c",i);
    mkc_table[t]=i;
  }
  mkc_table["\'"]=39;
  for(i in mkc_table){
    c=mkc_table[i];
    charcode_to_str_arr[c] = i;
  }
}

function charcode_to_str(a \
, c){
  c = charcode_to_str_arr[a];
  if(c!="") {
    return c;
  }
  print "charcode_to_str invalid: "+a;
  exit 1;
}

function mk_args(si \
, i \
, s \
, ret \
){
  i = 1;
  print "mk_args si: " si;
  split(si, s, " ");
  while(s[i]!="") {
    print i " : " s[i];
    argva[i-1]=s[i];
    argc = i;
    i=i+1;
  }
  print "argc : " argc;
  argv = v_malloc(argc * 4);
  for(i = 0; i < argc ; i++){
    wi32(argv+(4*i), mks(s[i+1]));
  }
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
  init_and_tt();
  init_mkC();
  brk_ptr = 128*1024;
  in_file_num = 5;
  out_file_num = 6;

  init_fast_or();
  init_fast_and();
}
