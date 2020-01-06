@echo off
REM
REM ulib - a collection of useful classes
REM Copyright (C) 2000-2020 Michael Fink
REM
REM Runs doxygen to document sourcecode
REM

doxygen doxygen.cfg

REM don't break build when doxygen reports errors
exit 0
