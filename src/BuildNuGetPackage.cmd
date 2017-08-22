@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2017 Michael Fink
REM
REM Build NuGet package
REM

call "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

REM
REM Build all
REM
msbuild UlibCpp.sln /m /property:Configuration=Debug   /property:Platform=x86 /target:Rebuild
msbuild UlibCpp.sln /m /property:Configuration=Release /property:Platform=x86 /target:Rebuild
msbuild UlibCpp.sln /m /property:Configuration=Debug   /property:Platform=x64 /target:Rebuild
msbuild UlibCpp.sln /m /property:Configuration=Release /property:Platform=x64 /target:Rebuild

cd nupkg
call CreateNuGetPackage.cmd
cd ..

pause
