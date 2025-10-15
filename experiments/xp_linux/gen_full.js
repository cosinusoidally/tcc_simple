fs=require("fs");

function find_files(x, prefix) {
  var t= fs.readdirSync(x);
  var n;
  var s;
  var out = [];
  var tout;
  if(prefix === undefined) {
    prefix = x+"/";
  }
  for(var i = 0 ; i < t.length ; i++){
    n = t[i];
    s = fs.statSync(x+"/"+n).isDirectory();
    if(s) {
      tout = find_files(x+"/"+n, x+"/"+n+"/");
      for(var j = 0; j< tout.length; j++) {
        out.push(tout[j]);
      } 
    } else {
      out.push(prefix+t[i]);
    }
  }
  return out;
}

function print_list(x) {
  for(var i =0;i< x.length;i++) {
    console.log(x[i]);
  }
}

tcc_list = find_files("../tcc-0.9.26-lb");

tcc_list = tcc_list.filter(function(x) {if(x.match("tests")==null) {return true;}});

print_list(tcc_list);
