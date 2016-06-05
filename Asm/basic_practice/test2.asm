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
dwArray         dd  4 , 5 , 2 , 7 , 2 , 4 , 1 , 9 , 9 , 2 , 0 , 2 , 0 , 3 , 0 , 5 , 1 , 1 , 'x'
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
        mov     esi , param
        mov     eax , 0
        _ss:
        cmp     dword ptr[ esi ] , 'x'
        je      ee
        cmp     eax , dword ptr[ esi ]
        jng     _lt
        add     esi , 4
        jmp     _ss
        _lt:
        mov     eax , dword ptr[ esi ]
        add     esi , 4
        jmp     _ss
        ee:
        ret
_max            endp

; _min            proc    param
_min :
        ; mov     esi , param
        mov     esi , dword ptr[ esp + 4 ]
        mov     eax , 9
        _ss:
        cmp     dword ptr[ esi ] , 'x'
        je      ee
        cmp     eax , dword ptr[ esi ]
        jg      _gt
        add     esi , 4
        jmp     _ss
        _gt:
        mov     eax , dword ptr[ esi ]
        add     esi , 4
        jmp     _ss
        ee:
        ret
; _min            endp

        
start :
        invoke  GetStdHandle , STD_OUTPUT_HANDLE
        mov     hStdOut , eax
        invoke  SetConsoleCtrlHandler , offset _CtrlHandler , TRUE

        ; invoke  _fibnacci , 40
        ; invoke  _max , offset dwArray
        push    offset  dwArray
        call    _min

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