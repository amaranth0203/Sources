#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "funcs.h"

int main( ) {
  unsigned char* in_raw =
    ""
    ;
  unsigned char* in_padded ;
  uint32_t M[16] , a0 , b0 , c0 , d0 ;
  uint32_t A , B , C , D , F , dTemp ;
  int block_count ;

  a0 = 0x67452301 ;
  b0 = 0xefcdab89 ;
  c0 = 0x98badcfe ;
  d0 = 0x10325476 ;
  
  block_count = pad_input( in_raw , &in_padded ) ;

  int i , ii , g ;
  for( ii = 0 ; ii < block_count ; ii ++ ) {
    break_512_chunk_into_M( ii*64 + in_padded , M ) ;
    A = a0 ;
    B = b0 ;
    C = c0 ;
    D = d0 ;
    for( i = 0 ; i < 64 ; i ++ ) {
      if( 0 <= i && i <= 15 ) {
        F = ( B & C ) | ( ( ~B ) & D ) ;
        g = i ;
      }
      else if( 16 <= i && i <= 31 ) {
        F = ( D & B ) | ( ( ~D ) & C ) ;
        g = ( 5 * i + 1 ) % 16 ;
      }
      else if( 32 <= i && i <= 47 ) {
        F = B ^ C ^ D ;
        g = ( 3 * i + 5 ) % 16 ;
      }
      else if( 48 <= i && i <= 63 ) {
        F = C ^ ( B | ( ~D ) ) ;
        g = ( 7 * i ) % 16 ;
      }
      dTemp = D ;
      D = C ;
      C = B ;
      B = B + leftrotate( ( A + F + K[i] + M[g] ) , s[i] ) ;
      A = dTemp ;

    }
    a0 = a0 + A ;
    b0 = b0 + B ;
    c0 = c0 + C ;
    d0 = d0 + D ;
  }

  print_512_bit_chunk( in_padded ) ;
  printf( " 0 1 2 3 4 5 6 7 8 9 a b c d e f\n" ) ;

  print_result( a0 , b0 , c0 , d0 ) ;

  return 0 ;
  
}
