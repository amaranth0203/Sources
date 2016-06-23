    .386
    .model flat , stdcall
    option casemap:none

include     windows.inc
include     user32.inc
include     kernel32.inc
include     gdi32.inc
include     comctl32.inc
include     comdlg32.inc
include     advapi32.inc
include     shell32.inc
include     masm32.inc
include     netapi32.inc
include     winmm.inc
include     ws2_32.inc
include     psapi.inc
include     mpr.inc
include     iphlpapi.inc
includelib  comctl32.lib
includelib  comdlg32.lib
includelib  gdi32.lib
includelib  user32.lib
includelib  kernel32.lib
includelib  advapi32.lib
includelib  shell32.lib
includelib  masm32.lib
includelib  netapi32.lib
includelib  winmm.lib
includelib  ws2_32.lib
includelib  psapi.lib
includelib  mpr.lib
includelib  iphlpapi.lib

ICO_MAIN    equ 1000
DLG_MAIN    equ 1000
IDC_INFO    equ 1001
IDM_MAIN    equ 2000
IDM_OPEN    equ 2001
IDM_EXIT    equ 2002
IDM_1       equ 4000
IDM_2       equ 4001
IDM_3       equ 4002

RESULT_MODULE   equ 5000
ID_TEXT1        equ 5001
ID_TEXT2        equ 5002
IDC_MODULETABLE equ 5003
IDC_OK          equ 5004
ID_STATIC       equ 5005
ID_STATIC1      equ 5006
IDC_BROWSE1     equ 5007
IDC_BROWSE2     equ 5008
IDC_THESAME     equ 5009

    .data
hInstance   dd  ?
hRichEdit   dd  ?
hWinMain    dd  ?
hWinEdit    dd  ?
dwCount     dd  ?
dwColorRed  dd  ?
hText1      dd  ?
hText2      dd  ?
hFile       dd  ?
hProcessModuleTable dd  ?
szFileName          db  MAX_PATH    dup( ? )
szFileNameOpen1     db  MAX_PATH    dup( 0 )
szFileNameOpen2     db  MAX_PATH    dup( 0 )
szResultColName1    db  'PE数据结构相关字段' , 0
szResultColName2    db  '文件1的值(H)' , 0
szResultColName3    db  '文件2的值(H)' , 0
szBuffer            db  256 dup( 0 ) , 0
bufTemp1            db  200 dup( 0 ) , 0
bufTemp2            db  200 dup( 0 ) , 0
szFilter1           db  'Excutable Files' , 0 , '*.exe;*.com' , 0
                    db  0

    .const
szDllEdit   db  'RichEd20.dll' , 0
szClassEdit db  'RichEdit20A' , 0
szFont      db  '宋体' , 0
szExtPe     db  'PE File' , 0 , '*.exe;*.dll;*.src;*.fon;*.drv' , 0
            db  'All Files( *.* )' , 0 , '*.*' , 0 , 0
szErr       db  '文件格式错误!' , 0
szErrFormat db  '这个文件不是PE格式文件!' , 0
szSuccess   db  '程序执行成功' , 0
szNotFound  db  '无法查找' , 0


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
