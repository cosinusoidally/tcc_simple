using System;
using System.IO;
using System.Text;

public class Program
{
  public static void Main(string[] args) {
    string[] o = {"\n", "\n", "\n"};
    if(args.Length!=2) {
      throw new Exception("must give 2 args");
    }
    o[0]=File.ReadAllText("pre.cs");
    o[2]=File.ReadAllText("post.cs");
    Console.Write(String.Join("\n", o));
/*
    fs.Write(pre);
    fs.Close();
*/
    Console.WriteLine("hello  world");
  }
}
