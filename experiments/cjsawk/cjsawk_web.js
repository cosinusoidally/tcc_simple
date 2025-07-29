pout = [];

window.onload=function() {
  console.log("cjsawk hello world");
  getfile("cjsawk_test.js");
  getfile("support.js");
  getfile("support_libc.js");
  getfile("support_alu.js");
  getfile("cjsawk.js");

  getfile("simple_support_js_m2_prims.c");
  getfile("support_m2.c");

  fname="./artifacts/cjsawk_full.c";

  print=function(x){
    pout.push(x);
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
  console.log("file read error " + x);
  throw "error";
};


function start() {
  vfs["./artifacts/cjsawk_full.c"] = {
    ready: true,
    file: [
        read("cjsawk.js"),
        read("support_libc.js"),
        read("simple_support_js_m2_prims.c"),
        read("support_m2.c"),
    ].join("")
  }

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

