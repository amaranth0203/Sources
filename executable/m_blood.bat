@echo off
setlocal enabledelayedexpansion

if "%identity%" == "Shadow" (
    @set PATH=D:\cygwin64\bin;!PATH!
    @set tmp_identity=!identity!
    @set identity=
    @set identity=!tmp_identity!
    @call "D:\cygwin64\bin\mintty.exe" -c "d:/cygwin64/home/Ending/.minttyrc_blood" 
)
if "%identity%" == "vivo_work" (
    @set PATH=D:\cygwin64\bin;!PATH!
    @set tmp_identity=!identity!
    @set identity=
    @set identity=!tmp_identity!
    @call "D:\cygwin64\bin\mintty.exe" -c "d:/cygwin64/home/administrator/.minttyrc_blood" 
)