
; 获取系统时间
; 输出当前日期
; 三种格式
; dd-mm-yyyy
; mm-dd-yyyy
; dd/mm/yyyy
;            2016.06.06

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
dwBytesRead     dd  ?
dwBytesWrite    dd  ?
time_s          SYSTEMTIME <>

    .const
szIntFmt        db  '%d' , 0
szCharFmt       db  '%c' , 0
szTimeFmt1      db  '%02d-%02d-%d' , 10 , 0
szTimeFmt2      db  '%02d-%02d-%d' , 10 , 0
szTimeFmt3      db  '%02d/%02d/%d' , 10 , 0

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

        invoke  GetLocalTime , offset time_s
        movzx   eax , time_s.wYear
        push    eax
        movzx   eax , time_s.wMonth
        push    eax
        movzx   eax , time_s.wDayOfWeek
        push    eax
        movzx   eax , time_s.wDay
        push    eax
        movzx   eax , time_s.wHour
        push    eax
        movzx   eax , time_s.wMinute
        push    eax
        movzx   eax , time_s.wSecond
        push    eax
        ; invoke  _print_int , dword ptr[ esp + 6*4 ]
        ; invoke  _print_char , 10
        ; invoke  _print_int , dword ptr[ esp + 5*4 ]
        ; invoke  _print_char , 10
        ; invoke  _print_int , dword ptr[ esp + 4*4 ]
        ; invoke  _print_char , 10
        ; invoke  _print_int , dword ptr[ esp + 3*4 ]
        ; invoke  _print_char , 10
        ; invoke  _print_int , dword ptr[ esp + 2*4 ]
        ; invoke  _print_char , 10
        ; invoke  _print_int , dword ptr[ esp + 1*4 ]
        ; invoke  _print_char , 10
        ; invoke  _print_int , dword ptr[ esp + 0*4 ]
        ; invoke  _print_char , 10


        ; dd-mm-yyyy
        invoke  wsprintf , offset szBuffer , offset szTimeFmt1 ,
                    dword ptr[ esp + (3+2)*4 ] ,
                    dword ptr[ esp + (5+1)*4 ] ,
                    dword ptr[ esp + (6+0)*4 ] 
        invoke  lstrlen , offset szBuffer
        invoke WriteConsole , hStdOut , offset szBuffer , eax , offset dwBytesWrite , NULL


        ; mm-dd-yyyy
        invoke  wsprintf , offset szBuffer , offset szTimeFmt2 ,
                    dword ptr[ esp + (5+2)*4 ] ,
                    dword ptr[ esp + (3+1)*4 ] ,
                    dword ptr[ esp + (6+0)*4 ] 
        invoke  lstrlen , offset szBuffer
        invoke WriteConsole , hStdOut , offset szBuffer , eax , offset dwBytesWrite , NULL


        ; dd/mm/yyyy
        invoke  wsprintf , offset szBuffer , offset szTimeFmt3 ,
                    dword ptr[ esp + (3+2)*4 ] ,
                    dword ptr[ esp + (5+1)*4 ] ,
                    dword ptr[ esp + (6+0)*4 ] 
        invoke  lstrlen , offset szBuffer
        invoke WriteConsole , hStdOut , offset szBuffer , eax , offset dwBytesWrite , NULL

        invoke  ExitProcess , NULL
        end start