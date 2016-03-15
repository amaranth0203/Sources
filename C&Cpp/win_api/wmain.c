#include <windows.h>
#include <wchar.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPTSTR  pCmdLine, int nCmdShow) {

    PDWORD cChars = NULL ;
    // HANDLE std = GetStdHandle( STD_OUTPUT_HANDLE ) ;
    HANDLE std = GetCommandLineW( ) ;

    if( std == INVALID_HANDLE_VALUE ) {
        wprintf( L"Cannot retrieve standard output handle\n (%d)" ,
            GetLastError( ) ) ;
    }

    WriteConsoleW( std , argv[1] , wcslen( argv[1] ) ) ;

    // return EXIT_SUCCESS ;

}