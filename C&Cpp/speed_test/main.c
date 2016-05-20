#include <stdio.h>
#include <time.h>

int main( ) {

    struct timeb start , end ;
    double elapsed ;
    int a = 1 , i ;

    ftime( &start ) ;
    for( i = 0 ; i < 1000000000 ; i ++ ) {
        // a = ( a * 8 ) >> 4 ;
        // a = ( a * 8 ) / 16 ;

        // a = a * 32 ;
        a = a << 5 ;
    }
    ftime( &end ) ;

    elapsed = ( end.time - start.time ) + ( end.millitm - start.millitm ) / 1000.0 ;

    printf( "[+] %g\n" , elapsed ) ;

    return 0 ;
}