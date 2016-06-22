    .386
    .model flat , stdcall
    option casemap:none

include     windows.inc
include     user32.inc
includelib  user32.lib
include     kernel32.inc
includelib  kernel32.lib
include     comdlg32.inc
includelib  comdlg32.lib

ICO_MAIN    equ 1000
DLG_MAIN    equ 1000
IDC_INFO    equ 1001
IDM_MAIN    equ 2000
IDM_OPEN    equ 2001
IDM_EXIT    equ 2002
IDM_1       equ 4000
IDM_2       equ 4001
IDM_3       equ 4002

    .data
hInstance   dd  ?
hRichEdit   dd  ?
hWinMain    dd  ?
hWinEdit    dd  ?
szFileName  db  MAX_PATH    dup( ? )

totalSize           dd  ?
lpMemory            dd  ?
lpServicesBuffer    dd  100 dup( 0 )
bufDisplay          dd  50  dup( 0 )
szBuffer            dd  200 dup( 0 )
lpszFilterFmt4      db  '%08x  ' , 0
lpszManyBlanks      db  '  ' , 0
lpszBlank           db  ' ' , 0
lpszSplit           db  '-' , 0
lpszScanFmt         db  '%02x' , 0
lpszHexArr          db  '0123456789ABCDEF' , 0
lpszReturn          db  0dh , 0ah , 0
lpszDoubleReturn    db  0dh , 0ah , 0dh , 0ah , 0
lpszOut1            db  '文件大小: %d" , 0
dwStop              db  0

    .const
szDllEdit   db  'RichEd20.dll' , 0
szClassEdit db  'RichEdit20A' , 0
szFont      db  '宋体' , 0

szExtPe     db  'PE File' , 0 , '*.exe;*.dll;*.src;*.fon;*.drv' , 0
            db  'All File(*.*)' , 0 , '*.*' , 0 , 0
szErr       db  '文件格式错误!' , 0
szErrFormat db  '操作文件时出现错误!' , 0


    .code

_init proc
    local   @stCf:CHARFORMAT

    invoke  GetDlgItem , hWinMain , IDC_INFO
    mov     hWinEdit , eax

    invoke  LoadIcon , hInstance , ICO_MAIN
    invoke  SendMessage , hWinMain , WM_SETICON , ICON_BIG , eax
    invoke  SendMessage , hWinEdit , EM_SETTEXTMODE , TM_PLAINTEXT , 0
    invoke  RtlZeroMemory , addr @stCf , sizeof @stCf 
    mov     @stCf.cbSize , sizeof @stCf
    mov     @stCf.yHeight , 9*20
    mov     @stCf.dwMask , CFM_FACE or CFM_SIZE or CFM_BOLD
    invoke  lstrcpy , addr @stCf.szFaceName , addr szFont
    invoke  SendMessage , hWinEdit , EM_SETCHARFORMAT , 0 , addr @stCf
    invoke  SendMessage , hWinEdit , EM_EXLIMITTEXT , 0 , -1
    ret
_init endp

_openFile   proc
    local   @stOF:OPENFILENAME
    local   @hFile , @hMapFile
    local   @bufTemp1
    local   @bufTemp2
    local   @dwCount
    local   @dwCount1
    local   @dwBlanks

    invoke  RtlZeroMemory , addr @stOF , sizeof @stOF
    mov     @stOF.lStructSize , sizeof @stOF
    push    hWinMain
    pop     @stOF.hwndOwner
    mov     @stOF.lpstrFilter , offset szExtPe
    mov     @stOF.lpstrFile , offset szFileName
    ret
_openFile   endp

_ProcDlgMain    proc uses ebx edi esi hWnd , wMsg , wParam , lParam
    mov     eax , wMsg
    .if     eax == WM_CLOSE
        invoke  EndDialog , hWnd , NULL
    .elseif eax == WM_INITDIALOG
        push    hWnd
        pop     hWinMain
        call    _init
    .elseif eax == WM_COMMAND
        mov     eax , wParam
        .if     eax == IDM_EXIT
            invoke  EndDialog , hWnd , NULL
        .elseif eax == IDM_OPEN
            invoke  _openFile
        .elseif eax == IDM_1
        .elseif eax == IDM_2
        .elseif eax == IDM_3
        .endif
    .else
        mov     eax , FALSE
        ret
    .endif
    mov     eax , TRUE
    ret
_ProcDlgMain    endp

start :
    invoke  LoadLibrary , offset szDllEdit
    mov     hRichEdit , eax
    invoke  GetModuleHandle , NULL
    mov     hInstance , eax
    invoke  DialogBoxParam , hInstance , DLG_MAIN , NULL , offset _ProcDlgMain , NULL
    invoke  FreeLibrary , hRichEdit
    invoke  ExitProcess , NULL
    end start
