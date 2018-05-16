#!/bin/sh 
#>nul 2>&1 || goto :batch_part 

# this code executes on unix system 
mkdir -p ${1}  
exit 0  

:batch_part 
@echo off 
set TARGETDIR=%1 
set TARGETDIR=%TARGETDIR:/=\%
set TARGETDIR=%TARGETDIR:"=%
IF NOT EXIST "%TARGETDIR%" mkdir "%TARGETDIR%"
