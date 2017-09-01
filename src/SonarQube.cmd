@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2017 Michael Fink
REM
REM Runs SonarQube analysis build
REM

call "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

REM set this to your SonarQube tools folder
set SONARQUBE=D:\devel\tools\SonarQube

PATH=%PATH%;%SONARQUBE%\build-wrapper-win-x86;%SONARQUBE%\sonar-scanner-msbuild

REM
REM Build using SonarQube scanner for MSBuild
REM
rmdir .\bw-output /s /q 2> nul

SonarQube.Scanner.MSBuild.exe begin ^
    /k:"UlibCpp" ^
    /v:"4.0.1" ^
    /d:"sonar.cfamily.build-wrapper-output=%CD%\bw-output" ^
    /d:"sonar.host.url=https://sonarqube.com" ^
    /d:"sonar.organization=vividos-github" ^
    /d:"sonar.login=c59d3403d04744579f89e55e23c27a171293577e" ^
    /d:sonar.cs.vstest.reportsPaths="%CD%\TestResults\*.trx"

REM
REM Build all
REM
build-wrapper-win-x86-64.exe --out-dir bw-output msbuild UlibCpp.sln /m /property:Configuration=Release /property:Platform=x86 /target:Rebuild

REM
REM Run unit tests
REM
vstest.console.exe /Platform:x86 /InIsolation /logger:trx ^
   "..\bin\Release\Win32\test\test.dll"

SonarQube.Scanner.MSBuild.exe end /d:"sonar.login=c59d3403d04744579f89e55e23c27a171293577e"

pause
