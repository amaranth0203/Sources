#include <stdio.h>
#include <curses.h>
int main( )
{
    int a = 0 ;
    initscr( ) ;
    a = getch( ) ;
    endwin( ) ;
    printf( "getch( ) = %d\n" , a ) ;
    return 0 ;

}
