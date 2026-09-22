Option Explicit

Dim args
Set args = WScript.Arguments

If args.Count < 2 Then
  WScript.Echo "Usage: cscript.exe hex2bin.vbs <input_hex_file> <output_binary_file>"
  WScript.Quit 1
End If

Dim inputFile, outputFile
inputFile  = args(0)
outputFile = args(1)

Dim fso
Set fso = CreateObject("Scripting.FileSystemObject")

If Not fso.FileExists(inputFile) Then
  WScript.Echo "Error: input file does not exist: " & inputFile
  WScript.Quit 1
End If

Dim hexFile, rawHex, ForReading
ForReading = 1
Set hexFile = fso.OpenTextFile(inputFile, ForReading)
rawHex = hexFile.ReadAll
hexFile.Close

rawHex = Replace(rawHex, " ", "")
rawHex = Replace(rawHex, vbCr, "")
rawHex = Replace(rawHex, vbLf, "")
rawHex = Replace(rawHex, vbTab, "")

If Len(rawHex) Mod 2 <> 0 Then
  WScript.Echo "Error: hex dump is an odd number of chars long" & Len(rawHex)
  WScript.Quit 1
End If

Dim totalBytes, byteArray()
totalBytes = Len(rawHex) / 2
ReDim byteArray(totalBytes - 1)

Dim i, hexByte, index
index = 0
For i = 1 To Len(rawHex) Step 2
  hexByte = Mid(rawHex, i, 2)
  byteArray(index) = Chr(CByte("&H" & hexByte))
  index = index + 1
Next

Dim binaryData
binaryData = Join(byteArray, "")

Dim binFile
' last arg being False means we are in ASCII mode
Set binFile = fso.CreateTextFile(outputFile, True, False)
binFile.Write binaryData
binFile.Close

WScript.Echo "Success: Converted " & inputFile & " to " & outputFile
