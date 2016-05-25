#include <stdio.h>
#include <termios.h>
#include <unistd.h>
int mygetch( ) 
{
    struct termios oldt , newt ;
    int ch ;
    tcgetattr( STDIN_FILENO , &oldt ) ;
    newt = oldt ;
    newt.c_lflag &= ~( ICANON | ECHO ) ;
    tcsetattr( STDIN_FILENO , TCSANOW , &newt ) ;
    ch = getchar( ) ;
    putchar( ch ) ;
    tcsetattr( STDIN_FILENO , TCSANOW , &oldt ) ;
    return ch ;
}
int main( )
{
    char ch ;
    char arr[30] ;
    int i = 0 ;
    ch = mygetch( ) ;
    while( ch != '#' )
    {
        arr[i++] = ch ;
        ch = mygetch( ) ;
    }
    arr[i] = '#' ;
    i = 0 ;
    while( arr[i] != '#' )
    {
        putchar( arr[i++] ) ;
    }
    putchar( '\n' ) ;
    return 0 ;
}
