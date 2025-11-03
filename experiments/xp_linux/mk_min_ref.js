console.log("Bootstrapping using node.js. Requires node.js >= 10.x");

process=require("process");
child_process=require("child_process");
os=require("os");
fs=require("fs");

console.log(process.execPath);

run=function(x){
  console.log("Running: "+x);
  var r=child_process.spawnSync(process.execPath,["../m2min_v3/node_compat_min.js",x]);
//  console.log("stdout:");
//  console.log(r.output[1].toString());
  console.log("stderr:");
  console.log(r.output[2].toString());
  return r.output[1];
};

process.chdir("../cjsawk");

function hex2(src, extra_args) {
  var res;
  var my_cmd = 'fname="' + src+ '"; \
extra_args = "' + extra_args + '"; \
load("hex2_test.js");'
  fs.writeFileSync("artifacts/test.js", my_cmd);
  res = run("artifacts/test.js").toString();
  return Buffer.from(res, "base64");
}

function m0(src) {
  var res;
  var my_cmd = 'fname="' + src+ '"; load("m0_test.js");'
  fs.writeFileSync("artifacts/test.js", my_cmd);
  res = run("artifacts/test.js").toString();
  return res;
}

function cjsawk(src) {
  var res;
  var my_cmd = 'fname="' + src+ '"; load("cjsawk_test.js");'
  fs.writeFileSync("artifacts/test.js", my_cmd);
  res = run("artifacts/test.js").toString();
  return res;
}

function concat(arr) {
  var t = [];
  var c;
  arr = arr.map(function(x) {
    return fs.readFileSync(x);
  });
  for(var i=0; i < arr.length;i++) {
    c = arr[i];
    for(var j = 0; j < c.length;j++) {
      t.push(c[j]);
    }
  }
  return new Buffer(t);
}

out0 = m0("../xp_linux/min_win32_ref.M1");
fs.writeFileSync("../xp_linux/artifacts/min_win32_node.hex2", out0);

out1 = hex2("../xp_linux/artifacts/min_win32_node.hex2", " artifacts/dummy1");
out2 = hex2("../xp_linux/artifacts/min_win32_node.hex2", " artifacts/dummy1 artifacts/dummy2");
fs.writeFileSync("../xp_linux/artifacts/min_win32_node1.exe.tmp", out1);
fs.writeFileSync("../xp_linux/artifacts/min_win32_node2.exe.tmp", out2);

out = new Buffer(1024*6.5);
i = 0;

while(i<512){
  out[i] = out1[i];
  i = i + 1;
}

while(i<(5.5*1024)){
  out[i] = out2[i];
  i = i + 1;
}

while(i<(6.5*1024)){
  out[i] = out1[i];
  i = i + 1;
}

fs.writeFileSync("../xp_linux/artifacts/min_win32_node.exe", out);

tmp = concat(["../xp_linux/globals.js", "../xp_linux/xp_linux.js"]).toString();
fs.writeFileSync("../xp_linux/artifacts/xp_linux_full_node.js", tmp);

out3 = cjsawk("../xp_linux/artifacts/xp_linux_full_node.js");
fs.writeFileSync("../xp_linux/artifacts/xp_linux_node.exe.M1", out3);

tmp = concat(["../m2min_v3/simple_asm_defs.M1",
              "../m2min_v3/x86_defs.M1",
              "../m2min_v3/libc-core.M1",
              "../xp_linux/artifacts/xp_linux_node.exe.M1"]);
fs.writeFileSync("../xp_linux/artifacts/xp_linux_full_node.exe-0.M1", tmp);

out4 = m0("../xp_linux/artifacts/xp_linux_full_node.exe-0.M1");
fs.writeFileSync("../xp_linux/artifacts/xp_linux_node.exe.hex2", out4);

tmp = concat(["../m2min_v3/ELF-i386.hex2",
              "../xp_linux/artifacts/xp_linux_node.exe.hex2"]);
fs.writeFileSync("../xp_linux/artifacts/xp_linux_full_node.exe-0.hex2", tmp);

out5 = hex2("../xp_linux/artifacts/xp_linux_full_node.exe-0.hex2");
fs.writeFileSync("../xp_linux/artifacts/xp_linux_node.exe", out5);
