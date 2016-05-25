#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
void new_op( int , siginfo_t* , void* ) ;
int main( int argc , char** argv )
{
    struct sigaction act ;
    int sig ;
    sig = atoi( argv[1] ) ;
    sigemptyset( &act.sa_mask ) ;
    act.sa_flags = SA_SIGINFO ;
    act.sa_sigaction = new_op ;
    sigaction( sig , &act , NULL ) ;
    while( 1 )
    {
        printf( "hello world\n" ) ;
        sleep( 2 ) ;
    }
}
void callback( int a )
{
    printf( "[+] I'm callback , receive signal %d\n" , a ) ;
    sleep( 5 ) ;
    printf( "[+] end of my sleep of 5" ) ;
}
void test( void (*beCall)( int ) , int a  )
{
    printf( "[+] I'm test, calling call back function with signal %d \n" , a ) ;
    beCall( a ) ;
}
void new_op( int signum , siginfo_t *info , void *myact )
{
    test( callback , signum ) ;
}
