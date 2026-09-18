print("hello world from child");

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

var tool;
if(tool == ("m0")) {
  load("m0_test.js");
} else if(tool == "hex2") {
  load("hex2_test.js");
} else {
  throw "unsupported tool";
}

gen_out = function(f){
  print("in gen_out writing: "+f);
//  print(out_file);
  if(tool == "m0") {
    for(var i=0;i<out_file.length;i++){
      out_file[i]=String.fromCharCode(out_file[i]);
    }
    out_file = out_file.join("");
    writeFile(f, out_file);
    return out_file;
  } else if(tool == "hex2") {
    for(var i =0;i<out_file.length;i++) {
      out_file[i]=("0000"+(out_file[i]&0xFF).toString(16)).slice(-2);
      if((((i+1)%16) == 0)) {out_file[i]=out_file[i]+"\n"}
    }
    out_file=out_file.join("");
    writeFile(f, out_file);
  } else {
    print("not supported tool");
    throw "error";
  }
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

go = function(cmdline) {
  var args;
  args=mk_args(cmdline);
  main(args[0], args[1]);
  gen_out(cmdline.split(" ")[2]);
}
