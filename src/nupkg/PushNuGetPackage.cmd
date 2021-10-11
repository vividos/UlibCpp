@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2021 Michael Fink
REM
REM Pushes NuGet package to NuGet.org
REM

if "%NUGET_APIKEY%" == "" echo "Environment variable NUGET_APIKEY is not set! Obtain a new API key under and set the environment variable!"
if "%NUGET_APIKEY%" == "" echo "Note: API keys are managed under https://www.nuget.org/account/apikeys"
if "%NUGET_APIKEY%" == "" exit 1

nuget setApiKey %NUGET_APIKEY%
nuget push Vividos.UlibCpp.Static.4.2.5.nupkg -Source https://www.nuget.org/api/v2/package

pause
