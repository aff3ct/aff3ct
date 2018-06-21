@echo off

IF "%THREADS%" NEQ "" goto End

set "THREADS=%NUMBER_OF_PROCESSORS%"

:End