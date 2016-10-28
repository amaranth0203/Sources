#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <windows.h>

int main( ) {

  HANDLE handle ;
  handle = LoadLibrary( "sha1sum.dll" ) ;
  char* ( *get_sha1sum )( unsigned char* ) ;
  get_sha1sum = ( void* )GetProcAddress( handle , "get_sha1sum" ) ;
  printf( "[+] %s\n" , get_sha1sum( "" ) ) ;

  return 0 ;
  
}
