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
msbuild /t:Restore UlibCpp.sln

REM
REM Build all
REM
msbuild ulib\ulib.vcxproj /p:Configuration=Debug /p:Platform=Win32
msbuild ulib\ulib.vcxproj /p:Configuration=Release /p:Platform=Win32
msbuild ulib\ulib.vcxproj /p:Configuration=Debug /p:Platform=x64
msbuild ulib\ulib.vcxproj /p:Configuration=Release /p:Platform=x64

pushd nupkg
nuget pack
popd

pause
