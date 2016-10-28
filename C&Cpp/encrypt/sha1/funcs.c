#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "funcs.h"

void print_512_bit_chunk( unsigned char* chunk ) {
  int i ;
  for( i = 0 ; i < 4 ; i ++ ) {
    printf( "%08x%08x%08x%08x\n" ,
            chunk[i*16+0] << 24 | chunk[i*16+1] << 16 | chunk[i*16+2] << 8 | chunk[i*16+3] ,
            chunk[i*16+4] << 24 | chunk[i*16+5] << 16 | chunk[i*16+6] << 8 | chunk[i*16+7] ,
            chunk[i*16+8] << 24 | chunk[i*16+9] << 16 | chunk[i*16+10] << 8 | chunk[i*16+11] ,
            chunk[i*16+12] << 24 | chunk[i*16+13] << 16 | chunk[i*16+14] << 8 | chunk[i*16+15] ) ;
  }
}

void print_128_bit_chunk( unsigned char* chunk ) {
    printf( "%08x%08x%08x%08x\n" ,
            chunk[0] << 24 | chunk[1] << 16 | chunk[2] << 8 | chunk[3] ,
            chunk[4] << 24 | chunk[5] << 16 | chunk[6] << 8 | chunk[7] ,
            chunk[8] << 24 | chunk[9] << 16 | chunk[10] << 8 | chunk[11] ,
            chunk[12] << 24 | chunk[13] << 16 | chunk[14] << 8 | chunk[15] ) ;
}

int pad_input( unsigned char* in_raw , unsigned char** in_padded ) {
  int block_count , block_reset ;
  
  block_count = 1 + strlen( in_raw ) / 64 ; // 512-bit block
  block_reset = strlen( in_raw ) % 64 ;
  if( block_reset > 56 ) block_count ++ ;
  *in_padded = ( char* )malloc( block_count * 64 ) ;
  memset( *in_padded , 0 , block_count * 64 ) ;
  memcpy( *in_padded , in_raw , strlen( in_raw ) ) ;

  int i ;
  for( i = 0 ; i < block_count * 64 ; i ++ ) {
    if( *( int* )&( *in_padded )[i] == 0 ) break ;
  }
  *( int* )&( *in_padded )[i] = 128 ;

  *( int* )&( *in_padded )[(block_count-1)*64+60] =
    ( strlen( in_raw ) << 3 ) >> 24 ;
  *( int* )&( *in_padded )[(block_count-1)*64+61] =
    ( strlen( in_raw ) << 3 ) >> 16 ;
  *( int* )&( *in_padded )[(block_count-1)*64+62] =
    ( strlen( in_raw ) << 3 ) >> 8 ;
  *( int* )&( *in_padded )[(block_count-1)*64+63] =
    strlen( in_raw ) << 3 ;

  return block_count ;

}

void break_512_chunk_into_w( unsigned char* chunk , uint32_t* w ) {
  int i ;
  for( i = 0 ; i < 16 ; i ++ ) {
    w[i] = chunk[0+4*i] << 24 | chunk[1+4*i] << 16 | chunk[2+4*i] << 8 | chunk[3+4*i] ;
  }
}

uint32_t leftrotate( uint32_t x , uint32_t c ) {
  return ( x << c ) | ( x >> ( 32 - c ) ) ;
}

char* get_sha1sum( unsigned char* in_raw ) {
  static char result[41] ;
  unsigned char* in_padded ;
  int block_count ;
  uint32_t a , b , c , d , e ;
  uint32_t ml , k , f , temp ;
  uint32_t w[80] ;
  ml = strlen( in_raw ) ;
  
  block_count = pad_input( in_raw , &in_padded ) ;

  int i , ii , g ;
  for( ii = 0 ; ii < block_count ; ii ++ ) {
    break_512_chunk_into_w( ii*64 + in_padded , w ) ;

    for( i = 16 ; i < 80 ; i ++ ) {
      w[i] = leftrotate( w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16] , 1 ) ;
    }
    a = h0 ;
    b = h1 ;
    c = h2 ;
    d = h3 ;
    e = h4 ;
    for( i = 0 ; i < 80 ; i ++ ) {
      if( 0 <= i && i < 20 ) {
        f = ( b & c ) | ( ( ~b ) & d ) ;
        k = 0x5a827999 ;
      }
      else if( 20 <= i && i < 40 ) {
        f = b ^ c ^ d ;
        k = 0x6ed9eba1 ;
      }
      else if( 40 <= i && i < 60 ) {
        f = ( b & c ) | ( b & d ) | ( c & d ) ;
        k = 0x8f1bbcdc ;
      }
      else if( 60 <= i && i < 80 ) {
        f = b ^ c ^ d ;
        k = 0xca62c1d6 ;
      }
      temp = leftrotate( a , 5 ) + f + e + k + w[i] ;
      e = d ;
      d = c ;
      c = leftrotate( b , 30 ) ;
      b = a ;
      a = temp ;
    }
    h0 = h0 + a ;
    h1 = h1 + b ;
    h2 = h2 + c ;
    h3 = h3 + d ;
    h4 = h4 + e ;
  }

  memset( result , 0 , 41 ) ;
  snprintf( result , 40 , "%x%x%x%x%x" , h0 , h1 , h2 , h3 , h4 ) ;

  return result ;

}
