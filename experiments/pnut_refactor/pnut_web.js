function init() {
  support_code = document.createElement("script");
  support_code.src = "pnut_js_runner.js";
  document.body.appendChild(support_code);
}

function quit(c) {
  console.log("quit, exit code: "+c);
  throw "quit";
}

vfs = {};

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

window.onload=function() {
  console.log("pnut_web");
  getfile("pnut.c");
  getfile("support.c");
  getfile("support_common.c");
  getfile("pnut.js");

  setTimeout(check_ready, 100);

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

function start() {
  os = {};
  os.file = {};
  os.file.readFile = function(x) {
    var f;
    console.log("readFile not impl: "+x);
    if(f=vfs[x]) {
      console.log("readFile reading: "+x);
      f = f.file.split("");
      f = f.map(function(x){return x.charCodeAt(0)});
    } else {
      console.log("readFile file not found: "+x);
      throw "error";
    }
//    console.log(f);
    return f;
  }

  os.file.writeTypedArrayToFile = function(filename, data) {
    console.log(filename);
    console.log(filename.length);
    console.log(data);
    console.log(data.length);
    var l = document.createElement("a");
    l.textContent="download pnut.exe";
    // FIXME we should aviod typed array toBase64 as it's relatively new
    l.href="data:application/octet-stream;base64,"+data.toBase64();
    l.download = "pnut.exe";
    document.body.appendChild(l);
  }

  scriptArgs = ["pnut.c","pnut.exe"];
  os.getenv = function(x) {
    console.log("os.getenv: "+x);
    return 0;
  }
  load = function(x) {
    console.log("load dummy impl: "+x);
  }

  printErr = function(x) {
    console.log("printErr: "+x);
    throw "error";
  }

  init();
}
