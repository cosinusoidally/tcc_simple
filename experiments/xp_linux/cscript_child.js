print("hello world from child");

print("cwd: "+getcwd());
chdir("../cjsawk");
print("cwd: "+getcwd());

Uint8Array=Array;

read_ =read;

read = function(x,y){ 
  var t;
  t = read_(x);
  if(y == "binary"){
    print("oh now binary");
    t = t.split("");
    for(var i=0;i<t.length;i++){
       t[i]=t[i].charCodeAt(0);
    }
  }
  return t;
}

fname="../xp_linux/min_win32_asm.M1";
load("m0_test.js");

gen_out = function(){
  print("in gen_out");
  for(var i=0;i<out_file.length;i++){
     out_file[i]=String.fromCharCode(out_file[i]);
  }
  out_file = out_file.join("");
  writeFile("../xp_linux/artifacts/result.hex2", out_file);
  return out_file;
}

mkc_= mkc;
mkc=function(c) {
  if(c==undefined) {return 0;}
  return mkc_(c);
}

ri8_= ri8;

ri8=function(x) {
  if(ri8_(x)==undefined) {return 0;}
  return ri8_(x);
}

mkC=function(x){
  return mkc(x);
}

go();
