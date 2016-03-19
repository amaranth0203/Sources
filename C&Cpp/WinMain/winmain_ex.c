#include <windows.h>
/*
 *INT WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance ,
 *    PSTR szCmdLine , INT nCmdShow ) {
 */
int WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , 
    PSTR szCmdLine , int CmdShow ) {

    MessageBoxW( NULL , L"content" , L"Title" , MB_OK ) ;

    return EXIT_SUCCESS ;

}