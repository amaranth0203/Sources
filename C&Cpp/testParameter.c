#include <stdio.h>
#include <string.h>
void func( char str[100] )
{
    puts( str ) ;
    printf( "[+] inner : %d , %d \n" , sizeof( str ) , strlen( str ) ) ;
    return ;
}
int main( )
{
    char str[5] = "aaa" ;
    printf( "[+] outer : %d , %d \n" , sizeof( str ) , strlen( str ) ) ;
    func( str ) ;
    return 0 ;
}
