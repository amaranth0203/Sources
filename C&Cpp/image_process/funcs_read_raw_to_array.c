#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"

void read_raw_to_array( const char* filename , double* raw_data ) {
    FILE* f = fopen( filename , "rwb+" ) ;
    unsigned char* buf ;
    buf = ( unsigned char* )malloc( 2 * WIDTH * HEIGHT * sizeof( unsigned char ) ) ;
    int i ;
    fread( buf , sizeof( unsigned char ) , WIDTH * HEIGHT * 2 , f ) ;
    for( i = 0 ; i < WIDTH * HEIGHT ; i ++ ) {
        *( raw_data + i ) = ( ( *(buf+i*2) ) | ( *(buf+i*2+1) << 8 ) ) / 4.0f ;
    }
    fclose( f ) ;
    return ;
}