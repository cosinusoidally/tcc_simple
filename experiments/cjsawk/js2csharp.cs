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
        ts = tl[0];
        string t2 = tl[1].Split(")")[0];
        t2 = String.Join("", t2.Split(" "));
        string[] t2a = t2.Split(",");
        if(t2a[0] != "") {
          for(int j=0 ; j<t2a.Length ; j++) {
            t2a[j]="int "+t2a[j];
          }
        }
        t2=String.Join(",", t2a);
        ta[i] = ts + "("+t2+") {";
      } else if(tl[0]=="var"){
        tl[0] = "static int";
        ta[i] = String.Join(" ", tl);
      }
    }

    t = String.Join("\n",ta);
    o[1] = t;
    f= String.Join("\n", o);
    File.WriteAllText(args[1], f, Encoding.ASCII);
  }
}
