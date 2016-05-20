@echo off
setlocal enabledelayedexpansion
:start


set "flag=false"
call :check_device flag
if "!flag!" == "false" (
    echo [-] check_device false
    goto :eof
)

adb vivoroot
choice /t 3 /d y /n >nul

set "flag=false"
call :check_root flag
if "!flag!" == "false" (
    echo [-] check_root false
    goto :start
)

:dir1
set "flag=false"
call :check_dir1 flag
if "!flag!" == "false" (
    echo [-] check_dir1 false
    adb shell "mkdir -p /system/app/AutoTestCamera/"
    goto :dir1
)

:dir2
set "flag=false"
call :check_dir2 flag
if "!flag!" == "false" (
    echo [-] check_dir2 false
    adb shell "mkdir -p /system/app/AutoTestCamera/lib/arm/"
    goto :dir2
)

:dir3
set "flag=false"
call :check_dir3 flag
if "!flag!" == "false" (
    echo [-] check_dir3 false
    adb shell "mkdir -p /sdcard/ATCamera/"
    goto :dir3
)

call :pushlib

adb reboot

goto :eof

:pushlib
set cmd="dir /b /s"
for /f "delims=" %%x in ( '!cmd!' ) do (
    set "line=%%x"
    if "!line:~-2!" equ "pk" (
        set "cmd=adb push !line! /system/app/AutoTestCamera/"
        echo !cmd!
        !cmd!
    )
    if "!line:~-2!" equ "so" (
        set "cmd=adb push !line! /system/app/AutoTestCamera/lib/arm/"
        echo !cmd!
        !cmd!
    )
    if "!line:~-2!" equ "ml" (
        set "cmd=adb push !line! /sdcard/ATCamera/"
        echo !cmd!
        !cmd!
    )
)
goto :eof

:check_dir1
set "cmd_check=adb shell ls -d /system/app/AutoTestCamera/"
set "success=/system/app/AutoTestCamera/"
set "failed="
set "%1=false"
echo !cmd_check!
for /f "delims=" %%i in ( '!cmd_check!' ) do (
    set "line=%%i"
    set "line=!line:~0,-1!"
    if "!line!" == "!success!" (
        set "%1=true"
        goto :eof
    ) 
)
set "%1=false"
goto :eof


:check_dir2
set "cmd_check=adb shell ls -d /system/app/AutoTestCamera/lib/arm/"
set "success=/system/app/AutoTestCamera/lib/arm/"
set "failed="
set "%1=false"
echo !cmd_check!
for /f "delims=" %%i in ( '!cmd_check!' ) do (
    set "line=%%i"
    set "line=!line:~0,-1!"
    if "!line!" == "!success!" (
        set "%1=true"
        goto :eof
    ) 
)
set "%1=false"
goto :eof


:check_dir3
set "cmd_check=adb shell ls -d /sdcard/ATCamera/"
set "success=/sdcard/ATCamera/"
set "failed="
set "%1=false"
echo !cmd_check!
for /f "delims=" %%i in ( '!cmd_check!' ) do (
    set "line=%%i"
    set "line=!line:~0,-1!"
    if "!line!" == "!success!" (
        set "%1=true"
        goto :eof
    ) 
)
set "%1=false"
goto :eof


:check_device
set "cmd_check=adb devices"
set "success=device"
echo !cmd_check!
for /f "delims=" %%i in ( '!cmd_check!' ) do (
    set "line=%%i"
    echo !line!
    if "!line:~0,24!" neq "List of devices attached" (
        if "!line:~-6,6!" == "device" (
            set "%1=true"
            goto :eof
        ) 
    )
)
set "%1=false"
goto :eof


:check_root
set "cmd_check=adb remount"
set "success=remount succeeded"
set "failed="
set "%1=false"
echo !cmd_check!
for /f "delims=" %%i in ( '!cmd_check!' ) do (
    set "line=%%i"
    echo !line!
    if "!line!" == "!success!" (
        set "%1=true"
        goto :eof
    ) 
)
set "%1=false"
goto :eof