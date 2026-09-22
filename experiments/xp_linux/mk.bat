REM assumes artifacts dir is clean and empty
REM Also, you need to have tcc_bootstrap_alt and mishmashvm alongside tcc_simple
REM for the whole end to end bootstrap to fully work

echo "MSScriptControl.ScriptControl is only supported by 32 bit cscript.exe"
set "CSCRIPT_CMD=C:\Windows\SysWOW64\cscript.exe"

if exist %CSCRIPT_CMD% (
  echo "64 bit system, using 32 bit cscript.exe from SysWOW64"
) else (
  echo "32 bit system, using default cscript.exe"
  set "CSCRIPT_CMD=cscript.exe"
)

%CSCRIPT_CMD% //T:0 cscript_parent.js

copy ".\artifacts\xp_linux.cscript.exe" ".\artifacts\xp_linux.exe"

%CSCRIPT_CMD% //T:0 gen_list_cscript.js

.\artifacts\bin.exe .\artifacts\full2.list

echo "Finished bootstrap"
