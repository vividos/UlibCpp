@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2018 Michael Fink
REM
REM Runs SonarCloud analysis build
REM

REM set this to your Visual Studio installation folder
set VSINSTALL=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community

REM set this to your SonarQube tools folder
set SONARQUBE=D:\devel\tools\SonarQube

REM and this to your OpenCppCoverage folder
set OPENCPPCOVERAGE=D:\devel\tools\OpenCppCoverage\

REM
REM Preparations
REM
call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat"

set PATH=%PATH%;%SONARQUBE%\build-wrapper-win-x86;%SONARQUBE%\sonar-scanner-msbuild;%OPENCPPCOVERAGE%

REM
REM Build using SonarQube scanner for MSBuild
REM
rmdir .\bw-output /s /q 2> nul

SonarQube.Scanner.MSBuild.exe begin ^
    /k:"UlibCpp" ^
    /v:"4.1.2" ^
    /d:"sonar.cfamily.build-wrapper-output=%CD%\bw-output" ^
    /d:"sonar.host.url=https://sonarcloud.io" ^
    /d:"sonar.organization=vividos-github" ^
    /d:"sonar.login=%SONARLOGIN%" ^
    /d:sonar.cs.vstest.reportsPaths="%CD%\TestResults\*.trx"

REM
REM Rebuild Release|x86
REM
build-wrapper-win-x86-64.exe --out-dir bw-output msbuild UlibCpp.sln /m /property:Configuration=Release /property:Platform=x86 /target:Rebuild

REM
REM Run unit tests
REM
OpenCppCoverage.exe ^
   --continue_after_cpp_exception --cover_children ^
   --sources ulib --sources test --sources include\ulib ^
   --excluded_sources packages\boost ^
   --export_type html:CoverageReport ^
   --modules test.dll ^
   -- "%VSINSTALL%\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" ^
   "..\bin\Release\Win32\test\test.dll" /Platform:x86 /InIsolation /logger:trx

SonarQube.Scanner.MSBuild.exe end /d:"sonar.login=%SONARLOGIN%"

pause
