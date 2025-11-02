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


my_cmd = 'fname="../xp_linux/min_win32_asm.M1"; \
load("m0_test.js");'
fs.writeFileSync("artifacts/test.js", my_cmd); 
out0=run("artifacts/test.js");
fs.writeFileSync("../xp_linux/artifacts/test.hex2", out0);

my_cmd = 'fname="../xp_linux/artifacts/test.hex2"; \
load("hex2_test.js"); \
extra_args = " artifacts/dummy1";'
fs.writeFileSync("artifacts/test.js", my_cmd); 
out1=run("artifacts/test.js").toString();
out1=Buffer.from(out1, "base64");
fs.writeFileSync("../xp_linux/artifacts/min_win32_node1.exe", out1);

my_cmd = 'fname="../xp_linux/artifacts/test.hex2"; \
load("hex2_test.js"); \
extra_args = " artifacts/dummy1 artifacts/dummy2";'
fs.writeFileSync("artifacts/test.js", my_cmd);
out2=run("artifacts/test.js").toString();
out2=Buffer.from(out2, "base64");
fs.writeFileSync("../xp_linux/artifacts/min_win32_node2.exe", out2);
