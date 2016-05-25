#include <stdio.h>
#include <stdlib.h>
int main( )
{
    char c ;
    system( "stty -echo" ) ;
    c = getchar( ) ;
    system( "stty echo" ) ;
    printf( "*** %c ***\n" , c ) ;
    return 0 ;
}
