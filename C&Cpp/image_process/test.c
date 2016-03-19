#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"
int main( ) {
    int i ;
    double* raw_data ;
    raw_data = ( double* )malloc( WIDTH * HEIGHT * sizeof( double ) ) ;
    read_raw_to_array( FILENAME , raw_data ) ;
    /*
     *for( i = 0 ; i < WIDTH * HEIGHT ; i ++ ) {
     *    printf( "%g " , *( raw_data + i ) ) ;
     *}
     */
        
    return 0 ;
}