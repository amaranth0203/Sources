@echo off
setlocal enabledelayedexpansion
if "%identity%" == "Shadow" (
    set "VIM_EXE_DIR=D:\Program Files\Vim\vim74\"
)
if "%identity%" == "vivo_work" (
    set "VIM_EXE_DIR=D:\Program Files (x86)\Vim\vim74\"
)

if "%VIM_EXE_DIR%"=="" (
    echo cannot find VIM_EXE_DIR
    pause
    goto :eof
)

start "dummy" /b "%VIM_EXE_DIR%/gvim.exe" -u %UserProfile%/.qyh_vim/vimrc %*
