REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2017 Michael Fink
REM
REM Runs CppCheck to check sourcecode
REM

REM when started via command line, assume "no xml" and "current folder"
set INTDIR=%1
if "%INTDIR%" == "" set INTDIR=%CD%\

REM when xml is passed as second param, set format and output file
set FORMAT=
set OUTFILE=%INTDIR%cppcheck.txt
if "%2" == "xml" set FORMAT=--xml
if "%2" == "xml" set OUTFILE=%INTDIR%cppcheck-Results.xml

REM run cppcheck
REM -I <dir>            Include path
REM -i <dir>            Ignore path
REM --suppressions-list=<file>   File with suppressed warnings
REM -j 4                Multithreading
REM --platform=win32W   Platform specific types
REM --language=c++      Language (file extensions)
REM --std=c++11         Language (syntax)%
REM --enable=all        Enable warnings
REM --template vs       Output format for warnings
REM --check-config
cppcheck.exe ^
   ..\ulib ..\include -DWIN32 -D_WINDOWS -DNDEBUG -D_UNICODE -D__cplusplus ^
   --suppressions-list=cppcheck-suppress.txt --inline-suppr ^
   -j 4 --platform=win32W --language=c++ --std=c++11 %FORMAT% --enable=all --template vs 2> %OUTFILE%

REM don't break build when cppcheck reports errors
exit 0
