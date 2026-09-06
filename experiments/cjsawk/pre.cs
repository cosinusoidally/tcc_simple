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
    int p = malloc(a.Length);
    for(int i=0; i<a.Length; i++) {
      Console.WriteLine(i+":"+char.ConvertToUtf32(a,i));
    }
    not_impl("mks");
    return 0;
  }
  static int ri8(int a) {
    not_impl("ri8");
    return 0;
  }
  static int wi8(int a, int b) {
    not_impl("wi8");
    return 0;
  }

  static int OR(int a, int b) {
    not_impl("OR int int");
    return 0;
  }
  static bool OR(bool a, bool b) {
    not_impl("OR bool bool");
    return false;
  }

  static int AND(int a, int b) { return 0; }
  static bool AND(bool a, bool b) { return false; }

  static int shl(int a, int b) { return a << b; }
  static int shr(int a, int b) { return a >> b; }
  static int add(int a, int b) { return a + b; }
  static int SUB(int a, int b) { return a - b; }
  static int sub(int a, int b) { return SUB(a, b); }
  static int mul(int a, int b) { return a * b; }
  static int div(int a, int b) { return a / b; }
  static int mod(int a, int b) { return a % b; }

  static bool neq(int a, int b) { return false; }
  static bool lt(int a, int b) { return false; }

  static bool eq(int a, int b) { return false; }
  static bool eq(int a, bool b) { return false; }
  static bool gt(int a, int b) { return false; }
  static bool gte(int a, int b) { return false; }
  static bool lte(int a, int b) { return false; }

  static bool TO_BOOL(int a) { return false; }

  static int v_fgetc(int a) { return 0; }
  static int v_fputs(int a, int b) { return 0; }
  static int v_fopen(int a, int b) { return 0; }
  static int v_fclose(int a) { return 0; }

  static int v_calloc(int a, int b) { return 0; }
  static int v_free(int a) { return 0; }

  static int v_exit(int a) { return 0; }

  static int mkC(string a) { return 0; }

  static int init_support() { return 0; }

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

  static int brk(int addr) {
    if(addr==0){
      return brk_ptr;
    } else {
      brk_ptr = addr;
      return addr;
    }
  }

  static void init_cs() {
    Console.WriteLine("init_cs called");
    heap = new byte[16*1024*1024];
    brk_ptr = 128*1024;
  }
