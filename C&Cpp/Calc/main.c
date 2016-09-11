#include "funcs.h"
#include <stdio.h>
int main( ) {
    char* str = " 12 + 85 - 34 + 11 + 222 + 234 - 500 " ;
    printf( "[+] wassup %g\n" , evaluateExpression( str ) ) ;
    return 0 ;
}
