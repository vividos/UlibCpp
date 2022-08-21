@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2022 Michael Fink
REM
REM Cleans project folders for archiving
REM

REM set this to your Visual Studio installation folder
set VSINSTALL=%ProgramFiles%\Microsoft Visual Studio\2022\Community

REM
REM Preparations
REM
call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat"

REM
REM Cleaning
REM
msbuild UlibCpp.sln /m /property:Configuration=Debug /property:Platform=x86 /target:Clean
msbuild UlibCpp.sln /m /property:Configuration=Debug /property:Platform=x86 /target:Clean
msbuild UlibCpp.sln /m /property:Configuration=Release /property:Platform=x64 /target:Clean
msbuild UlibCpp.sln /m /property:Configuration=Release /property:Platform=x64 /target:Clean

rmdir .vs /s /q
rmdir .\bw-output /s /q
rmdir .\.sonarqube /s /q
rmdir CoverageReport /s /q
del LastCoverageResults.log
del ulib-coverage.xml
rmdir TestResults /s /q
rmdir packages /s /q
rmdir doxygen\obj /s /q
rmdir .\cov-int /s /q
del cov-int.zip

rmdir ..\bin /s /q
rmdir ..\intermediate /s /q

pause
