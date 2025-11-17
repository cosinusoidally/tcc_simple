rm -r artifacts/builds/xpcshell
mkdir artifacts/builds
mkdir artifacts/builds/xpcshell
# work around https://bugzilla.mozilla.org/show_bug.cgi?id=1807043 by
# redirecting stdout and and stderr
../../../firefox/firefox.exe -xpcshell cjsawk_xpcshell.js *>&1 | ForEach-Object { Write-Host "$_" }

Get-FileHash -A sha1 artifacts/builds/xpcshell/*

echo "DONE xpcshell"
