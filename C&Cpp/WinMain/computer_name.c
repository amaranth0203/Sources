#include <windows.h>
#include <wchar.h>

int WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , 
    PSTR szCmdLine , int CmdShow ) { 
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1] ;
    DWORD size = sizeof( computerName ) / sizeof( computerName[0] ) ;
    int r = GetComputerNameW( computerName , &size ) ;
    if( r == 0 ) {
        wprintf( L"[-] Failed to get computer name %ld" , GetLastError( ) ) ;
        return 1 ;
    }
    wprintf( L"[+] Computer name : %ls\n" , computerName ) ;
    wchar_t str[] = L"aaaa °¡  aaa  " ;
    wprintf( str ) ;
    return 0 ;
}