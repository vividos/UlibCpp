@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2019 Michael Fink
REM
REM Build NuGet package
REM

call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"

REM
REM Build all
REM
msbuild Build.proj /m /target:Build

cd nupkg
call CreateNuGetPackage.cmd
cd ..

pause
