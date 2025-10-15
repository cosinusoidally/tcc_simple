fs=require("fs");

function find_files(x, prefix) {
  var t= fs.readdirSync(x);
  var n;
  var s;
  var st;
  var out = [];
  var tout;
  if(prefix === undefined) {
    prefix = x+"/";
  }
  for(var i = 0 ; i < t.length ; i++){
    n = t[i];
    st = fs.lstatSync(x+"/"+n);
    s = st.isDirectory();
    if(s) {
      tout = find_files(x+"/"+n, x+"/"+n+"/");
      for(var j = 0; j< tout.length; j++) {
        out.push(tout[j]);
      } 
    } else {
      if(!st.isSymbolicLink()) {
        out.push(prefix+t[i]);
      }
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
mes_list = find_files("../mes-0.27");
portable_libc_list = find_files("../pnut_refactor/portable_libc");
extra_list = find_files("../mes-0.27/include/mes");

tcc_list = tcc_list.filter(function(x) {if(x.match("tests")==null) {return true;}});

print_list(tcc_list);
print_list(mes_list);
print_list(portable_libc_list);
print_list(extra_list);
