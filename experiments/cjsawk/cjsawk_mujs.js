Uint8Array = Array;

read_=read;

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
