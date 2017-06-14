@echo off
@rmdir /s /q dist\
@python pack.py py2exe
@xcopy dist c:\Users\Ending\Desktop\plc_test\ /f /s /e /y
@call common.bat :check_result
@pause