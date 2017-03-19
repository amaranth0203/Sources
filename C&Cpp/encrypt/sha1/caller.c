#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#ifdef _WIN32
#include <windows.h>
#define DLL_NAME "sha1sum.dll"
#else
#include <dlfcn.h>
#define DLL_NAME "sha1sum.so"
#endif
#define FUNC_NAME "get_sha1sum"

int main( ) {
#ifdef _WIN32
  HANDLE handle ;
  handle = LoadLibrary( ".\\"DLL_NAME ) ;
  char* ( *get_sha1sum )( unsigned char* ) ;
  get_sha1sum = ( void* )GetProcAddress( handle , FUNC_NAME ) ;
#else
  void* handle = NULL ;
  handle = dlopen( "./"DLL_NAME , RTLD_NOW ) ;
  char* ( *get_sha1sum )( unsigned char* ) =
    handle == NULL ?
    NULL : ( void* )dlsym( handle , FUNC_NAME ) ;
#endif
  
  if( NULL == get_sha1sum ) {
    printf( "handle , get_sha1sum : %p %p\n" , handle , get_sha1sum ) ;
    exit( -1 ) ;
  }
  printf( "[+] %s\n" , get_sha1sum( "" ) ) ;

  return 0 ;
}
