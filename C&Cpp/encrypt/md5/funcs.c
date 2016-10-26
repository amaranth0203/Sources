#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

  *( int* )&( *in_padded )[(block_count-1)*64+56] = strlen( in_raw ) << 3 ;

  return block_count ;

}

void break_512_chunk_into_M( unsigned char* chunk , uint32_t* M ) {
  int i ;
  for( i = 0 ; i < 16 ; i ++ ) {
    M[i] = chunk[0+4*i] | chunk[1+4*i] << 8 | chunk[2+4*i] << 16 | chunk[3+4*i] <<24 ;
  }
}

uint32_t leftrotate( uint32_t x , uint32_t c ) {
  return ( x << c ) | ( x >> ( 32 - c ) ) ;
}

void print_result( uint32_t a0 , uint32_t b0 , uint32_t c0 , uint32_t d0 ) {
  printf( "%08x%08x%08x%08x" ,
          ( a0 & 0xff000000 ) >> 24 | ( a0 & 0x00ff0000 ) >> 8 | ( a0 & 0x0000ff00 ) << 8 | ( a0 & 0x000000ff ) << 24 ,
          ( b0 & 0xff000000 ) >> 24 | ( b0 & 0x00ff0000 ) >> 8 | ( b0 & 0x0000ff00 ) << 8 | ( b0 & 0x000000ff ) << 24 ,
          ( c0 & 0xff000000 ) >> 24 | ( c0 & 0x00ff0000 ) >> 8 | ( c0 & 0x0000ff00 ) << 8 | ( c0 & 0x000000ff ) << 24 ,
          ( d0 & 0xff000000 ) >> 24 | ( d0 & 0x00ff0000 ) >> 8 | ( d0 & 0x0000ff00 ) << 8 | ( d0 & 0x000000ff ) << 24 ) ;
}
