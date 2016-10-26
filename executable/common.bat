@echo off

call %*
exit /b

:check_result
if not "%ERRORLEVEL%" == "0" (
    color 0c
    exit /b
)
if "%ERRORLEVEL%" == "0" (
    color 0a
    exit /b
)
exit /b

:test
set %1="aaaaaa"
exit /b

:error_msg
color 0c
echo\
echo ******************
echo  TOO BAD : %*
echo ******************
echo\
exit /b
