rm -r artifacts/builds/xpcshell
mkdir artifacts/builds
mkdir artifacts/builds/xpcshell
../../../firefox/firefox.exe -xpcshell cjsawk_xpcshell.js

echo "DONE xpcshell"
