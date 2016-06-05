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
dwArray         dd  4 , 5 , 2 , 7 , 2 , 4 , 1 , 9 , 9 , 2 , 0 , 2 , 0 , 3 , 0 , 5 , 1 , 1 , 'x'

    .const
szIntFmt        db  '%d' , 0
szCharFmt       db  '%c' , 0

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

_min            proc    param
        mov     esi , param
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
_min            endp

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

_print_char_array    proc    param
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

_print_int_array    proc    param
        mov     esi , param
        _ss:
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

_swap           proc , ptr1 , ptr2
        mov     esi , ptr1
        mov     eax , dword ptr[ esi ]
        mov     esi , ptr2
        mov     ebx , dword ptr[ esi ]
        mov     esi , ptr2
        mov     dword ptr[ esi ] , eax
        mov     esi , ptr1
        mov     dword ptr[ esi ] , ebx
        ret
_swap           endp

_bubble_sort    proc    param
        local   tmp
        mov     esi , param
        _ss:
        cmp     dword ptr[ esi ] , 'x'
        je      _ee
        push    esi
        ; sort inc or dec
        ; invoke  _max , esi
        invoke  _min , esi
        ; sort inc or dec end
        pop     esi
        mov     tmp , eax
        ; find max in right area and swap to mid
        mov     ecx , 0
        _sss:
        cmp     dword ptr[ esi + ecx*4 ] , 'x'
        je      _eee
        mov     eax , dword ptr[ esi + ecx*4 ]
        cmp     eax , tmp
        jne     _ne
        lea     ebx , dword ptr[ esi + ecx*4 ]
        push    esi
        invoke  _swap , esi , ebx
        pop     esi
        jmp     _eee
        _ne:
        inc     ecx
        jmp     _sss
        _eee:
        ; find max in right area and swap to mid end
        add     esi , 4
        jmp     _ss
        _ee:
        ret
_bubble_sort    endp

_process_input  proc    param
        mov     esi , param
        mov     edi , offset szTargetBuffer
        xor     eax , eax
        _ss:
        cmp     byte ptr[ esi ] , 'x'
        je      _ee
        cmp     byte ptr[ esi ] , ' '
        jne      _nespace
        mov     dword ptr[ edi ] , eax
        add     edi , 4
        ; pushad
        ; invoke  _print_int , eax
        ; invoke  _print_char , 10
        ; popad
        xor     eax , eax
        inc     esi
        jmp     _ss
        _nespace:
        cmp     byte ptr[ esi ] , '0' - 1
        jg      _gt
        inc     esi
        jmp     _ss
        _gt:
        cmp     byte ptr[ esi ] , '9'
        jng     _ng
        inc     esi
        jmp     _ss
        _ng:
        mov     bx , 10
        mul     bx
        xor     ebx , ebx
        mov     bl , byte ptr[ esi ]
        sub     bl , 48
        add     eax , ebx
        ; pushad
        ; invoke  _print_int , eax
        ; invoke  _print_char , 10
        ; popad
        inc     esi
        jmp     _ss
        _ee:
        mov     dword ptr[ edi ] , 'x'
        invoke  _bubble_sort , offset szTargetBuffer
        invoke  _print_int_array , offset szTargetBuffer
        ret
_process_input  endp

        
start :





        invoke  GetStdHandle , STD_INPUT_HANDLE
        mov     hStdIn , eax
        invoke  GetStdHandle , STD_OUTPUT_HANDLE
        mov     hStdOut , eax
        invoke  SetConsoleCtrlHandler , offset _CtrlHandler , TRUE
        invoke  ReadConsole , 
                    hStdIn , 
                    offset szRawBuffer , 
                    sizeof szRawBuffer , 
                    offset dwBytesRead , 
                    NULL
        invoke  _process_input , offset szRawBuffer

        ; invoke  _print_char_array , offset dwArray
        ; invoke  _bubble_sort , offset dwArray
        ; invoke  _print_char_array , offset dwArray

        invoke  ExitProcess , NULL
        end start