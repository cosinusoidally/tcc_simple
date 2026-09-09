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

    string t = File.ReadAllText(args[0]);

    string[] ta;
    ta = t.Split("\n");

    t = String.Join("\n",ta);
    o[1] = t;
    f= String.Join("\n", o);
    File.WriteAllText(args[1],f);
  }
}
