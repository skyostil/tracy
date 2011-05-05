: This file enables calling NMake from Cygwin
@echo off

: Load the Visual Studio environment variables
if "%VS71COMNTOOLS%" neq "" (
  call "%VS71COMNTOOLS%\vsvars32.bat"
)
if "%VS80COMNTOOLS%" neq "" (
  call "%VS80COMNTOOLS%\vsvars32.bat"
)

: Run NMake
call nmake %1 %2 %3 %4 %5 %6 %7 %8 %9
