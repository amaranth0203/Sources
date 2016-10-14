#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define SIZE 1024*1024
int main( int argc , char** argv ) {
  
  FILE* f ;
  unsigned char info[SIZE] ;
  int i , size ;
  assert( argc == 2 ) ;
  assert( f = fopen( argv[1] , "rb" ) ) ;
  assert( fseek( f , 0 , SEEK_END ) == 0 ) ;
  assert( ( size = ftell( f ) ) >= 0 ) ;
  size = size > SIZE ? SIZE : size ;
  assert( fseek( f , 0 , SEEK_SET ) == 0 ) ;
  assert( fread( info , sizeof( unsigned char) , size , f ) == size ) ;
  for( i = 0 ; i < size ; i ++ ) {
    if( info[ i ] == 'A' )
      if( info[ i + 1 ] == 'L' )
        if( info[ i + 2] == 'C' )
          if( info[ i + 3 ] == 'S' ) {
            //system( "cmd /c start cmd /c \"color 0a & echo ********** & echo exif exists & echo ********** & pause>nul\"" ) ;//cygwin
            system( "cmd /c \"color 0a & echo ********** & echo exif exists & echo ********** & pause>nul\"" ) ;//cmd
            return 0 ;
          }
  }
  assert( fclose( f ) == 0 ) ;
  //system( "cmd /c start cmd /c \"color 0C & echo ********** & echo exif not exists & echo ********** & pause>nul\"" ) ;//cygwin
  system( "cmd /c \"color 0C & echo ********** & echo exif not exists & echo ********** & pause>nul\"" ) ;//cmd
  return -1 ;
}
