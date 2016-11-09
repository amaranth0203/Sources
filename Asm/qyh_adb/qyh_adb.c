//http://blog.163.com/lixiangqiu_9202/blog/static/53575037201272211216317/
#include <windows.h>
#include "qyh_adb.h"

int _ProcWinMain( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam ) 
{
  DefWindowProc( hWnd , uMsg , wParam , lParam ) ;
  return 0 ;
}


//int main(int argc, char *argv[])
int WINAPI WinMain( HINSTANCE hInstance ,
                    HINSTANCE hprevinstance ,
                    LPSTR line ,
                    int cmd ) {
  WNDCLASSEX stWndClass ;
  MSG stMsg ;
  /* void* hAccelerator ; */
  /* void* hMenu ; */
  void* hWinMain ;
  int rc ;
  puts( "1" ) ;
  // hInstance = GetModuleHandle ;
  // hMenu = LoadMenu( hInstance , IDM_MAIN ) ;
  // hAccelerator = LoadAccelerators( hInstance , IDA_MAIN ) ;
  RtlZeroMemory( &stWndClass , sizeof( stWndClass ) ) ;
  stWndClass.hIcon = LoadIcon( hInstance , ICO_MAIN ) ;
  stWndClass.hIconSm = stWndClass.hIcon ;
  stWndClass.hCursor = LoadCursor( 0 , IDC_ARROW ) ;
  stWndClass.hInstance = hInstance ;
  stWndClass.cbSize = sizeof( WNDCLASSEX ) ;
  stWndClass.style = CS_HREDRAW | CS_VREDRAW ;
  stWndClass.lpfnWndProc = ( void* )_ProcWinMain ;
  stWndClass.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 ) ;
  stWndClass.lpszClassName = "szClassName" ;
  RegisterClassEx( &stWndClass ) ;
  puts( "2" ) ;
  hWinMain = CreateWindowEx( WS_EX_CLIENTEDGE ,
                             "szClassName" ,
                             "szCaptionMain" ,
                             WS_OVERLAPPEDWINDOW ,
                             100 , 100 , 400 , 300 ,
                             NULL , NULL , hInstance , NULL ) ;
  puts( "3" ) ;
  ShowWindow( hWinMain , SW_SHOWNORMAL ) ;
  puts( "4" ) ;
  UpdateWindow( hWinMain ) ;
  puts( "5" ) ;
  while( 1 ) {
    rc = GetMessage( &stMsg , NULL , 0 , 0 ) ;
    if( rc == 0 ) break ;
    // rc = TranslateAccelerator( hWinMain , ( HACCEL )&hAccelerator , &stMsg ) ;
    // if( rc == 0 ) {
      TranslateMessage( &stMsg ) ;
      DispatchMessage( &stMsg ) ;
    // }
  }
  puts( "6" ) ;
  ExitProcess( 0 ) ;
  puts( "7" ) ;
  return 0;
}
