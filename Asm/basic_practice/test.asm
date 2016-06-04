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
szBuffer        db  5 dup (0)
dwBytesRead     dd  ?
dwBytesWrite    dd  ?
szMsgBuffer     dd  1024 dup (?)
dwMsgSize       dd  ?
szTipBuffer     dd  1024 dup (?)
dwTipSize       dd  ?
szTipFmt        db  '[+] wassup input [ %s ] to exit' , 13 , 10 , 0
szExitWord      db  'bye' , 0

    .const
szMsgFmt        db  '%d:%s' , 13 , 10 , 0
szNewLineFmt    db  '%s' , 13 , 10 , 0
szDebugFmt      db  '%d' , 0

    .code
_CtrlHandler    proc    _dwCtrlType
        pushad
        mov eax , _dwCtrlType
        .if eax ==  CTRL_C_EVENT || eax == CTRL_BREAK_EVENT
            invoke CloseHandle , hStdIn
        .endif
        popad
        mov eax , TRUE
        ret
_CtrlHandler    endp

_str_strip      proc    lpStr
        pushad
        mov     eax , lpStr
        mov     ebx , lpStr
        _ww :
        .if byte ptr[eax] == 10 ||            byte ptr[eax] == 13 ||            byte ptr[eax] == 13 ||            byte ptr[eax] == 1 ||            byte ptr[eax] == 2 ||            byte ptr[eax] == 3 ||            byte ptr[eax] == 4 ||            byte ptr[eax] == 5 ||            byte ptr[eax] == 6 ||            byte ptr[eax] == 7 ||            byte ptr[eax] == 8 ||            byte ptr[eax] == 9 
            inc     eax
            jmp     _ww
        .endif
        ; cmp     byte ptr[eax] , 10
        ; jne     _c1_end
        ; inc     eax
        ; jmp      _ww
        ; _c1_end :
        ; cmp     byte ptr[eax] , 13
        ; jne     _c2_end
        ; inc     eax
        ; jmp      _ww
        ; _c2_end :
        ; cmp     byte ptr[eax] , 5
        ; jne     _c3_end
        ; inc     eax
        ; jmp      _ww
        ; _c3_end :
        ; cmp     byte ptr[eax] , 3
        ; jne     _c4_end
        ; inc     eax
        ; jmp      _ww
        ; _c4_end :
        ; cmp     byte ptr[eax] , 4
        ; jne     _c5_end
        ; inc     eax
        ; jmp      _ww
        ; _c5_end :
        mov     cl , byte ptr[eax]
        mov     byte ptr[ebx] , cl
        inc     eax
        inc     ebx
        cmp     byte ptr[eax] , 0
        jne     _ww
        popad
        ret
_str_strip      endp

_str_to_hex     proc lpStr
        pushad

        mov     esi , lpStr
        _ss:
        xor     ecx , ecx
        mov     cl , byte ptr[esi]
        invoke  wsprintf , offset szBuffer , offset szDebugFmt , ecx
        invoke  MessageBox , NULL , offset szBuffer , NULL , MB_OK
        inc     esi
        cmp     byte ptr[esi] , 0
        jne     _ss

        popad
        ret
_str_to_hex     endp
        
start :
        invoke  GetStdHandle , STD_INPUT_HANDLE
        mov     hStdIn , eax
        invoke  GetStdHandle , STD_OUTPUT_HANDLE
        mov     hStdOut , eax
        ; invoke  _str_to_hex , offset szExitWord
        invoke  wsprintf , offset szTipBuffer , offset szTipFmt , offset szExitWord
        invoke  lstrlen , offset szTipBuffer 
        mov     dwTipSize , eax
        invoke  WriteConsole , 
                    hStdOut ,
                    offset szTipBuffer ,
                    dwTipSize ,
                    offset dwBytesWrite ,
                    NULL
        invoke  SetConsoleMode , hStdIn , ENABLE_LINE_INPUT or ENABLE_ECHO_INPUT or ENABLE_PROCESSED_INPUT
        invoke  SetConsoleCtrlHandler , offset _CtrlHandler , TRUE

        .while TRUE
            invoke  SetConsoleTextAttribute , hStdOut , FOREGROUND_RED or FOREGROUND_GREEN or FOREGROUND_BLUE
            invoke  ReadConsole , 
                        hStdIn , 
                        offset szBuffer , 
                        sizeof szBuffer , 
                        offset dwBytesRead , 
                        NULL
            .break  .if ! eax
            invoke  SetConsoleTextAttribute , hStdOut , FOREGROUND_BLUE or FOREGROUND_INTENSITY
            invoke  _str_strip , offset szMsgBuffer
            invoke  wsprintf , offset szMsgBuffer , offset szMsgFmt , dwBytesRead , offset szBuffer
            invoke  lstrlen , offset szMsgBuffer
            mov     dwMsgSize , eax
        ; invoke  _str_to_hex , offset szMsgBuffer
            invoke  WriteConsole , 
                        hStdOut , 
                        offset szMsgBuffer , 
                        dwMsgSize , 
                        offset dwBytesWrite , 
                        NULL
            ; invoke  wsprintf , offset szBuffer , offset szNewLineFmt , offset szBuffer
            ; invoke  lstrlen , offset szBuffer
            ; mov     dwBytesRead , eax
            ; invoke  WriteConsole , 
                        ; hStdOut , 
                        ; offset szBuffer , 
                        ; dwBytesRead , 
                        ; offset dwBytesWrite , 
                        ; NULL
        .endw
        invoke  ExitProcess , NULL
        end start