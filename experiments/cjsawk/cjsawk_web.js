window.onload=function() {
  console.log("cjsawk hello world");
  getfile("cjsawk.js");
  setTimeout(check_ready, 100);
}

function go() {
  load("cjsawk.js");
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
    go();
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
    fd.response = xhttp.responseText;
    fd.ready = true;
  }
  };
  xhttp.open("GET", f, true);
  xhttp.send();
}

