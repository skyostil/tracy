: This file enables calling ABLD and friends from Cygwin
@echo off

: Run bldmake
call bldmake bldfiles

: Run abld
call abld %1 %2 %3 %4 %5 %6 %7 %8 %9
