@echo off
setlocal enabledelayedexpansion

if "%*" equ "" (
    echo %0 [push_lib^|kill_camera^|start_camera^|take_picture]
)

:start
if "%1" neq "" (
    if "%1" equ "push_lib" (
        call :push_lib 
    )
    if "%1" equ "kill_camera" (
        call :kill_camera
    )
    if "%1" equ "start_camera" (
        call :start_camera
    )
    if "%1" equ "take_picture" (
        call :take_picture
    )
    shift
    goto :start
)
goto :eof

rem
rem     set the value of command 
rem     belongs to take_picture 
rem     in file qyh.ini first
rem
:take_picture
set "cmd="
call :read_ini take_picture command cmd
echo !cmd!
!cmd!
goto :eof

rem
rem     set the value of command 
rem     belongs to start_camera 
rem     in file qyh.ini first
rem
:start_camera
set "cmd="
call :read_ini start_camera command cmd
echo !cmd!
!cmd!
goto :eof

rem
rem     set the value of camera_thread_name 
rem     belongs to kill_camera 
rem     in file qyh.ini first
rem
rem     thread_name kill sequense : 
rem         left -> right
rem         up -> down
rem
:kill_camera
set "thread_name="
call :read_ini kill_camera camera_thread_name thread_name
call :kill_thread !thread_name!
goto :eof

rem
rem     parameter: thread name
rem     split with space
rem
:kill_thread
for %%i in ( %* ) do (
    set "cmd_ps=adb shell "ps ^| grep %%i""
    echo !cmd_ps!
    !cmd_ps!
    for /f "tokens=1,2 delims= " %%j in ( '!cmd_ps!' ) do (
        set "cmd_kill=adb shell kill -9 %%k"
        echo !cmd_kill!
        !cmd_kill!
    )
)
goto :eof

rem
rem     set the value of log_file 
rem     belongs to push_lib 
rem     in file qyh.ini first
rem
:push_lib
set "log_filename="
call :read_ini push_lib log_file log_filename
for /f "delims=" %%x in ( !log_filename! ) do (
    set "line=%%x"
    if "!line:~0,8!" equ "Install:" (
        for /f "tokens=2 delims= " %%i in ( "!line!" ) do ( set "target_file=%%i" )
        for %%F in ("!log_filename!") do set dirname=%%~dpF
        set "dirname=!dirname:\=/!"
        set "target_file=!dirname!!target_file!"

        set "temp_str=!line:/= !"
        set "flag=false"
        for %%i in ( !temp_str! ) do set "filename_only=%%i"
        for %%i in ( !temp_str! ) do ( 
            if "!flag!" equ "true" (
                set "target=!target_dir!
                set "target_dir=!target_dir!/%%i"
            )
            if "%%i" equ "system"  (
                set "flag=true"
                set "target_dir=/system"
            )
        )
        set "target_dir=!target!"

        set "cmd=adb push"
        set "cmd=!cmd! !target_file! !target_dir!"
        echo !cmd!
        !cmd!
    )
)
goto :eof


rem
rem     first parameter is area
rem     second paramter is key
rem     third parameter is variable to store the value
rem     example :
rem         [push_lib]
rem         log_file=e:\log.log
rem     use
rem     call :read_ini push_lib log_file filename
rem     echo !filename!
rem     the result is e:\log.log
rem
:read_ini
set "file=qyh.ini"
set "area=[%1]"
set "key=%2"
set "target_value="
set "flag=false"
for /f "delims=" %%i in ( %~dp0/!file! ) do (
    set "line=%%i"
    if "!flag!" equ "true" (
        if "!line:~0,1!" equ "[" (
            if not "!line!" equ "!area!" (
                goto :end_for
            )
        )
        for /f "tokens=1,2 delims==" %%j in ( "!line!" ) do (
            if "%%j" equ "!key!" (
                set "target_value=!target_value!%%k "
            )
        )
    )
    if "!line!" equ "!area!" (
        set "flag=true"
    )
)
:end_for
set "%3=!target_value!"
goto :eof

