        .386
        .model flat , stdcall
        option casemap :none

include     windows.inc
include     user32.inc
include     kernel32.inc
includelib  user32.lib
includelib  kernel32.lib

ICO_MAIN    equ     1000h
DLG_MAIN    equ     1

            .data?
hInstance   dd      ?

            .code
_ProcDlgMain    proc    uses ebx edi esi hWnd , wMsg , wParam , lParam
        mov     eax , wMsg
        .if     eax == WM_CLOSE
                invoke  EndDialog , hWnd , NULL
        .elseif eax == WM_INITDIALOG
                invoke  LoadIcon , hInstance , ICO_MAIN
                invoke  SendMessage , hWnd , WM_SETICON , ICON_BIG , eax
        .elseif eax == WM_COMMAND
                mov     eax , wParam
                .if     ax == IDOK
                        invoke  EndDialog , hWnd , NULL
                .endif
        .else
                mov     eax , FALSE
                ret
        .endif
        mov     eax , TRUE
        ret
_ProcDlgMain    endp

start:
        invoke  GetModuleHandle , NULL
        mov     hInstance , eax
        invoke  DialogBoxParam , hInstance , DLG_MAIN , NULL , offset _ProcDlgMain , NULL
        invoke  ExitProcess , NULL
        end start