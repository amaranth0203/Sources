
; 冒泡排序
; 输入数字 以空格为间隔符 以字符 'x' 为结束符
; 输出排序后的数字
; 排列方式到标记 wassup 处更改调用 max 或 min

; 输入示例 :
; 4 13 3 2 x
; 得到输出
; 2 3 4 13

;           2016.06.05


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

_max            proc    uses esi    param
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

_min            proc    uses esi    param
        mov     esi , param
        mov     eax , 7fffffffh
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

_swap           proc    uses esi ebx ptr1 , ptr2
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

_bubble_sort    proc    uses esi ecx ebx    param
        local   tmp
        mov     esi , param
        _ss:
        cmp     dword ptr[ esi ] , 'x'
        je      _ee
;wassup
        ; ; ; sort inc or dec
        ; invoke  _max , esi
        invoke  _min , esi
        ; ; ; sort inc or dec end
        mov     tmp , eax
        ; ; ; find max in right area and swap to mid
        mov     ecx , 0
        _sss:
        cmp     dword ptr[ esi + ecx*4 ] , 'x'
        je      _eee
        mov     eax , dword ptr[ esi + ecx*4 ]
        cmp     eax , tmp
        jne     _ne
        lea     ebx , dword ptr[ esi + ecx*4 ]
        invoke  _swap , esi , ebx
        inc     ecx
        jmp     _eee
        _ne:
        inc     ecx
        jmp     _sss
        _eee:
        ; ; ; find max in right area and swap to mid end
        add     esi , 4
        jmp     _ss
        _ee:
        ret
_bubble_sort    endp

_process_input  proc    uses esi ebx    param
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
        ; invoke  _print_int , eax
        ; invoke  _print_char , 10
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
        ; xor     ebx , ebx
        ; mov     bl , byte ptr[ esi ]
        movzx   ebx , byte ptr[ esi ]
        sub     bl , 48
        add     eax , ebx
        ; invoke  _print_int , eax
        ; invoke  _print_char , 10
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
        invoke  _

        invoke  ExitProcess , NULL
        end start