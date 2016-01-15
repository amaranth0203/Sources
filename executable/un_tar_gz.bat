@echo off
setlocal enabledelayedexpansion
set file=%1
set file=!file::=!
set file=!file:\=/!
set file=/cygdrive/!file!
REM echo !file!
tar -zxvf !file!
pause