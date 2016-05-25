#include <stdio.h>
#include <stdlib.h>
void modify( int** ptr , int tmp )
{
    printf( "^^^ %d ^^^\n" , **ptr ) ;
    **ptr = tmp ;
}
int main( )
{
    int tmp = 5 ;
    int* a = &tmp ;
    modify( &a , 44 ) ;
    printf( "tmp : %d\n" , tmp ) ;
    printf( "*a  : %d\n" , *a ) ;
    return 0 ;
}
