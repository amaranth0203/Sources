@echo off
set timetable=  650     720   740^
                750     830^
                840     920^
                935     1015^
                1025    1105^
                1115    1155^
                1440    1520^
                1535    1615^
                1625    1705^
                1830    1930    1950^
                2000    2045^
                2055    2140^
                2150    2230


:start
set t=%time::=%
set t=%t:~0,4%
for %%i in ( %timetable% ) do (
    if %%i==%t% (
        goto :ring
    )
)
choice /t 1 /d y /n > nul
goto :start


:ring
REM mshta "javascript:new ActiveXObject('WScript.Shell').Run('cmd /c wscript msgbox.vbs',0);window.close()"
mshta "javascript:new ActiveXObject('WScript.Shell').Run('cmd /c mshta vbscript:msgbox(%"%"ticktack%"%",,%"%"wassup%"%")(window.close) & taskkill /im mshta.exe /f',0);window.close()"
for /f %%. in ('forfiles /m "%~nx0" /c "cmd /c echo 0x07"') do set bell=%%.
echo %bell%
choice /t 0 /d y /n > nul
echo %bell%
choice /t 0 /d y /n > nul
echo %bell%
choice /t 0 /d y /n > nul
echo %bell%
choice /t 60 /d y /n > nul
goto :start


