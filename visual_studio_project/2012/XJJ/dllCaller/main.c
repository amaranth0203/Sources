#include <stdio.h>
#include <winsock2.h>
#pragma comment( lib , "Ws2_32.lib" )
#include <string.h>
#include <stdlib.h>
#include "funcs.h"
//#define PRINTF( ... ) printf( __VA_ARGS__ )

int wmain(int argc, wchar_t* argv[]) {
  HINSTANCE hDll ;
  hDll = LoadLibraryA( "XJJ.dll" ) ;
  if( !hDll ) MessageBox( NULL , L"LoadLibraryA Ê§°Ü" , NULL , 0 ) ;
  getch( ) ;
  return 0 ;
}
