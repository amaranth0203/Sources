#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"
#include "drafts.h"
int main( ) {
    int i , len = 0 ;
    int field[ 9*9 ] = { 0 } ; // which offset needs to fill in
#ifdef _DEBUG_
    trans_input( quick_input , draft ) ;
#endif
    for( i = 0 , len = 0 ; i < 9*9 ; i ++ ) {
        if( draft[i] == 1 ) {
            field[len++] = i ;
        }
    }
    print( draft ) ;
    print( field ) ;
    re_calc( draft , field , 0 , len ) ; // from ( 1 , 1 ) to ( 7 , 7 )
    return 0 ;
}










