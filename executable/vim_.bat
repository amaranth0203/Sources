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

if exist "%UserProfile%/qyh_session_vim.vim" (
    "%VIM_EXE_DIR%/vim.exe" -u %UserProfile%/.qyh_vim/vimrc -S "%UserProfile%/qyh_session_vim.vim"  %*
) else (
    echo. 2> %UserProfile%/qyh_session_vim.vim
    "%VIM_EXE_DIR%/vim.exe" -u %UserProfile%/.qyh_vim/vimrc -S "%UserProfile%/qyh_session_vim.vim"  %*
)
