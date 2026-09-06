using System;

public class Program
{
  static bool LTRUE = true;
  static byte[] heap;
  static int brk_ptr;

  static void not_impl(string a) {
    Console.WriteLine("not_impl: "+a);
    Environment.Exit(1);
  }
  static int mks(string a) {
    int i;
    int p = malloc(a.Length+1);
    for(i=0; i<a.Length; i++) {
      wi8(p+i, char.ConvertToUtf32(a,i));
//      Console.WriteLine(i+":"+char.ConvertToUtf32(a,i));
    }
    wi8(p+i,0);
//    not_impl("mks");
    return p;
  }
  static int ri8(int a) {
    return heap[a];
  }
  static int wi8(int a, int b) {
    heap[a]=(byte)(b & 255);
    return 0;
  }

  static int OR(int a, int b) {
    return a | b;
  }
  static bool OR(bool a, bool b) {
    return a | b;
  }

  static int AND(int a, int b) { return a & b; }
  static bool AND(bool a, bool b) { return a & b; }

  static int shl(int a, int b) { return a << b; }
  static int shr(int a, int b) { return a >> b; }
  static int add(int a, int b) { return a + b; }
  static int SUB(int a, int b) { return a - b; }
  static int sub(int a, int b) { return SUB(a, b); }
  static int mul(int a, int b) { return a * b; }
  static int div(int a, int b) { return a / b; }
  static int mod(int a, int b) { return a % b; }

  static bool neq(int a, int b) { return a != b; }
  static bool lt(int a, int b) { return a < b; }

  static bool eq(int a, int b) { return a == b; }
  static bool eq(int a, bool b) { return Convert.ToBoolean(a) == b; }
  static bool gt(int a, int b) { return a > b; }
  static bool gte(int a, int b) { return a >= b; }
  static bool lte(int a, int b) { return a <= b; }

  static bool TO_BOOL(int a) { return Convert.ToBoolean(a); }

  static int v_fgetc(int a) {
    not_impl("v_fgetc");
    return 0;
  }
  static int v_fputs(int a, int b) {
    not_impl("v_fputs");
    return 0;
  }
  static int v_fopen(int a, int b) {
    return fopen(a, b);
  }
  static int v_fclose(int a) {
    not_impl("v_fclose");
    return 0;
  }

  static int v_calloc(int a, int b) {
    return calloc(a, b);
  }
  static int v_free(int a) {
    not_impl("v_free");
    return 0;
  }

  static int v_exit(int a) {
    not_impl("v_exit");
    return 0;
  }

  static int mkC(string a) {
    not_impl("mkC");
    return 0;
  }

  static int init_support() {
    Console.WriteLine("init_support called");
    return 0;
  }

  static int _malloc_ptr;
  static int _brk_ptr;

  static int malloc(int size) {
    int old_malloc;
    if(eq(NULL, _brk_ptr)) {
            _brk_ptr = brk(0);
            _malloc_ptr = _brk_ptr;
    }

    if(lt(_brk_ptr, add(_malloc_ptr, size))) {
            _brk_ptr = brk(add(_malloc_ptr, size));
            if(eq(sub(0,1), _brk_ptr)) return 0;
    }

    old_malloc = _malloc_ptr;
    _malloc_ptr = add(_malloc_ptr, size);
    return old_malloc;
  }

  static int memset(int ptr, int value, int num) {
    int s;
    s = ptr;
    while(lt(0, num)) {
      wi8(s, value);
      s = add(s, 1);
      num = sub(num, 1);
    }
    return 0;
  }

  static int calloc(int count, int size) {
    int ret;
    ret = malloc(mul(count, size));
    if(eq(NULL, ret)) {
            return NULL;
    }
    memset(ret, 0, mul(count, size));
    return ret;
  }

  static int brk(int addr) {
    if(addr==0){
      return brk_ptr;
    } else {
      brk_ptr = addr;
      return addr;
    }
  }

  static int fopen(int filename, int mode) {
    int f;
    if(eq(mkc('w'), ri8(mode))) {
      /* 577 is O_WRONLY|O_CREAT|O_TRUNC, 384 is 600 in octal */
      f = open(filename, 577 , 384);
    } else {
      /* Everything else is a read */
      f = open(filename, 0, 0);
    }

    /* Negative numbers are error codes */
    if(gt(0, f)) {
      return 0;
    }
    return f;
  }

  static int mkc(int c) {
    Console.WriteLine("mkc: "+c);
    return c;
  }

  static int open(int pathname, int flags, int mode) {
    not_impl("open");
    return 0;
  }


  static string mk_cs_string(int p) {
    int i=0;
    int c;
    string t = "";
    Console.WriteLine(p);
    c = ri8(p);
    while(c!=0) {
/* FIXME quadratic */
      t = t + ((char)c).ToString();
//      Console.WriteLine(t); 
      p=p+1;
      c=ri8(p);
    }
    return t;
  }

  static void init_cs() {
    Console.WriteLine("init_cs called");
    heap = new byte[16*1024*1024];
    brk_ptr = 128*1024;
  }
