#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"
#include "drafts.h"
int main( ) {
    int i , len = 0 ;
    int field[ 9*9 ] = { 0 } ;
    for( i = 0 , len = 0 ; i < 9*9 ; i ++ ) {
        if( !draft[i] ) {
            field[len++] = i ;
        }
    }
    re_calc( draft , field , 0 , len ) ;
    return 0 ;
}










