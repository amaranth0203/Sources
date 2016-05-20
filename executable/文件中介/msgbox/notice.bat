@echo off

:start
for /f "delims=" %%i in ('type flag') do (set f=%%i)

echo %f:~0,1%
if not %f:~0,1% == 0 if not %f:~0,1% == 1 goto :exec
if %f:~0,1% == 0 goto :start
if %f:~0,1% == 1 goto :msgbox

choice /t 1 /d y /n > nul
goto :start

:msgbox
mshta "javascript:new ActiveXObject('WScript.Shell').Run('cmd /c mshta vbscript:msgbox(%"%"ta-da%"%",,%"%"wassup%"%")(window.close) & taskkill /im mshta.exe /f',0);window.close()"
echo 0 > flag
goto :start

:exec
REM echo %f%
REM pause
REM cmd /c %f%
explorer %f%
echo 0 > flag
goto :start