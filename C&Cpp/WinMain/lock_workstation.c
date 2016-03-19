#include <windows.h>
#include <wchar.h>
#pragma comment( lib , "user32.lib" )

int WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , 
    PSTR szCmdLine , int CmdShow ) { 
    int r = LockWorkStation( ) ;
    wprintf( L"哈哈哈哈\n" ) ;
    if( r == 0 ) {
        wprintf( L"[-] LockWorkStation() failed\n %d\n" , GetLastError( ) ) ;
        return 1 ;
    }
    return 0 ;
}