#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "funcs.h"

int main(int argc, char *argv[])
{
  unsigned char in[16] = {
    0x19 , 0xa0 , 0x9a , 0xe9 ,
    0x3d , 0xf4 , 0xc6 , 0xf8 ,
    0xe3 , 0xe2 , 0x8d , 0x48 ,
    0xbe , 0x2b , 0x2a , 0x08
  } ;
  unsigned char keys[16] ;

  sub_bytes( in ) ;
  shift_rows( in ) ;
  mix_columns( in ) ;
  generate_keys( "wassup" ) ;

  return 0;
}

