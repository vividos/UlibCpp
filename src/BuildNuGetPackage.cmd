@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2025 Michael Fink
REM
REM Build NuGet package
REM

REM set this to your Visual Studio installation folder
set VSINSTALL=%ProgramFiles%\Microsoft Visual Studio\18\Community

REM
REM Preparations
REM
call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat"

REM
REM Restore NuGet packages
REM
nupkg\nuget restore UlibCpp.sln

REM
REM Build all
REM
msbuild Build.proj /m /target:Build

cd nupkg
call CreateNuGetPackage.cmd
cd ..

pause
