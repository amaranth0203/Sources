#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "funcs.h"

int main(int argc, char *argv[])
{
  /* unsigned char in[16] = { */
  /*   0x19 , 0xa0 , 0x9a , 0xe9 , */
  /*   0x3d , 0xf4 , 0xc6 , 0xf8 , */
  /*   0xe3 , 0xe2 , 0x8d , 0x48 , */
  /*   0xbe , 0x2b , 0x2a , 0x08 */
  /* } ; */
  unsigned char in[16] = "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF" ;
  int round ;

  generate_keys( "wassup" ) ;
#ifdef _DEBUG_
  //  for( round = 0 ; round < 16 ; round ++ ) {
  //    print_16_bytes_a_line( keys ) ;
    //  }
#endif
  
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

  print_4x4_matrix( in ) ;

  return 0;
}

