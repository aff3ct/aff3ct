@echo off

IF "%THREADS%"=="" goto End

set "THREADS=%NUMBER_OF_PROCESSORS%"

:End