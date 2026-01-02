print("Hello world from mquickjs");

var dprint;

var NULL;
var FALSE;
var TRUE;

var in_file_num;
var out_file_num;

var mks;

var argv;

var fname;

fname = "artifacts/deps/cjsawk_full.c";

var read_=read;

read=function(x,y){
  if(arguments.length>1){
    if(y==="binary"){
      var b=read_(x);
      b=b.split("");
      b=b.map(function(x) {return x.charCodeAt(0);});
      return b;
    };
  };
  return read_(x);
};

load("cjsawk_test.js");
