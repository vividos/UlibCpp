@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2021 Michael Fink
REM
REM Runs SonarCloud analysis build
REM

REM set this to your Visual Studio installation folder
set VSINSTALL=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community

REM set this to your SonarQube tools folder
set SONARQUBE=C:\Projekte\Tools\SonarQube

REM and this to your OpenCppCoverage folder
set OPENCPPCOVERAGE=C:\Projekte\Tools\OpenCppCoverage\

REM
REM Preparations
REM
call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat"

if "%SONARLOGIN%" == "" echo "Environment variable SONARLOGIN is not set! Obtain a new token and set the environment variable!"
if "%SONARLOGIN%" == "" exit 1

set PATH=%PATH%;%SONARQUBE%\build-wrapper-win-x86;%SONARQUBE%\sonar-scanner-msbuild;%OPENCPPCOVERAGE%

REM
REM Build using SonarQube scanner for MSBuild
REM
rmdir .\.sonarqube /s /q 2> nul
rmdir .\bw-output /s /q 2> nul

SonarScanner.MSBuild.exe begin ^
    /k:"UlibCpp" ^
    /v:"4.2.5" ^
    /d:"sonar.cfamily.build-wrapper-output=%CD%\bw-output" ^
    /d:"sonar.coverageReportPaths=ulib-coverage.xml" ^
    /d:"sonar.host.url=https://sonarcloud.io" ^
    /o:"vividos-github" ^
    /d:"sonar.login=%SONARLOGIN%" ^
    /d:sonar.cs.vstest.reportsPaths="%CD%\TestResults\*.trx"
if errorlevel 1 goto end

REM
REM Restore NuGet packages
REM
nupkg\nuget restore UlibCpp.sln

REM
REM Rebuild Release|x86
REM
build-wrapper-win-x86-64.exe --out-dir bw-output msbuild UlibCpp.sln /m /property:Configuration=SonarQube /property:Platform=x86 /target:Rebuild

REM
REM Run unit tests
REM
OpenCppCoverage.exe ^
   --continue_after_cpp_exception --cover_children ^
   --sources ulib --sources test --sources include\ulib ^
   --excluded_sources test ^
   --excluded_sources packages\boost ^
   --export_type SonarQube:ulib-coverage.xml ^
   --export_type html:CoverageReport ^
   --modules test.dll ^
   -- "%VSINSTALL%\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" ^
   "..\bin\Release\Win32\test\test.dll" /Platform:x86 /InIsolation /logger:trx

SonarScanner.MSBuild.exe end /d:"sonar.login=%SONARLOGIN%"

:end

pause
