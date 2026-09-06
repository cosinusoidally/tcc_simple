  public static void Main(string[] args) {
    Console.WriteLine("Hello world");
//    not_impl("blah");
    init_cs();
    int cmd=mks("cjsawk.exe");
    int inp=mks(args[0]);
    int outp=mks(args[1]);
    Console.WriteLine(mk_cs_string(cmd));
    Console.WriteLine(mk_cs_string(inp));
    Console.WriteLine(mk_cs_string(outp));
  }
}
