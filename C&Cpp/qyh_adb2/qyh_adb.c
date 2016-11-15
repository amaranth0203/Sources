#include <windows.h>
#include "qyh_adb.h"
#include "resource.h"

LRESULT CALLBACK WndProc( HWND hwnd ,
                          UINT msg ,
                          WPARAM wParam ,
                          LPARAM lParam ) {
  switch( msg ) {
  case WM_CREATE : {
    HMENU hMenu , hSubMenu ;
    HICON hIcon , hIconSm ;

    hMenu = CreateMenu( ) ;

    hSubMenu = CreatePopupMenu( ) ;
    AppendMenu( hSubMenu ,
                MF_STRING ,
                ID_FILE_EXIT ,
                "E&xit" ) ;
    AppendMenu( hMenu ,
                MF_STRING | MF_POPUP ,
                ( UINT )hSubMenu ,
                "&File" ) ;

    hSubMenu = CreatePopupMenu( ) ;
    AppendMenu( hSubMenu ,
                MF_STRING ,
                ID_STUFF_GO ,
                "&Go" ) ;
    AppendMenu( hMenu ,
                MF_STRING | MF_POPUP ,
                ( UINT )hSubMenu ,
                "&Stuff" ) ;

    hSubMenu = CreatePopupMenu( ) ;
    AppendMenu( hSubMenu ,
                MF_STRING ,
                ID_HELP_ABOUT ,
                "&About" ) ;
    AppendMenu( hMenu ,
                MF_STRING | MF_POPUP ,
                ( UINT )hSubMenu ,
                "&Help" ) ;

    SetMenu( hwnd , hMenu ) ;
    break ;

    hIcon = LoadImage( NULL ,
                       "Main2.ico" ,
                       IMAGE_ICON ,
                       32 ,
                       32 ,
                       LR_LOADFROMFILE ) ;
    if( hIcon )
      SendMessage( hwnd ,
                   WM_SETICON ,
                   ICON_BIG ,
                   ( LPARAM )hIcon ) ;
    else
      MessageBox( hwnd ,
                  "[-] LoadImage" ,
                  "Error" ,
                  MB_OK | MB_ICONERROR ) ;

    hIconSm = LoadImage( NULL ,
                         "Main2.ico" ,
                         IMAGE_ICON ,
                         16 ,
                         16 ,
                         LR_LOADFROMFILE ) ;
    if( hIconSm )
      SendMessage( hwnd ,
                   WM_SETICON ,
                   ICON_SMALL ,
                   ( LPARAM )hIconSm ) ;
    else
      MessageBox( hwnd ,
                  "[-] SendMessage" ,
                  "Error" ,
                  MB_OK | MB_ICONERROR ) ;
  }
    break ;
  case WM_COMMAND : {
    switch( LOWORD( wParam ) ) {
    case ID_FILE_EXIT :
      PostMessage( hwnd , WM_CLOSE , 0 , 0 ) ;
      break ;
    case ID_STUFF_GO :
      break ;
    case ID_HELP_ABOUT : {
      int ret = DialogBox( GetModuleHandle ( NULL ) ,
                           MAKEINTRESOURCE( IDD_ABOUT ) ,
                           hwnd ,
                           AboutDlgProc ) ;
      if( ret == IDOK ) {
        /* MessageBox( hwnd , */
        /*             "Dialog Exited with IDOK" , */
        /*             "Notice" , */
        /*             MB_OK | MB_ICONINFORMATION ) ; */
      }
      else if( ret == IDCANCEL ) {
        /* MessageBox( hwnd , */
        /*             "Dialog Exited with IDCANCEL" , */
        /*             "Notice" , */
        /*             MB_OK | MB_ICONINFORMATION ) ; */
      }
      else if( ret == -1 ) {
        MessageBox( hwnd ,
                    "Dialog failed" ,
                    "Error" ,
                    MB_OK | MB_ICONINFORMATION ) ;
      }
    }
      break ;
    }
  }
    break ;
  case WM_LBUTTONDOWN : {
    char szFileName[ MAX_PATH ] ;
    HINSTANCE hInstance = GetModuleHandle( NULL ) ;
    GetModuleFileName( hInstance ,
                       szFileName ,
                       MAX_PATH ) ;
    MessageBox( hwnd ,
                szFileName ,
                "This program is : " ,
                MB_OK | MB_ICONEXCLAMATION ) ;
  }
    break ;
  case WM_CLOSE :
    DestroyWindow( hwnd ) ;
    break ;
  case WM_DESTROY :
    PostQuitMessage( 0 ) ;
    break ;
  default :
    return DefWindowProc( hwnd , msg , wParam , lParam ) ;
  }
}

BOOL CALLBACK AboutDlgProc( HWND hwnd ,
                            UINT Message ,
                            WPARAM wParam ,
                            LPARAM lParam ) {
  switch( Message ) {
  case WM_INITDIALOG :
    return TRUE ;
  case WM_COMMAND :
    switch( LOWORD( wParam ) ) {
    case IDOK :
      EndDialog( hwnd , IDOK ) ;
      break ;
    case IDCANCEL :
      EndDialog( hwnd , IDCANCEL ) ;
      break ;
    }
    break ;
  default :
    return FALSE ;
  }
  return TRUE ;
}
