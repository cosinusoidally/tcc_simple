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

    for(int i=0;i<ta.Length;i++) {
      string[] tl;
      string ts;
      tl=ta[i].Split(" ");
      if(tl[0]=="function") {
        tl[0] = "static int";
        ts = String.Join(" ", tl);
        tl = ts.Split("(");
        string t2 = tl[1].Split(")")[0];
        t2 = String.Join("", t2.Split(" "));
        ta[i] = ts + "// "+t2;
      } else {
      }
    }

    t = String.Join("\n",ta);
    o[1] = t;
    f= String.Join("\n", o);
    File.WriteAllText(args[1],f);
  }
}
