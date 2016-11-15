#include <windows.h>
#include "resource.h"
#include "qyh_adb.h"

const char g_szClassName[] = "qyh_adb" ;

int WINAPI WinMain( HINSTANCE hInstance ,
                    HINSTANCE hPrevInstance ,
                    LPSTR lpCmdLine ,
                    int nCmdShow ) {
  WNDCLASSEX wc ;
  HWND hwnd ;
  MSG Msg ;

  wc.cbSize = sizeof( WNDCLASSEX ) ;
  wc.style = 0 ;
  wc.lpfnWndProc = WndProc ;
  wc.cbClsExtra = 0 ;
  wc.cbWndExtra = 0 ;
  wc.hInstance = hInstance ;
  wc.hIcon = LoadIcon( GetModuleHandle( NULL ) ,
                       MAKEINTRESOURCE( IDI_MYICON ) ) ;
  wc.hIconSm = ( HICON )LoadIcon( NULL ,
                                  MAKEINTRESOURCE( IDI_MYICON ) ,
                                  IMAGE_ICON ,
                                  16 ,
                                  16 ,
                                  0 ) ;
  wc.hCursor = LoadCursor( NULL , IDC_ARROW ) ;
  wc.hbrBackground = ( HBRUSH )( COLOR_WINDOW +1 ) ;
  wc.lpszMenuName = MAKEINTRESOURCE( IDR_MYMENU ) ;
  wc.lpszClassName = g_szClassName ;

  if( !RegisterClassEx( &wc ) ) {
    MessageBox( NULL ,
                "[-] RegisterClassEx" ,
                "Error" ,
                MB_ICONEXCLAMATION | MB_OK ) ;
    return 0 ;
  }

  hwnd = CreateWindowEx( WS_EX_CLIENTEDGE ,
                         g_szClassName ,
                         "qyh_adb" ,
                         WS_OVERLAPPEDWINDOW ,
                         CW_USEDEFAULT ,
                         CW_USEDEFAULT ,
                         240 ,
                         120 ,
                         NULL ,
                         NULL ,
                         hInstance ,
                         NULL ) ;
  if( hwnd == NULL ) {
    MessageBox( NULL ,
                "[-] CreateWindowEx" ,
                "Error" ,
                MB_ICONEXCLAMATION | MB_OK ) ;
    return 0 ;
  }

  ShowWindow( hwnd , nCmdShow ) ;
  UpdateWindow( hwnd ) ;

  while( GetMessage( &Msg , NULL , 0 , 0 ) > 0 ) {
    TranslateMessage( &Msg ) ;
    DispatchMessage( &Msg ) ;
  }
  
  return Msg.wParam ;
}
