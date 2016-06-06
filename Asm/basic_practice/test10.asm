
; 创建一个文件
; 文件名是当前系统时间 年月日_时分秒.tmp
; 文件的三个时间戳（创建 最后存取 最后写入）均为
;     当前系统时间 wYear - 1 之后填充进去的
;                                   2016.06.06

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
szFilename   db  128 dup( 0 )
time_s          SYSTEMTIME <>
time_f          FILETIME <>

    .const
szIntFmt        db  '%d' , 0
szCharFmt       db  '%c' , 0
szFilenameFmt       db  '%d%02d%02d_%02d%02d%02d.tmp' , 0

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

_main   proc
        local @hFile , @yyyy , @mm , @dd , @HH , @MM , @SS

        invoke  GetLocalTime , offset time_s
        movzx   eax , time_s.wYear
        mov     @yyyy , eax
        movzx   eax , time_s.wMonth
        mov     @mm , eax
        movzx   eax , time_s.wDay
        mov     @dd , eax
        movzx   eax , time_s.wHour
        mov     @HH , eax
        movzx   eax , time_s.wMinute
        mov     @MM , eax
        movzx   eax , time_s.wSecond
        mov     @SS , eax
        invoke  wsprintf , offset szFilename , offset szFilenameFmt ,
                    @yyyy , @mm , @dd , @HH , @MM , @SS

        invoke  CreateFile ,
                    offset szFilename ,
                    GENERIC_READ or GENERIC_WRITE ,
                    0 ,
                    NULL ,
                    CREATE_ALWAYS ,
                    FILE_ATTRIBUTE_NORMAL ,
                    NULL
		.if	eax ==	INVALID_HANDLE_VALUE
			invoke	_print_char , 'E'
			invoke	_print_char , 'r'
			invoke	_print_char , 'r'
			invoke	_print_char , 10
			ret
		.endif
        mov     @hFile , eax
        mov     ax , time_s.wYear
        dec     ax
        mov     time_s.wYear , ax
        invoke  SystemTimeToFileTime , offset time_s , offset time_f
        invoke  SetFileTime , @hFile , offset time_f , offset time_f , offset time_f
        invoke  CloseHandle , @hFile
        ret
_main   endp

start :
        invoke  _init_stdio

        invoke  _main

        invoke  ExitProcess , NULL
        end start