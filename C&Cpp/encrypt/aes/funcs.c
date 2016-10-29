#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>
#include "funcs.h"

void test( ) {
  printf( "[+] wassup test\n" ) ;
#ifdef _DEBUG_
  printf( "[+] wassup test -- debug\n" ) ;
#endif
}

void print_4x4_matrix( unsigned char* state ) {
  int i , j ;
  puts( "..........." ) ;
  for( i = 0 ; i < 4 ; i ++ ) {
    for( j = 0 ; j < 4 ; j ++ ) {
      printf( "%02x " , state[4*i+j] ) ;
    }
    puts( "" ) ;
  }
  puts( "'''''''''''" ) ;
}

void print_16_bytes_a_line( unsigned char* state ) {
  int i ;
  for( i = 0 ; i < 16 ; i ++ ) {
    printf( "%02x " , state[i] ) ;
  }
  puts( "" ) ;
}

void sub_bytes( unsigned char* state ) {
  int i ;
  for( i = 0 ; i < 16 ; i ++ ) {
    state[i] = s_box[state[i]] ;
  }
}

void shift_rows( unsigned char* state ) {
  unsigned char ch ;
  /* row 2 shift 1 offset */
  ch = state[4] ;
  state[4] = state[5] ;
  state[5] = state[6] ;
  state[6] = state[7] ;
  state[7] = ch ;
  /* row 3 shift 2 offset */
  ch = state[8] ;
  state[8] = state[10] ;
  state[10] = ch ;
  ch = state[9] ;
  state[9] = state[11] ;
  state[11] = ch ;
  /* row 4 shift 3 offset */
  ch = state[15] ;
  state[15] = state[14] ;
  state[14] = state[13] ;
  state[13] = state[12] ;
  state[12] = ch ;
}

void mix_columns( unsigned char* state ) {
  unsigned char r[4] ;
  unsigned char a[4] ;
  unsigned char b[4] ;
  unsigned char c , i ;
  unsigned h ;
  for( i = 0 ; i < 4 ; i ++ ) {
    for( c= 0 ; c < 4 ; c ++ ) {
      r[c] = state[i+4*c] ;
    }
    
    for( c = 0 ; c < 4 ; c ++ ) {
      a[c] = r[c] ;
      h = ( unsigned char )( ( signed char )r[c] >> 7 ) ;
      b[c] = r[c] << 1 ;
      b[c] ^= 0x1b & h ;
    }
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1] ; /* 2*a0 + a3 + a2 + 3*a1 */
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2] ; /* 2*a1 + a0 + a3 + 3*a2 */
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3] ; /* 2*a2 + a1 + a0 + 3*a3 */
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0] ; /* 2*a3 + a2 + a1 + 3*a0 */

    for( c = 0 ; c < 4 ; c ++ ) {
      state[i+4*c] = r[c] ;
    }
  }
}

void generate_keys( unsigned char* passphrase ) {
  int i , ii ;
  char* buf ;
  HANDLE handle ;
  char* ( *get_sha1sum )( unsigned char* ) ;
  
  handle = LoadLibrary( "sha1sum.dll" ) ;
  get_sha1sum = ( void* )GetProcAddress( handle , "get_sha1sum" ) ;
  keys = ( unsigned char* )malloc( 16*11*sizeof( unsigned char ) ) ;
  memset( keys , 0 , 16*11*sizeof( unsigned char ) ) ;
  buf = get_sha1sum( "" ) ;
  for( i = 0 ; i < 16 ; i ++ ) {
    sscanf( buf , "%2hhx" , &keys[i] ) ;
    buf += 2 ;
  }

#ifdef _DEBUG_
  memcpy( keys ,
          "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F" ,
          /* "\x54\x68\x61\x74\x73\x20\x6D\x79\x20\x4B\x75\x6E\x67\x20\x46\x75" , */
          /* "\x2b\x28\xab\x09\x7e\xae\xf7\xcf\x15\xd2\x15\x4f\x16\xa6\x88\x3c" , */
          16
          );
#endif

  /* generate keys for 11 rounds */
  for( ii = 1 ; ii < 11 ; ii ++ ) {
    for( i = 0 ; i < 4 ; i ++ ) {
      if( i == 0 ) {
        keys[ ii*16 + 0*4 + i ] = keys[(ii-1)*16 + 1*4 + 3 ] ;
        keys[ ii*16 + 1*4 + i ] = keys[(ii-1)*16 + 2*4 + 3 ] ;
        keys[ ii*16 + 2*4 + i ] = keys[(ii-1)*16 + 3*4 + 3 ] ;
        keys[ ii*16 + 3*4 + i ] = keys[(ii-1)*16 + 0*4 + 3 ] ;

        keys[ ii*16 + 0*4 + i ] = s_box[ keys[ ii*16 + 0*4 + i ] ] ;
        keys[ ii*16 + 1*4 + i ] = s_box[ keys[ ii*16 + 1*4 + i ] ] ;
        keys[ ii*16 + 2*4 + i ] = s_box[ keys[ ii*16 + 2*4 + i ] ] ;
        keys[ ii*16 + 3*4 + i ] = s_box[ keys[ ii*16 + 3*4 + i ] ] ;

        keys[ ii*16 + 0*4 + i ] ^= rcon[ ii ] ;
      }
      else{
        keys[ ii*16 + 0*4 + i ] = keys[(ii)*16 + 0*4 + i-1 ] ;
        keys[ ii*16 + 1*4 + i ] = keys[(ii)*16 + 1*4 + i-1 ] ;
        keys[ ii*16 + 2*4 + i ] = keys[(ii)*16 + 2*4 + i-1 ] ;
        keys[ ii*16 + 3*4 + i ] = keys[(ii)*16 + 3*4 + i-1 ] ;
      }
      keys[ ii*16 + 0*4 + i ] ^= keys[ (ii-1)*16 + 0*4 + i ] ;
      keys[ ii*16 + 1*4 + i ] ^= keys[ (ii-1)*16 + 1*4 + i ] ;
      keys[ ii*16 + 2*4 + i ] ^= keys[ (ii-1)*16 + 2*4 + i ] ;
      keys[ ii*16 + 3*4 + i ] ^= keys[ (ii-1)*16 + 3*4 + i ] ;
    }
  }
#ifdef _DEBUG_
  for( ii = 0 ; ii < 11 ; ii ++ ) {
    print_16_bytes_a_line( keys + ii * 16 ) ;
  }
#endif
}

void add_round_key( unsigned char* in , int round ) {
  unsigned char ch ;
  for( ch = 0 ; ch < 16 ; ch ++ ) {
    in[ch] ^= keys[ch+round*16] ;
  }
}
