    .386
    .model   flat , stdcall
    option  casemap:none

include     windows.inc
include     user32.inc
includelib  user32.lib
include     kernel32.inc
includelib  kernel32.lib
include     stdio.lib

    .data
szBuffer    db  256 dup (?)

    .const
szCaption   db  'SendMessage' , 0
szStart     db  'Press OK to start SendMessage , param: %s' , 0
szText      db  'Text send to other windows' , 'ddddddd' , 0
szTemp      db  'aaaaaaaaaaaaaaaaaaaaaaaaaaaaa' , 0

    .code
start :
        invoke  printf , addr szStart , addr szText
        ; invoke  MessageBox , NULL , offset szBuffer , addr szCaption , MB_OK
        invoke  ExitProcess , NULL
        end start

