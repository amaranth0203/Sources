    .386
    .model   flat , c
    option  casemap:none

include     windows.inc
include     user32.inc
includelib  user32.lib
include     kernel32.inc
includelib  kernel32.lib

    .data
hStdOut         dd  ?
szBuffer        dd  5 dup (0)
dwBytesRead     dd  ?
dwBytesWrite    dd  ?
; dwArray         dd  '4' , '5' , '2' , '7' , '2' , '4' , '1' , '9' , '9' , '2' , '0' , '2' , '0' , '3' , '0' , '5' , '1' , '1' , 0 
dwArray         dd  1 , 2 , 0
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

_fibnacci       proc    param
        local tmp:dword
        .if param == 0
            mov     eax , 0
            ret
        .elseif param == 1
            mov     eax , 1
            ret
        .else
            mov     ebx , param
            dec     ebx
            invoke  _fibnacci , ebx
            mov     tmp , eax
            mov     ebx , param
            dec     ebx
            dec     ebx
            invoke  _fibnacci , ebx
            add     tmp , eax
            mov     eax , tmp
            ret
        .endif
_fibnacci       endp

_max            proc    param
        local   tmp:dword
        mov     tmp , 0
        _ss:
        cmp     param , 0
        je      ee
        cmp     tmp , param
        jng     _lt
        inc     param
        jmp     _ss
        _lt:
        mov     tmp , param
        inc     param
        jmp     _ss
        ee:
        ret
_max            endp

        
start :
        invoke  GetStdHandle , STD_OUTPUT_HANDLE
        mov     hStdOut , eax
        invoke  SetConsoleCtrlHandler , offset _CtrlHandler , TRUE

        ; invoke  _fibnacci , 9
        invoke  _max , dwArray
        invoke  wsprintf , offset szBuffer , offset szFmt , eax
        invoke  lstrlen , offset szBuffer
        mov     dwBytesRead , eax
        invoke  WriteConsole , 
                    hStdOut , 
                    offset szBuffer , 
                    dwBytesRead , 
                    offset dwBytesWrite , 
                    NULL

        invoke  ExitProcess , NULL
        end start