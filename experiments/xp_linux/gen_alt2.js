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

function gen_load_list(x) {
  var o = [];
  var t;
  for(var i = 0;i< x.length;i++) {
    t = "load_file "+x[i]+" "+x[i];
    o.push(t);
  }
  return o.join("\n");
}

function gen_load_list_remap(x, s1, s2) {
  var o = [];
  var t;
  for(var i = 0;i< x.length;i++) {
    t = "load_file "+x[i]+" " +s2+x[i].slice(s1.length);
    o.push(t);
  }
  return o.join("\n");
}

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

function gen_load_list(x) {
  var o = [];
  var t;
  for(var i = 0;i< x.length;i++) {
    t = "load_file "+x[i]+" "+x[i];
    o.push(t);
  }
  return o.join("\n");
}

function gen_load_list_remap(x, s1, s2) {
  var o = [];
  var t;
  for(var i = 0;i< x.length;i++) {
    t = "load_file "+x[i]+" " +s2+x[i].slice(s1.length);
    o.push(t);
  }
  return o.join("\n");
}

function print_file(x) {
  var t;
  t = fs.readFileSync(x,"utf8").split("\n");
  if(t[t.length-1] === "") {
    t.pop();
  }
  t = t.join("\n");
  return t;
}

tcc_b_list = find_files("../../../tcc_bootstrap_alt");

tcc_b_list = tcc_b_list.filter(function(x) {if(x.match(".git")==null) {return true;}});
tcc_b_list = tcc_b_list.filter(function(x) {if(x.match("artifacts/")==null) {return true;}});

tcc_list = find_files("../tcc-0.9.26-lb");
mes_list = find_files("../mes-0.27");
portable_libc_list = find_files("../pnut_refactor/portable_libc");
extra_list = find_files("../mes-0.27/include/mes");

tcc_list = tcc_list.filter(function(x) {if(x.match("tests")==null) {return true;}});

var out = [
  gen_load_list(tcc_list),
  gen_load_list(mes_list),
  gen_load_list_remap(portable_libc_list, "../pnut_refactor/", ""),
  gen_load_list_remap(extra_list, "../mes-0.27/include", "../artifacts/boot0"),
  gen_load_list_remap(tcc_b_list, "../../../tcc_bootstrap_alt", ""),
  print_file("cjsawk.list"),
  print_file("pnut.list"),
  print_file("mes_libc.list"),
  print_file("tcc_boot_mes.list"),
  print_file("elf_loader.list"),
  "/catm ../artifacts/elf_loader_cc_x86.exe /elf_loader_cc_x86.exe",
  print_file("tcc_26.list"),
].join("\n") +"\n";

fs.writeFileSync("artifacts/full.list", out);
