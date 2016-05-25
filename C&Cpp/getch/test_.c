//#include     <stdlib.h>
#include     <curses.h>
//#include     <sys/types.h> 
//#include     <sys/stat.h>  
//#include     <fcntl.h>
//#include     <errno.h>
int main( )
{
    char ch ;
    ch = getch( ) ;
        putchar( ch ) ;
        ch = getch( ) ;
    putchar( '\n' ) ;
    printf( "%d" , ch ) ;
    return 0 ;
} 
