#include <windows.h>
#include <wchar.h>
#pragma comment( lib , "user32.lib" )

int WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , 
    PSTR szCmdLine , int CmdShow ) { 
    int x = GetSystemMetrics( SM_CXSCREEN ) ;
    int y = GetSystemMetrics( SM_CYSCREEN ) ;
    printf( "[+] %d %d \n" , x , y ) ;
    return 0 ;
}