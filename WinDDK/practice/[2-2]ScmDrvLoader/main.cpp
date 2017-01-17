#include <stdio.h>
#include <Windows.h>
#include "ScmDrvCtrl.h"

#pragma comment( lib , "user32.lib" )

void GetAppPath( char* szCurFile ) {
  GetModuleFileNameA( 0 , szCurFile , MAX_PATH ) ;
  for( SIZE_T i = strlen( szCurFile ) - 1 ; i >= 0 ; i -- ) {
    if( szCurFile[i] == '\\' ) {
      szCurFile[i+1] = '\0' ;
      break ;
    }
  }
}

int main( ) {
  BOOL b ;
  cDrvCtrl dc ;
  char szSysFile[MAX_PATH] = {0} ;
  char szSvcLnkName[] = "KrnlHW64" ;
  GetAppPath( szSysFile ) ;
  strcat( szSysFile , "KrnlHW64.sys" ) ;
  b = dc.Install( szSysFile , szSvcLnkName , szSvcLnkName ) ;
  b = dc.Start( ) ;
  printf( "LoadDriver=%d\n" , b ) ;
  dc.Open( "\\\\.\\KrnlHW64" ) ;
  DWORD x=100 , y=0 , z=0 ;
  dc.IoControl( 0x800 , &x , sizeof( x ) , &y , sizeof( y ) , &z ) ;
  printf( "INPUT=%ld\nOUTPUT=%ld\nReturnBytesLength=%ld\n" , x , y , z ) ;
  dc.IoControl( 0x801 , 0 , 0 , 0 , 0 , 0 ) ;
  CloseHandle( dc.m_hDriver ) ;
  b = dc.Stop( ) ;
  b = dc.Remove( ) ;
  printf( "UnloadDriver=%d\n" , b ) ;
  getchar( ) ;
  return 0 ;
}
