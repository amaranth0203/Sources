#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "funcs.h"

void test( ) {
  printf( "[+] wassup test\n" ) ;
#ifdef _DEBUG_
  printf( "[+] wassup test -- debug\n" ) ;
#endif
}
