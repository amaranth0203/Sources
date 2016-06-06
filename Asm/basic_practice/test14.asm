
; 整型数组 以空格为间隔符 以字符 'x' 为结束符
; 向数组的 头/尾/index 处插入元素
; 删除数组 头/尾/index 处的元素
; 没有进行下标的容错处理
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
dwArray         dd  4 , 5 , 2 , 7 , 2 , 4 , 'x' , 128 dup ( 0 ) 

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

_array_size         proc uses esi lpArray
        mov     esi , lpArray
        mov     eax , 0
        _ss:
        cmp     dword ptr[ esi + eax*4 ] , 'x'
        je      _ee
        inc     eax
        jmp     _ss
        _ee:
        ret
_array_size         endp

_insert_to_index    proc uses ecx esi lpArray , index , value
        mov     esi , lpArray
        mov     ecx , index
        _push_start:
        push    dword ptr[ esi + ecx * 4 ] 
        cmp     dword ptr[ esi + ecx * 4 ] , 'x'
        je      _insert_start
        inc     ecx
        jmp     _push_start
        _insert_start:
        inc     ecx
        inc     ecx
        _pop_loop:
        dec     ecx
        pop     dword ptr[ esi + ecx * 4 ] 
        mov     eax , ecx
        dec     eax
        cmp     eax , index
        jne     _pop_loop
        dec     ecx
        mov     eax , value
        mov     dword ptr[ esi + ecx * 4 ] , eax
        ret
_insert_to_index    endp

_delete_element     proc uses esi ecx edx lpArray , index
        mov     esi , lpArray
        mov     ecx , index
        lea     edx , dword ptr[ ecx + 1 ]
        _ss:
        cmp     dword ptr[ esi + ecx*4 ] , 'x'
        je      _ee
        mov     eax , dword ptr[ esi + edx*4 ]
        mov     dword ptr[ esi + ecx*4 ] , eax
        inc     ecx
        inc     edx
        jmp     _ss
        _ee:
        ret
_delete_element     endp

_insert_to_head     proc    lpArray , value
        invoke  _insert_to_index , lpArray , 0 , value
        ret
_insert_to_head     endp


_insert_to_tail     proc lpArray , value
        invoke  _array_size , lpArray
        invoke  _insert_to_index , lpArray , eax , value
        ret
_insert_to_tail     endp

_delete_head        proc lpArray 
        invoke  _delete_element , lpArray , 0
        ret
_delete_head        endp

_delete_tail        proc lpArray
        invoke  _array_size , lpArray
        dec     eax
        invoke  _delete_element , lpArray , eax
        ret
_delete_tail        endp

_main   proc
        invoke  _print_int_array , offset dwArray
        invoke  _insert_to_head , offset dwArray , 10
        invoke  _print_int_array , offset dwArray
        invoke  _insert_to_tail , offset dwArray , 11
        invoke  _print_int_array , offset dwArray
        invoke  _insert_to_index , offset dwArray , 3 , 12
        invoke  _print_int_array , offset dwArray
        invoke  _delete_element , offset dwArray , 3
        invoke  _print_int_array , offset dwArray
        invoke  _delete_tail , offset dwArray
        invoke  _print_int_array , offset dwArray
        invoke  _delete_head , offset dwArray
        invoke  _print_int_array , offset dwArray
        ret
_main   endp

start :
        invoke  _init_stdio

        invoke  _main

        invoke  ExitProcess , NULL
        end start