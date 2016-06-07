
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
szBuffer        dd  1024 dup (0)
szRawBuffer     dd  1024 dup (0)
szTargetBuffer  dd  1024 dup (0)
dwBytesRead     dd  ?
dwBytesWrite    dd  ?
dwArray         dd  4 , 5 , 2 , 7 , 2 , 4 , 'x'

    .const
szIntFmt        db  '%d' , 0
szCharFmt       db  '%c' , 0

    .code
_CtrlHandler    proc    _dwCtrlType
        mov eax , _dwCtrlType
        .if eax ==  CTRL_C_EVENT || eax == CTRL_BREAK_EVENT
            invoke  ExitProcess , NULL
        .endif
        mov eax , TRUE
        ret
_CtrlHandler    endp

_init_stdio     proc
        invoke  GetStdHandle , STD_INPUT_HANDLE
        mov     hStdIn , eax
        invoke  GetStdHandle , STD_OUTPUT_HANDLE
        mov     hStdOut , eax
        invoke  SetConsoleCtrlHandler , offset _CtrlHandler , TRUE
        ret
_init_stdio     endp

_print_char     proc    param
        invoke  wsprintf , offset szBuffer , offset szCharFmt , param
        invoke  lstrlen , offset szBuffer
        mov     dwBytesRead , eax
        invoke  WriteConsole , 
                    hStdOut , 
                    offset szBuffer , 
                    dwBytesRead , 
                    offset dwBytesWrite , 
                    NULL
        ret
_print_char     endp

_print_int      proc    param
        invoke  wsprintf , offset szBuffer , offset szIntFmt , param
        invoke  lstrlen , offset szBuffer
        mov     dwBytesRead , eax
        invoke  WriteConsole , 
                    hStdOut , 
                    offset szBuffer , 
                    dwBytesRead , 
                    offset dwBytesWrite , 
                    NULL
        ret
_print_int      endp

_print_char_array    proc   uses esi    param
        mov     esi , param
        _ss:
        cmp     dword ptr[ esi ] , 'x'
        je      _ee
        mov     eax , dword ptr[ esi ]
        invoke  _print_char , eax
        add     esi , 4
        jmp     _ss
        _ee:
        invoke  _print_char , 10
        ret
_print_char_array    endp

_print_int_array    proc    uses esi    param
        mov     esi , param
        _ss:
        xor     eax , eax
        cmp     dword ptr[ esi ] , 'x'
        je      _ee
        mov     eax , dword ptr[ esi ]
        invoke  _print_int , eax
        invoke  _print_char , ' '
        add     esi , 4
        jmp     _ss
        _ee:
        invoke  _print_char , 10
        ret
_print_int_array    endp

start :
        invoke  _init_stdio

        invoke  _print_int_array , offset dwArray

        invoke  ExitProcess , NULL
        end start