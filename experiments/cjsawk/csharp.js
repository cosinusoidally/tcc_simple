fs=require("fs");

f=fs.readFileSync("cjsawk.js","utf-8");

f=f.split("\n");

for(i=0;i<f.length;i++) {
  var t;
  var to;
  var o;
  to = t;
  t = f[i].split(" ");
  to = t.join(" ");
  if(t[0]==="function") {
    t.shift();
    t="static int "+t.join(" ");
    t=t.split("(");
    t2=t[1].split(")")[0];
    t2=t2.split(",");
    t2=t2.map(function(x){x=x.split(" ").join(""); return "int "+x});
    t = t[0] +"("+t2.join(", ")+") {";
    console.log("----");
    console.log(to);
    console.log(t);
  } else if(t[0]==="var") {
    t.shift();
    t="static int "+t.join(" ");
    console.log(t);
  }
}
