#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1024*1024
int find_str_in_binary( unsigned char* binary , int binary_size , const char* target ) {
  int i , ii ;
  for( i = 0 ; i < binary_size ; i ++ ) {
    if( binary[i] == target[0] ) {
      for( ii = 1 ; ii < strlen( target ) ; ii ++ ) {
        if( i + ii >= binary_size ) return 0 ;
        if( binary[ i + ii ] == target[ ii ] ) continue ;
        else break ;
      }
      if( ii == strlen( target ) ) return 1 ;
    }
  }
  return 0 ;
}
void success( char* result ) {
  system( "cmd /c \"color 0a & echo ******************** & echo \ 3a mobicat exists & echo ********************\"" ) ;
  char* cmd ;
  cmd = ( char* )malloc( sizeof( char )*1024 ) ;
  cmd[0] = 1 ;
  cmd[1] = 0 ;
  strcat( cmd , "cmd /c \"color 0a & echo ******************** & echo \ " ) ;
  strcat( cmd , result ) ;
  strcat( cmd , " & echo ******************** & pause>nul\"" ) ;
  cmd = cmd + 1 ;
  system( cmd ) ;
  return 0 ;
}
void failed( ) {
  system( "cmd /c \"color 0c & echo ******************** & echo \ mobicat not exists & echo ******************** & pause>nul\"" ) ;
  exit( -1 ) ;
}
void special_result( char* result ) {
  system( "cmd /c \"color 0e & echo ******************** & echo \ 3a mobicat exists & echo ********************\"" ) ;
  char* cmd ;
  cmd = ( char* )malloc( sizeof( char )*1024 ) ;
  cmd[0] = 1 ;
  cmd[1] = 0 ;
  strcat( cmd , "cmd /c \"color 0e & echo ******************** & echo \ " ) ;
  strcat( cmd , result ) ;
  strcat( cmd , " & echo ******************** & pause>nul\"" ) ;
  cmd = cmd + 1 ;
  system( cmd ) ;
  return 0 ;
}
void output( int* flag ) {
  if( !flag[0] ) failed( ) ;
  if( !flag[1] && !flag[2] && !flag[3] ) failed( ) ;
  char* result ;
  result = ( char* )malloc( sizeof( char )*800 ) ;
  if( flag[1] )
    memcpy( result , "aec : yes" , 9 ) ;
  else
    memcpy( result , "aec : no " , 9 ) ;
  if( flag[2] )
    memcpy( result + 9 , " & echo \ af  : yes" , 19 ) ;
  else
    memcpy( result + 9 , " & echo \ af  : no " , 19 ) ;
  if( flag[3] )
    memcpy( result + 27 , " & echo \ awb : yes" , 19 ) ;
  else
    memcpy( result + 27 , " & echo \ awb : no " , 19 ) ;
  if( flag[0] && flag[1] && flag[2] && flag[3] ) success( result ) ;
  else special_result( result ) ;
}
int main(int argc, char *argv[])
{
  int flag[4] = {
    0 , // "Qualcomm Camera Debug"
    0 , // "QCAEC"
    0 , // "QC_AF"
    0   // "QCAWB"
  } ;
  FILE* f ;
  unsigned char* info ;
  info = ( unsigned char* )malloc( sizeof( unsigned char ) * SIZE ) ;
  int i , size ;
  assert( argc == 2 ) ;
  assert( f = fopen( argv[1] , "rb" ) ) ;
  assert( fseek( f , 0 , SEEK_END ) == 0 ) ;
  assert( ( size = ftell( f ) ) >= 0 ) ;
  size = size > SIZE ? SIZE : size ;
  assert( fseek( f , 0 , SEEK_SET ) == 0 ) ;
  assert( fread( info , sizeof( unsigned char ) , size , f ) == size ) ;

  flag[0] = find_str_in_binary( info , size , "Qualcomm Camera Debug" ) ;
  flag[1] = find_str_in_binary( info , size , "QCAEC" ) ;
  flag[2] = find_str_in_binary( info , size , "QC_AF" ) ;
  flag[3] = find_str_in_binary( info , size , "QCAWB" ) ;

  output( flag ) ;

  return 0 ;
}
