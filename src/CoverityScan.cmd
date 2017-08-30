@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2017 Michael Fink
REM
REM runs Coverity analysis build
REM

rmdir .\cov-int /s /q 2> nul
del cov-int.zip 2> nul

call "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
set PATH=%PATH%;D:\devel\tools\cov-analysis-win64-8.7.0\bin

cov-build --dir cov-int ^
   msbuild UlibCpp.sln /property:Configuration=Release /property:Platform=x86 /target:Rebuild

"%ProgramFiles%\7-Zip\7z.exe" a cov-int.zip .\cov-int

pause
