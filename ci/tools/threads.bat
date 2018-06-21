@echo off

IF "%THREADS%" NEQ "" goto End
set "THREADS=%NUMBER_OF_PROCESSORS%"
IF "%THREADS%"=="" set "THREADS=1"

:End