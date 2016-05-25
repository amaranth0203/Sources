#include <stdio.h>
int main( )
{
    int a ;
    a = 3 * 5 , a * 4 ;
    printf( "[+] %d \n" , a ) ;
    a = ( 1 , a * 4 ) ;
    printf( "[+] %d \n" , a ) ;
    return 0 ;
}
