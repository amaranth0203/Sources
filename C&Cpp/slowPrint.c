#include <stdio.h>
#include <unistd.h>
void slowPrint( char* str , int time )
{
    char* ptr = str ;
    while( *ptr )
    {
        putchar( *ptr ++ ) ;
        fflush( stdout ) ;
        usleep( time ) ;
    }
    putchar( '\n' ) ;
    return ;
}
int main( )
{
    char *str = "Every one is a moon and has a dark side which never shows to others." ;
    slowPrint( str , 1000 ) ;
    slowPrint( str , 1000 ) ;
    slowPrint( str , 1000 ) ;
    slowPrint( str , 1000 ) ;
    return 0 ;
}
