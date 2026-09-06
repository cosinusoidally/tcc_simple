using System;
using System.IO;
using System.Text;

public class Program
{
  public static int Main(string[] args) {
    byte[] fa;
    byte[] fb;
    if(args.Length==2) {
       fa=File.ReadAllBytes(args[0]);
       fb=File.ReadAllBytes(args[1]);
       if(fa.Length == fb.Length) {
         for(int i=0;i<fa.Length;i++) {
            if(fa[i]!=fb[i]) {
              Console.WriteLine("files differ");
              return 1;
            }
         }
         Console.WriteLine("files same");
         return 0;
       }
       Console.WriteLine("files differ");
       return 1;
    }
    Console.WriteLine("you must specify 2 files to diff");
    return 1;
  }
}
