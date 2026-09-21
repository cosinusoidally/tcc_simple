var fso = new ActiveXObject("Scripting.FileSystemObject");
var shell = new ActiveXObject("WScript.Shell");

print=function(x) {
  WScript.Echo(x);
}

function writeFile(x, s) {
  var fs = new ActiveXObject("Scripting.FileSystemObject");
  // Create text file, second arg is overwrite, third makes it ascii
  var f = fs.CreateTextFile(x, true, false);
  f.Write(s);
  f.Close();
}

ForReading = 1;
TristateFalse = 0;

function readFile(x) {
  var fso;
  var f;
  print("readFile: "+x);
  fso = new ActiveXObject("Scripting.FileSystemObject");
  f = fso.OpenTextFile(x, ForReading, false, TristateFalse);
  return f.ReadAll();
}

function find_files(rel_dir) {
  var t = [];
  var rel_path;
    
  /* handle none existing folder by returning an empty list */
  if (!fso.FolderExists(rel_dir)) {
    return t;
  }
    
  var abs_path = fso.GetAbsolutePathName(rel_dir);

  /* We shell out here as cscript's native folder traversal stuff can't handle
   * paths longer than 260 (historical MAX_PATH limit).
   * We use 'dir' with the following flags:
   * /b Bare format (just paths, no headers/footers)
   * /s Recursive subfolders
   * /a:-d Attribute: not directories
   */
  var cmd = 'cmd.exe /c dir "' + abs_path + '" /b /s /a:-d';
  var exec = shell.Exec(cmd);
    
  while (!exec.StdOut.AtEndOfStream) {
    var path = exec.StdOut.ReadLine();
    // skip empty lines
    if (path.replace(/\s/g, "") !== "") {
      // path will be absolute so we must convert to relative
      rel_path=(rel_dir+path.slice(abs_path.length)).split("\\").join("/");
      t.push(rel_path);
    }
  }
  return t;
}

function print_list(x) {
  for(var i =0;i< x.length;i++) {
    print(x[i]);
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

function filter_array(a, f) {
  var o=[];
  for(var i=0;i<a.length;i++) {
    if(f(a[i])) {
      o.push(a[i])
    }
  }
  return o;
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
  t = readFile(x).split("\n");
  if(t[t.length-1] === "") {
    t.pop();
  }
  t = t.join("\n");
  return t;
}

tcc_b_list = find_files("../../../tcc_bootstrap_alt");

tcc_b_list = filter_array(tcc_b_list, function(x) {if(x.match(".git")==null) {return true;}});
tcc_b_list = filter_array(tcc_b_list, function(x) {if(x.match("artifacts/")==null) {return true;}});

tcc_list = find_files("../tcc-0.9.26-lb");
mes_list = find_files("../mes-0.27");
portable_libc_list = find_files("../pnut_refactor/portable_libc");
extra_list = find_files("../mes-0.27/include/mes");

tcc_list = filter_array(tcc_list, function(x) {if(x.match("tests")==null) {return true;}});

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
].join("\n");

writeFile("artifacts/full2.list", out);
