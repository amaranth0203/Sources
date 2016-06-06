
; 以十进制的方式输出从 0 开始到 0ffffffh 的数字
; 按 ctrl-c 可以终止
;               2016.06.04

    .386
    .model   flat , c
    option  casemap:none

include     windows.inc
include     user32.inc
includelib  user32.lib
include     kernel32.inc
includelib  kernel32.lib

    .data
hStdIn          dd  ?
hStdOut         dd  ?
szBuffer        dd  5 dup (0)
dwBytesRead     dd  ?
dwBytesWrite    dd  ?

    .const
szFmt           db  '%d' , 13 , 10 , 0

    .code
_CtrlHandler    proc    _dwCtrlType
        pushad
        mov eax , _dwCtrlType
        .if eax ==  CTRL_C_EVENT || eax == CTRL_BREAK_EVENT
            invoke  ExitProcess , NULL
        .endif
        popad
        mov eax , TRUE
        ret
_CtrlHandler    endp

        
start :
        invoke  GetStdHandle , STD_INPUT_HANDLE
        mov     hStdIn , eax
        invoke  GetStdHandle , STD_OUTPUT_HANDLE
        mov     hStdOut , eax
        invoke  SetConsoleMode , hStdIn , ENABLE_LINE_INPUT or ENABLE_ECHO_INPUT or ENABLE_PROCESSED_INPUT
        invoke  SetConsoleCtrlHandler , offset _CtrlHandler , TRUE

        xor     ecx , ecx
        loo:
            pushad
            invoke  wsprintf , offset szBuffer , offset szFmt , ecx
            invoke  lstrlen , offset szBuffer
            mov     dwBytesRead , eax
            invoke  WriteConsole , 
                        hStdOut , 
                        offset szBuffer , 
                        dwBytesRead , 
                        offset dwBytesWrite , 
                        NULL
            .if !eax
                jmp out__
            .endif
            popad
            inc     ecx
            cmp     ecx , 0ffffffh
            JLE     loo 
        out__:

        invoke  ExitProcess , NULL
        end start