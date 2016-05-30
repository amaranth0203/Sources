    .386
    .model flat,stdcall
    option casemap:none

include     windows.inc
include     gdi32.inc
includelib  gdi32.lib
include     user32.inc
includelib  user32.lib
include     kernel32.inc
includelib  kernel32.lib

    .data?
hInstance   dd  ?
hWinMain    dd  ?

    .const
szClassName     db      'MyClass' , 0
szCaptionMain   db      'My First Window !' , 0
szText          db      'Win32 Assembly, Simple and powerful !' , 0

    .code
_ProcWinMain    proc    use ebx edi esi , hWnd , uMsg , wParam , lParam
                local   @stPs:PAINTSTRUCT
                local   @stRect:RECT
                local   @hDc

                mov     eax , uMsg
                .if     eax ==  WM_PAINT
                        invoke  BeginPaint , hWnd , addr @stPs
                        mov     @hDc , eax
                        invoke  GetClientRect , hWnd , addr @stRect
                        invoke  DrawText , @hDc , addr szText , -1 , \
                                addr @stRect , \
                                DT_SINGLELINE or DT_CENTER or DT_VCENTER
                        invoke  EndPaint , hWnd , addr @stPs
                .elseif eax ==  WM_CLOSE
                        invoke  DestoryWindow , hWinMain
                        invoke  PostQuitMessage , NULL
                .else
                        invoke  DefWindowProc , hWnd , uMsg , wParam , lParam
                        ret
                .endif
                xor     eax , eax
                ret
_ProcWinMain    endp

_WinMain        proc
                local   @stWndClass:WNDCLASSEX
                local   @stMsg:MSG
                invoke  GetModuleHandle , NULL
                mov     RtlZeroMemory , addr @stWndClass , sizeof @stWndClass
                invoke  LoadCur