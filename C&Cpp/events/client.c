#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
int main( int argc , char** argv )
{
    printf( "[+] send to pid %d signal %d \n" , atoi( argv[1] ) , atoi( argv[2] ) ) ;
    kill( atoi( argv[1] ) , atoi( argv[2] ) ) ;
    return 0 ;
}
