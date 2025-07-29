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

  sha256_init();
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

  in_area = document.createElement("textarea");
  in_area.style.width="50%";
  in_area.style.height="100%";
  in_area.textContent = read("./artifacts/cjsawk_full.c");

  out_area = document.createElement("textarea");
  out_area.style.width="50%";
  out_area.style.height="100%";

  document.body.appendChild(in_area);
  document.body.appendChild(out_area);


  var st = Date.now();
  load("cjsawk_test.js");
  var fin = Date.now();
  console.log("compile took: "+(fin-st)+"ms");

  out_area.textContent = pout[0];

  console.log("cjsawk.exe.M1 sha256sum:");
  console.log(sha256([pout[0], "\n"].join("")));
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

