function writeFile(x, s) {
  var fs = new ActiveXObject("Scripting.FileSystemObject");
  // Create text file, second arg is overwrite, third makes it ascii
  var f = fs.CreateTextFile(x, true, false);
  f.Write(s);
  f.Close();
}

print=function(x) {
  WScript.Echo(x);
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

function read(x, y) {
  print("read: "+x+" "+y);
  if(y=="binary") {
    print("read binary read not supported yet");
    throw "error";
  }
  return readFile(x);
}

function getcwd(){
  var shell = new ActiveXObject("WScript.Shell");
  return shell.CurrentDirectory;
}

function chdir(x) {
  var shell = new ActiveXObject("WScript.Shell");
  var fso = new ActiveXObject("Scripting.FileSystemObject");
  var targetPath = fso.GetAbsolutePathName(x);
  shell.CurrentDirectory = targetPath;
}


function new_engine() {
  var engine = new ActiveXObject("MSScriptControl.ScriptControl");
  engine.Language = "JScript";
  load_order=[];
  function load(x) {
    print("Loading: " + x);
    var code = read(x);

    if((x=="m0_test.js") || (x == "hex2_test.js")) {
      print("apply hack (to not run go() straight away)");
      code=code.split("\n");
      code.pop();
      code.pop();
      code=code.join("\n");
      print(code);
    }
    
    // This executes your file code directly in the engine's global scope
    try{
      engine.AddCode(code);
    } catch(e) {
      var err = engine.Error;
           
    // 2. Format a comprehensive error message
    /* FIXME the line number is nonesense */
      var errorLog = "\n=== ENGINE ENGINE ERROR ===\n" +
                     "File:        " + x + "\n" +
                     "Line Number: " + err.Line + "\n" +
                     "Character:   " + err.Column + "\n" +
                     "Description: " + err.Description + "\n";
            
      // Add the specific source code snippet if available
      if (err.Text) {
        errorLog += "Source Code: " + err.Text + "\n";
      }
      errorLog += "===========================\n";
            
      // 3. Output the debug log to the console
      WScript.Echo(errorLog);
            
      // 4. Clear the engine error state so it doesn't leak into subsequent loads
      engine.Error.Clear();
      WScript.Quit();
   }
  }

  /* thse functions are exposed to the child */
  hostBridge = {
    print: function(x) {print(x)},
    load: load,
    read: read,
    getcwd: getcwd,
    chdir: chdir,
    writeFile: writeFile
  }
  engine.AddObject("Host", hostBridge, true);
  return {engine: engine, load:load};
}

print("cwd: "+getcwd());
chdir("../cjsawk");
print("cwd: "+getcwd());

function run(cmdline) {
  var e=new_engine();
  var tool = (cmdline.split(" ")[0]);
  print("tool: "+tool);
  e.engine.AddCode('tool="'+tool+'";');
  e.load("../xp_linux/cscript_child.js");
  e.engine.AddCode('go("'+cmdline+'")');
}

run("m0 ../xp_linux/min_win32_asm.M1 ../xp_linux/artifacts/min_win32_cscript.hex2");
run("hex2 ../xp_linux/artifacts/min_win32_cscript.hex2 ../xp_linux/artifacts/min_win32_cscript1.exe.tmp dummy");
run("hex2 ../xp_linux/artifacts/min_win32_cscript.hex2 ../xp_linux/artifacts/min_win32_cscript2.exe.tmp dummy dummy");
print("got here");
