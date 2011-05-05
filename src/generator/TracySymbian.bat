: This file handles creating and compiling needed tracers
@echo off

: List current symbian projects and ask for user's choice
:BEGIN
mode con:cols=90 lines=45
CLS
echo -----------------[ Generating and compiling Tracy projects for symbian ]-----------------
echo.
REM ################################################################################### 
echo Select compilation options:
echo 1. Emulator ( sbs winscw udeb )
echo 2. Target ( sbs armv5 urel )
echo 3. Emulator ( abld winscw udeb )
echo 4. Target ( abld armv5 urel )
echo 5. Quit
echo.
CHOICE /N /C:12345 Selection: %1


IF ERRORLEVEL ==5 GOTO END
IF ERRORLEVEL ==4 GOTO TARGET_ABLD
IF ERRORLEVEL ==3 GOTO EMULATOR_ABLD
IF ERRORLEVEL ==2 GOTO TARGET_SBS
IF ERRORLEVEL ==1 GOTO EMULATOR_SBS


:EMULATOR_SBS
set comp=COMP=sbs ARCH=winscw CONF=udeb
GOTO GENERATION

:TARGET_SBS
set comp=COMP=sbs ARCH=armv5 CONF=urel
GOTO GENERATION

:EMULATOR_ABLD
set comp=COMP=abld ARCH=winscw CONF=udeb
GOTO GENERATION

:TARGET_ABLD
set comp=COMP=abld ARCH=armv5 CONF=urel
GOTO GENERATION

REM ################################################################################### 
:GENERATION
CLS
echo -----------------[ Generating and compiling Tracy projects for symbian ]-----------------
echo.
echo Select symbian tracer:
echo 1. gles11-vg11-vgu11-egl13-tracer-symbian
echo 2. gles20-vg11-vgu11-egl13-tracer-symbian
echo 3. vg11-vgu11-egl13-tracer-symbian
echo 4. gles11-egl11-tracer-symbian 
echo 5. vg11-vgu11-vgi10-tracer-symbian
echo 6. Quit
echo.
CHOICE /N /C:123456 Selection: %1
CLS
IF ERRORLEVEL ==6 GOTO END
IF ERRORLEVEL ==5 GOTO VG11-VGU11-VGI10-TRACER-SYMBIAN
IF ERRORLEVEL ==4 GOTO GLES11-EGL11-TRACER-SYMBIAN
IF ERRORLEVEL ==3 GOTO VG11-VGU11-EGL13-TRACER-SYMBIAN
IF ERRORLEVEL ==2 GOTO GLES20-VG11-VGU11-EGL13-TRACER-SYMBIAN
IF ERRORLEVEL ==1 GOTO GLES11-VG11-VGU11-EGL13-TRACER-SYMBIAN
GOTO END

REM ################################################################################### 
:GLES11-VG11-VGU11-EGL13-TRACER-SYMBIAN

echo Building GLES11-VG11-VGU11-EGL13-TRACER-SYMBIAN..

: delete common def file
rm config\cross-api\common\common.def

: Copy tracers def file as common def file
cp config\cross-api\gles-vg-vgu-egl\gles11-vg11-vgu11-egl13u.def config\cross-api\common\common.def

: Clean existing tracers
make clean

: Generate the needed tracers
make gles11-vg11-vgu11-egl13-tracer-symbian %comp%
make gles11-passthrough_tracer-symbian %comp%
make vg11-passthrough_tracer-symbian %comp%
make vgu11-passthrough_tracer-symbian %comp%
make egl13-passthrough_tracer-symbian %comp%
GOTO END

REM ###################################################################################
:GLES20-VG11-VGU11-EGL13-TRACER-SYMBIAN

echo Building GLES20-VG11-VGU11-EGL13-TRACER-SYMBIAN..

: delete common def file
rm config\cross-api\common\common.def

: Copy tracers def file as common def file
cp config\cross-api\gles-vg-vgu-egl\gles20-vg11-vgu11-egl13u.def config\cross-api\common\common.def

: Clean existing tracers
make clean

make gles20-vg11-vgu11-egl13-tracer-symbian %comp%
make gles20-passthrough_tracer-symbian %comp%
make vg11-passthrough_tracer-symbian %comp%
make vgu11-passthrough_tracer-symbian %comp%
make egl13-passthrough_tracer-symbian %comp%
GOTO END

REM ###################################################################################
:VG11-VGU11-EGL13-TRACER-SYMBIAN

echo Building VG11-VGU11-EGL13-TRACER-SYMBIAN..

: delete common def file
rm config\cross-api\common\common.def

: Copy tracers def file as common def file
cp config\cross-api\vg-vgu-egl\vg11-vgu11-egl13u.def config\cross-api\common\common.def

: Clean existing tracers
make clean

make vg11-vgu11-egl13-tracer-symbian %comp%
make vg11-passthrough_tracer-symbian %comp%
make vgu11-passthrough_tracer-symbian %comp%
make egl13-passthrough_tracer-symbian %comp%
GOTO END

REM ###################################################################################
:GLES11-EGL11-TRACER-SYMBIAN

echo Building GLES11-EGL11-TRACER-SYMBIAN..

: Clean existing tracers
make clean
make gles11-egl11-tracer-symbian %comp%
GOTO END

REM ###################################################################################
:VG11-VGU11-VGI10-TRACER-SYMBIAN

echo Building VG11-VGU11-VGI10-TRACER-SYMBIAN..

: delete common def file
rm config\cross-api\common\common.def

: Copy tracers def file as common def file
cp config\cross-api\vg-vgu-vgi\vg11-vgu11-vgi.def config\cross-api\common\common.def

: Clean existing tracers
make clean

make vg11-vgu11-vgi10-tracer-symbian %comp%
make vg11-passthrough_tracer-symbian %comp%
make vgu11-passthrough_tracer-symbian %comp%
make vgi10-passthrough_tracer-symbian %comp%
GOTO END

:END


