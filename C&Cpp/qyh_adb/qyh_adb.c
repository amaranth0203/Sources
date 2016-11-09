#include <windows.h>
#include <stdio.h>
#include "resource.h"

static HWND hWinMain ;
static void* hMenu ;

void _DisplayMenuItem( int _dwCommandId ) {
  char buf[100] ;
  sprintf( buf , "%08x" , _dwCommandId ) ;
  MessageBox( NULL , buf , "wassup" , MB_ICONEXCLAMATION | MB_OK ) ;
}

LRESULT CALLBACK _ProcWinMain( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam ) 
{
  HMENU hMenu , hSysMenu , hSubMenu ;
  switch( uMsg ) {
  case WM_CREATE :
    hSubMenu = GetSubMenu( hMenu , 0 ) ;
    hSysMenu = GetSystemMenu( hWnd , FALSE ) ;
    AppendMenu( hSysMenu , MF_SEPARATOR , 0 , NULL ) ;
    AppendMenu( hSysMenu , 0 , IDM_ABOUT , "szMenuAbout" ) ;
    break ;
  case WM_COMMAND :
    _DisplayMenuItem( wParam ) ;
    wParam &= 0x0000ffff ;
    if( wParam == IDM_EXIT ) {
      DestroyWindow( hWinMain ) ;
      PostQuitMessage( 0 ) ;
    }
    else if( wParam >= IDM_TOOLBAR && wParam <= IDM_STATUSBAR ) {
      int rc = GetMenuState( hMenu , wParam , MF_BYCOMMAND ) ;
      if( rc == MF_CHECKED ) {
        CheckMenuItem( hMenu , wParam , MF_UNCHECKED ) ;
      }
      else {
        CheckMenuItem( hMenu , wParam , MF_CHECKED ) ;
      }
          wchar_t err[256] ;
    memset( err , 0 , 256 ) ;
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);
    int msgboxID = MessageBoxW( NULL,
                                err,
                                (LPCWSTR)L"error",
                                MB_OK );
    }
    else if( wParam >= IDM_BIG && wParam <= IDM_DETAIL ) {
      CheckMenuItem( hMenu , IDM_BIG , IDM_DETAIL , wParam , MF_BYCOMMAND ) ;
    }
    break ;
  case WM_SYSCOMMAND :
    if( wParam == IDM_HELP ) {
      _DisplayMenuItem( wParam ) ;
    }
    else {
      DefWindowProc( hWnd , uMsg , wParam , lParam ) ;
    }
    break ;
  case WM_RBUTTONDOWN :
    POINT stPos ;
    GetCursorPos( &stPos ) ;
    TrackPopupMenu( hSubMenu , TPM_LEFTALIGN , stPos.x , stPos.y , 0 , hWnd , NULL ) ;
    /* wchar_t err[256] ; */
    /* memset( err , 0 , 256 ) ; */
    /* FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), */
    /*               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL); */
    /* int msgboxID = MessageBoxW( NULL, */
    /*                             err, */
    /*                             (LPCWSTR)L"error", */
    /*                             MB_OK ); */
    break ;
  case WM_CLOSE :
    DestroyWindow( hWinMain ) ;
    break ;
  case WM_DESTROY :
    PostQuitMessage( 0 ) ;
    break ;
  }
  return DefWindowProc( hWnd , uMsg , wParam , lParam ) ;
}

int WINAPI WinMain( HINSTANCE hInstance ,
                    HINSTANCE hPrevInstance ,
                    LPSTR lpCmdLine ,
                    int nCmdShow ) {
  WNDCLASSEX stWndClass ;
  MSG stMsg ;
  void* hAccelerator ;
  int rc ;
  hInstance = GetModuleHandle( NULL ) ;
  hMenu = LoadMenu( hInstance , IDM_MAIN ) ;
  hAccelerator = LoadAccelerators( hInstance , IDA_MAIN ) ;
  RtlZeroMemory( &stWndClass , sizeof( stWndClass ) ) ;
  stWndClass.hIcon = LoadIcon( NULL , ICO_MAIN ) ;
  stWndClass.hIconSm = stWndClass.hIcon ;
  stWndClass.hCursor = LoadCursor( 0 , IDC_ARROW ) ;
  stWndClass.hInstance = hInstance ;
  stWndClass.cbSize = sizeof( WNDCLASSEX ) ;
  stWndClass.style = CS_HREDRAW | CS_VREDRAW ;
  stWndClass.lpfnWndProc = _ProcWinMain ;
  stWndClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 ) ;
  stWndClass.lpszClassName = "szClassName" ;
  if( !RegisterClassEx( &stWndClass ) ) {
    MessageBox( NULL , "[-] RegisterClassEx" , "Error" , MB_ICONEXCLAMATION | MB_OK ) ;
    return 0 ;
  }
  hWinMain = CreateWindowEx( WS_EX_CLIENTEDGE ,
                             "szClassName" ,
                             "szCaptionMain" ,
                             WS_OVERLAPPEDWINDOW ,
                             100 , 100 , 400 , 300 ,
                             NULL , hMenu , hInstance , NULL ) ;
  if( hWinMain == NULL ) {
    printf( "%s\n" , GetLastError( ) ) ;
    MessageBox( NULL , "[-] CreateWindowEx" , "Eroor" , MB_ICONEXCLAMATION | MB_OK ) ;
    return 0 ;
  }
  ShowWindow( hWinMain , SW_SHOWNORMAL ) ;
  UpdateWindow( hWinMain ) ;
  while( 1 ) {
    rc = GetMessage( &stMsg , NULL , 0 , 0 ) ;
    if( rc == 0 ) break ;
    rc = TranslateAccelerator( hWinMain , hAccelerator , &stMsg ) ;
    if( rc == 0 ) {
      TranslateMessage( &stMsg ) ;
      DispatchMessage( &stMsg ) ;
    }
  }
  ExitProcess( 0 ) ;
  return 0;
}
