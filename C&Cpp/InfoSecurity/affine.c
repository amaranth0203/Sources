#include <stdio.h>
#include <stdlib.h>
void test( char* text , int a , int b ) ;
void toUpperCase( char* lowerCase ) ;
int main( )
{
//    char buffer[512] = "hello" ;
//    char buffer[512] = "general" ;
    char buffer[512] = "FMXVEDK" ;
    char* text = buffer ;
    puts( text ) ;
//    test( text , 7 , 11 ) ;
//    test( text , 3 , 5 ) ;
    test( text , 9 , -19 ) ;
    puts( text ) ;
    return 0 ;
}
void test( char* text , int a , int b )
{
    char* ptr = text ;
    toUpperCase( text ) ;
    while( *ptr != '\0' )
    {
        *ptr = ( ( a * ( (int)(*ptr) - (int)'A' ) + b ) % 26 ) + 'A' ;
        ptr ++ ;
    }
}
void toUpperCase( char* lowerCase )
{
    char* ptr = lowerCase ;
    while( *ptr != '\0' )
    {
        if( *ptr >= 'a' && * ptr <= 'z' )
        {
            *ptr = *ptr - ( 'a' - 'A' ) ;
        }
        ptr ++ ;
    }
}

