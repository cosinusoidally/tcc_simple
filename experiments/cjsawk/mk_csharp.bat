del artifacts\js2csharp.exe
del artifacts\cjsawk.cs
del artifacts\cjsawk.exe
del artifacts\cjsawk.M1

set CSC_PATH=C:\Windows\Microsoft.NET\Framework\v4.0.30319\csc.exe

%CSC_PATH% /out:artifacts\js2csharp.exe js2csharp.cs
.\artifacts\js2csharp.exe cjsawk_csharp.js artifacts\cjsawk.cs
%CSC_PATH% /out:artifacts\cjsawk.exe artifacts\cjsawk.cs
.\artifacts\cjsawk.exe cjsawk.js artifacts\cjsawk.M1
