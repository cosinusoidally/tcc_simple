fs=require("fs");

f=fs.readFileSync("cjsawk.js","utf-8");

f=f.split("\n");

for(i=0;i<f.length;i++) {
  console.log(f[i]);
}
