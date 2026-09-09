using System;
using System.IO;
using System.Text;

public class Program
{
  public static void Main(string[] args) {
    string[] o = {"\n", "\n", "\n"};
    string f;
    if(args.Length!=2) {
      throw new Exception("must give 2 args");
    }
    o[0]=File.ReadAllText("pre.cs");
    o[2]=File.ReadAllText("post.cs");
    f= String.Join("\n", o);
    File.WriteAllText(args[1],f);
  }
}
