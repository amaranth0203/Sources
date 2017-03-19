#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "funcs.h"
#define passphrase "wassup"

int main(int argc, char *argv[])
{
  FILE* f_in ;
  FILE* f_out ;
  char ex_name[4] ;
  char* out_fname ;
  unsigned char flag ;
  unsigned char buf[16] ;
  int readed_size ;

  assert( argc == 2 ) ;
  assert( f_in = fopen( argv[1] , "rb" ) ) ;
  memcpy( ex_name , argv[1] + strlen( argv[1] ) - 3 , 3 ) ;
  if( strcmp( ex_name , "qyh" ) == 0 ) { //decrypt
    out_fname = ( char* )malloc( strlen( argv[1] ) * sizeof( char ) ) ;
    memset( out_fname , 0 , strlen( argv[1] ) * sizeof( char ) ) ;
    memcpy( out_fname , argv[1] , strlen( argv[1] ) ) ;
    out_fname[ strlen( argv[1] ) - 4 ] = 0 ;
    assert( f_out = fopen( out_fname , "wb" ) ) ;
    while( 1 ) {
      memset( buf , 0 , 16 ) ;
      readed_size = fread( buf , sizeof( unsigned char ) , 16 , f_in ) ;
      if( readed_size == 0 ) break ;
      aes_R( buf , passphrase ) ;
      fwrite( buf , sizeof( unsigned char ) , 16 , f_out ) ;      
    }
  }
  else { //encrypt
    out_fname = ( char* )malloc( ( strlen( argv[1] ) + 5 ) * sizeof( char ) ) ;
    memset( out_fname , 0 , ( strlen( argv[1] ) + 5 ) * sizeof( char ) ) ;
    memcpy( out_fname , argv[1] , strlen( argv[1] ) ) ;
    out_fname[ strlen( argv[1] ) + 0 ] = '.' ;
    out_fname[ strlen( argv[1] ) + 1 ] = 'q' ;
    out_fname[ strlen( argv[1] ) + 2 ] = 'y' ;
    out_fname[ strlen( argv[1] ) + 3 ] = 'h' ;
    assert( f_out = fopen( out_fname , "wb" ) ) ;
    while( 1 ) {
      memset( buf , 0 , 16 ) ;
      readed_size = fread( buf , sizeof( unsigned char ) , 16 , f_in ) ;
      if( readed_size == 0 ) break ;
      aes( buf , passphrase ) ;
      fwrite( buf , sizeof( unsigned char ) , 16 , f_out ) ;
    }
  }
  assert( fclose( f_in ) == 0 ) ;
  assert( fclose( f_out ) == 0 ) ;

  printf( "[+] done. Press any key to continue...\n" ) ;
  getchar( ) ;
#if 0
  unsigned char in[16] = {
    0x19 , 0xa0 , 0x9a , 0xe9 ,
    0x3d , 0xf4 , 0xc6 , 0xf8 ,
    0xe3 , 0xe2 , 0x8d , 0x48 ,
    0xbe , 0x2b , 0x2a , 0x08
  } ;
  int round ;
  unsigned char* out ;

  print_16_bytes_a_line( in ) ;
  aes( in , "wassup" ) ;
  print_16_bytes_a_line( in ) ;
  aes_R( in , "wassup" ) ;
  print_16_bytes_a_line( in ) ;
#endif
  return 0;
}

