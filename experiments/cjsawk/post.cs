  public static void Main(string[] args) {
    Console.WriteLine("Hello world");
//    not_impl("blah");
    init_cs();
    int cmd=mks("cjsawk.exe");
    int inp=mks(args[0]);
    int outp=mks(args[1]);
    Console.WriteLine(cmd);
    Console.WriteLine(inp);
    Console.WriteLine(outp);
  }
}
