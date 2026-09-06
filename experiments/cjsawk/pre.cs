using System;

public class Program
{
  static bool LTRUE = true;

  static void not_impl(string a) {
    Console.WriteLine("not_impl: "+a);
    Environment.Exit(1);
  }
  static int mks(string a) {
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

  static int shl(int a, int b) { return 0; }
  static int shr(int a, int b) { return 0; }
  static int add(int a, int b) { return 0; }
  static int SUB(int a, int b) { return 0; }
  static int mul(int a, int b) { return 0; }
  static int div(int a, int b) { return 0; }
  static int mod(int a, int b) { return 0; }

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


  static void init_cs() {
    Console.WriteLine("init_cs called");

  }
