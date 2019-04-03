@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2019 Michael Fink
REM
REM Runs ulib Unit Tests and collects coverage information
REM

REM set this to your Visual Studio installation folder
set VSINSTALL=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community

REM and this to your OpenCppCoverage folder
set OPENCPPCOVERAGE=D:\devel\tools\OpenCppCoverage\

REM
REM Preparations
REM
call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat"

set PATH=%PATH%;%OPENCPPCOVERAGE%

REM
REM Build Debug|x86
REM
msbuild UlibCpp.sln /m /property:Configuration=Debug /property:Platform=x86 /target:Build

REM
REM Run unit tests
REM
OpenCppCoverage.exe ^
   --continue_after_cpp_exception --cover_children ^
   --sources ulib --sources test --sources include\ulib ^
   --excluded_sources packages\boost ^
   --export_type cobertura:ulib-coverage.xml ^
   --export_type html:CoverageReport ^
   --modules test.dll ^
   -- "%VSINSTALL%\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" ^
   "..\bin\Debug\Win32\test\test.dll" /Platform:x86 /InIsolation /logger:trx

pause
