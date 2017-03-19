#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "funcs.h"

#ifdef _WIN32
#include <windows.h>
#define DLL_NAME "sha1sum.dll"
#else
#include <dlfcn.h>
#define DLL_NAME "sha1sum.so"
#endif
#define FUNC_NAME "get_sha1sum"

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

void sub_bytes_R( unsigned char* state ) {
  int i ;
  for( i = 0 ; i < 16 ; i ++ ) {
    state[i] = s_box_R[state[i]] ;
  }
}

void shift_rows( unsigned char* state ) {
  unsigned char ch ;
  /* row 2 shift 1 offset */
  ch = state[1] ;
  state[1] = state[5] ;
  state[5] = state[9] ;
  state[9] = state[13] ;
  state[13] = ch ;
  /* row 3 shift 2 offset */
  ch = state[2] ;
  state[2] = state[10] ;
  state[10] = ch ;
  ch = state[6] ;
  state[6] = state[14] ;
  state[14] = ch ;
  /* row 4 shift 3 offset */
  ch = state[15] ;
  state[15] = state[11] ;
  state[11] = state[7] ;
  state[7] = state[3] ;
  state[3] = ch ;
}

void shift_rows_R( unsigned char* state ) {
  unsigned char ch ;
  /* row 2 shift 1 offset */
  ch = state[13] ;
  state[13] = state[9] ;
  state[9] = state[5] ;
  state[5] = state[1] ;
  state[1] = ch ;
  /* row 3 shift 2 offset */
  ch = state[2] ;
  state[2] = state[10] ;
  state[10] = ch ;
  ch = state[6] ;
  state[6] = state[14] ;
  state[14] = ch ;
  /* row 4 shift 3 offset */
  ch = state[3] ;
  state[3] = state[7] ;
  state[7] = state[11] ;
  state[11] = state[15] ;
  state[15] = ch ;
}

unsigned gmul( unsigned char a , unsigned char b ) {
  unsigned char p = 0 ;
  unsigned char counter ;
  unsigned char hi_bit_set ;
  for( counter = 0 ; counter < 8 ; counter ++ ) {
    if( ( b & 1 ) == 1 ) {
      p ^= a ;
    }
    hi_bit_set = ( a & 0x80 ) ;
    a <<= 1 ;
    if( hi_bit_set == 0x80 ) {
      a ^= 0x1b ;
    }
    b >>= 1 ;
  }
  return p ;
}

void mix_columns( unsigned char* state ) {
  unsigned char r[4] ;
  unsigned char a[4] ;
  unsigned char b[4] ;
  unsigned char c , i ;
  unsigned h ;
  for( i = 0 ; i < 4 ; i ++ ) {
    for( c= 0 ; c < 4 ; c ++ ) {
      r[c] = state[4*i+c] ;
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
      state[4*i+c] = r[c] ;
    }
  }
}

void mix_columns_R( unsigned char* state ) {
  unsigned char r[4] ;
  unsigned char a[4] ;
  unsigned char b[4] ;
  unsigned char c , i ;
  unsigned h ;
  for( i = 0 ; i < 4 ; i ++ ) {
    for( c= 0 ; c < 4 ; c ++ ) {
      r[c] = state[4*i+c] ;
    }
    
    for( c = 0 ; c < 4 ; c ++ ) {
      a[c] = r[c] ;
      h = ( unsigned char )( ( signed char )r[c] >> 7 ) ;
      b[c] = r[c] << 1 ;
      b[c] ^= 0x1b & h ;
    }

    r[0] = gmul( a[0] , 14 )^gmul( a[1] , 11 )^gmul( a[2] , 13 )^gmul( a[3] , 9 ) ;
    r[1] = gmul( a[1] , 14 )^gmul( a[2] , 11 )^gmul( a[3] , 13 )^gmul( a[0] , 9 ) ;
    r[2] = gmul( a[2] , 14 )^gmul( a[3] , 11 )^gmul( a[0] , 13 )^gmul( a[1] , 9 ) ;
    r[3] = gmul( a[3] , 14 )^gmul( a[0] , 11 )^gmul( a[1] , 13 )^gmul( a[2] , 9 ) ;

    for( c = 0 ; c < 4 ; c ++ ) {
      state[4*i+c] = r[c] ;
    }
  }
}

void generate_keys( unsigned char* passphrase ) {
  int i , ii ;
  char* buf ;
#ifdef _WIN32
  HANDLE handle ;
#else
  void* handle = NULL ;
#endif
  char* ( *get_sha1sum )( unsigned char* ) ;

#ifdef _WIN32  
  handle = LoadLibrary( "sha1sum.dll" ) ;
  get_sha1sum = ( void* )GetProcAddress( handle , "get_sha1sum" ) ;
#else
  handle = dlopen( "./"DLL_NAME , RTLD_NOW ) ;
  get_sha1sum = ( void* )dlsym( handle , FUNC_NAME ) ;
#endif
  if( NULL == get_sha1sum ) {
    printf( "handle , get_sha1sum : %p %p\n" , handle , get_sha1sum ) ;
    exit( -1 ) ;
  }
  keys = ( unsigned char* )malloc( 16*11*sizeof( unsigned char ) ) ;
  memset( keys , 0 , 16*11*sizeof( unsigned char ) ) ;
  buf = get_sha1sum( passphrase ) ;
  for( i = 0 ; i < 16 ; i ++ ) {
    sscanf( buf , "%2hhx" , &keys[i] ) ;
    buf += 2 ;
  }

  /* generate keys for 11 rounds */
  for( ii = 1 ; ii < 11 ; ii ++ ) {
    for( i = 0 ; i < 4 ; i ++ ) {
      if( i == 0 ) {
        keys[ ii*16 + i*4 + 0 ] = keys[(ii-1)*16 + 3*4 + 1 ] ;
        keys[ ii*16 + i*4 + 1 ] = keys[(ii-1)*16 + 3*4 + 2 ] ;
        keys[ ii*16 + i*4 + 2 ] = keys[(ii-1)*16 + 3*4 + 3 ] ;
        keys[ ii*16 + i*4 + 3 ] = keys[(ii-1)*16 + 3*4 + 0 ] ;

        keys[ ii*16 + i*4 + 0 ] = s_box[ keys[ ii*16 + i*4 + 0 ] ] ;
        keys[ ii*16 + i*4 + 1 ] = s_box[ keys[ ii*16 + i*4 + 1 ] ] ;
        keys[ ii*16 + i*4 + 2 ] = s_box[ keys[ ii*16 + i*4 + 2 ] ] ;
        keys[ ii*16 + i*4 + 3 ] = s_box[ keys[ ii*16 + i*4 + 3 ] ] ;
        
        keys[ ii*16 + i*4 + 0 ] ^= rcon[ ii ] ;
      }
      else{
        keys[ ii*16 + i*4 + 0 ] = keys[(ii)*16 + (i-1)*4 + 0 ] ;
        keys[ ii*16 + i*4 + 1 ] = keys[(ii)*16 + (i-1)*4 + 1 ] ;
        keys[ ii*16 + i*4 + 2 ] = keys[(ii)*16 + (i-1)*4 + 2 ] ;
        keys[ ii*16 + i*4 + 3 ] = keys[(ii)*16 + (i-1)*4 + 3 ] ;
      }
      keys[ ii*16 + i*4 + 0 ] ^= keys[ (ii-1)*16 + i*4 + 0 ] ;
      keys[ ii*16 + i*4 + 1 ] ^= keys[ (ii-1)*16 + i*4 + 1 ] ;
      keys[ ii*16 + i*4 + 2 ] ^= keys[ (ii-1)*16 + i*4 + 2 ] ;
      keys[ ii*16 + i*4 + 3 ] ^= keys[ (ii-1)*16 + i*4 + 3 ] ;
    }
  }
}

void add_round_key( unsigned char* in , int round ) {
  unsigned char ch ;
  for( ch = 0 ; ch < 16 ; ch ++ ) {
    in[ch] ^= keys[ch+round*16] ;
  }
}

void aes( unsigned char* in , unsigned char* passphrase ) {
  int round ;

  generate_keys( passphrase ) ;

  add_round_key( in , 0 ) ;
  for( round = 1 ; round < 10 ; round ++ ) {
    sub_bytes( in ) ;
    shift_rows( in ) ;
    mix_columns( in ) ;
    add_round_key( in , round ) ;
  }
  sub_bytes( in ) ;
  shift_rows( in ) ;
  add_round_key( in , 10 ) ;
}

void aes_R( unsigned char* in , unsigned char* passphrase ) {
  int round ;

  generate_keys( passphrase ) ;

  add_round_key( in , 10 ) ;
  shift_rows_R( in ) ;
  sub_bytes_R( in ) ;
  for( round = 9 ; round > 0 ; round -- ) {
    add_round_key( in ,round ) ;
    mix_columns_R( in ) ;
    shift_rows_R( in ) ;
    sub_bytes_R( in ) ;
  }
  add_round_key( in , 0 ) ;
}


