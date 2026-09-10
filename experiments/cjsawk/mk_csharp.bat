del artifacts\js2csharp.exe
del artifacts\cjsawk.cs
del artifacts\cjsawk.exe
del artifacts\cjsawk.M1

set CSC_PATH=C:\Windows\Microsoft.NET\Framework\v4.0.30319\csc.exe

%CSC_PATH% js2csharp.cs /out:artifacts\js2csharp.exe
.\artifacts\js2csharp.exe cjsawk_csharp.cs artifacts\cjsawk.cs
%CSC_PATH% artifacts\csharp.cs /out:artifacts\cjsawk.exe
.\artifacts\cjsawk.exe cjsawk.js artifacts\cjsawk.M1
