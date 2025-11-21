@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2025 Michael Fink
REM
REM Cleans project folders for archiving
REM

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
