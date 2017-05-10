REM Generate .lib files using existing .dll files
REM Helpful to skip QtC building and use installed QtC binaries
REM First argument is a directory with dlls
REM Second argument is an output directory

setlocal ENABLEDELAYEDEXPANSION
@ECHO OFF
SET "INPUT_DIR=%1"
SET "OUTPUT_DIR=%2"
for /F %%x in ('dir /B/D %INPUT_DIR%\*.dll') do (
 call :makelib %%x %%~nx %INPUT_DIR%)

goto:End

:makelib
  set x=%1
  set NAME=%2
  set FP=%3
  echo !NAME!
  dumpbin /exports %FP%\%x% > !OUTPUT_DIR!\!NAME!_export.txt
  echo LIBRARY !NAME! > !OUTPUT_DIR!\!NAME!.def 
  echo EXPORTS >> !OUTPUT_DIR!\!NAME!.def 
  call :prepend !OUTPUT_DIR!\!NAME!
  lib /def:!OUTPUT_DIR!\!NAME!.def /out:!OUTPUT_DIR!\!NAME!.lib /machine:x86
goto:eof

:prepend
  for /f "skip=19 tokens=4" %%a in (%1_export.txt) do echo %%a >> %1.def
goto:eof
:End