#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main( )
{
    char errbuf[ PCAP_ERRBUF_SIZE ] = "" ;
    char* dev = "vboxnet0" ;
//    char* dev = "ppp0" ;
    char* mask ;
    char* net ;
    pcap_t* handle ;
    bpf_u_int32 netp , maskp ;
    struct in_addr addr ;
    int status ;

    printf( "[+] name  :  %s\n" , dev ) ;
    handle = pcap_open_live( dev , 65535 , 0 , 0 , errbuf ) ;
    if( errbuf == NULL ) { puts( "open failed" ) ; exit( 1 ) ; }
    status = pcap_lookupnet( dev , &netp , &maskp , errbuf ) ;
    if( status == -1 ) { puts( "lookupnet failed" ) ; exit( 1 ) ; }
    addr.s_addr = netp ;
    net = inet_ntoa( addr ) ;
    if( net == NULL ) { puts( "ntoa failed" ) ; exit( 1 ) ; }
    printf( "[+] net   :  %s\n" , net ) ;
    addr.s_addr = maskp ;
    mask = inet_ntoa( addr ) ;
    if( mask == NULL ) { puts( "ntoa failed" ) ; exit( 1 ) ; }
    printf( "[+] mask  :  %s\n" , mask ) ;
    return 0 ;
}
