#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "funcs.h"
#include <Windows.h>

int wmain(int argc, wchar_t* argv[]) {
  DWORD color_temperature ;
  DWORD lux_index ;
  int i ;
  if( argc < 2 ) return 0 ;
  for( i = 0 ; i < argc ; i ++ ) {
    if( i == 0 ) {
      write_header( ) ;
      continue ;
    }
    dump_exif_info( 
      argv[i] , 
      &color_temperature , 
      &lux_index 
      ) ;
    /* printf( "[+] %ls : %d %d\n" , argv[i] , color_temperature , lux_index ) ; */
    write_to_file(
      argv[i] ,
      color_temperature ,
      lux_index ,
      OUTPUT_FILE_NAME
      ) ;
  }
  getch( ) ;
  return 0;
}
