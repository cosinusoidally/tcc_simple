window.onload=function() {
  console.log("cjsawk hello world");
  getfile("cjsawk_test.js");
  getfile("support.js");
  getfile("support_libc.js");
  getfile("support_alu.js");
  getfile("cjsawk.js");

  fname="./artifacts/cjsawk_full.c";

  print=function(x){
    console.log(x);
  };
  setTimeout(check_ready, 100);
}

function load(x){
  var code = read(x);
  eval.call(this,code);
};

function read(x,y){
  if(arguments.length>1){
    if(y==="binary"){
      var b=read(x);
      b=b.split("");
      b=b.map(function(x) {return x.charCodeAt(0);});
      return new Uint8Array(b);
    };
  };
  if(vfs[x] !== undefined) {
    return vfs[x].file;
  }
  print("file read error " + x);
  throw "error";
};


function start() {
  load("cjsawk_test.js");
}

function check_ready() {
  var ready = true;
  for(var i in vfs) {
    if(vfs[i].ready == false){
      ready=false;
    }
  }
  if(ready) {
    console.log("ready now");
    start();
  } else {
    console.log("not ready");
    setTimeout(check_ready, 100);
  }
}

vfs ={};

function getfile(f) {
  var xhttp = new XMLHttpRequest();
  var fd = {};
  fd.ready = false;
  vfs[f] = fd;
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
    // Typical action to be performed when the document is ready:
    fd.file = xhttp.responseText;
    fd.ready = true;
  }
  };
  xhttp.open("GET", f, true);
  xhttp.send();
}

