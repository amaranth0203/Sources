#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main( )
{
    char *dev ;
    char *net ;
    char *mask ;
    int ret ;
    char errbuf[ PCAP_ERRBUF_SIZE ] ;
    bpf_u_int32 netp ;
    bpf_u_int32 maskp ;
    struct in_addr addr ;
    pcap_if_t *alldevsp ;

    ret = pcap_findalldevs( &alldevsp , errbuf ) ;
    printf( "*** %d ***\n" , ret ) ;
    
    int i = 0 ;
    pcap_if_t *pdev ;
//    for( pdev = alldevsp ; pdev ; pdev = pdev->next )
    for( pdev = alldevsp ; pdev ; pdev = (*pdev).next )
    {
        printf( "%d : %s ||| %s\n" , ++i , pdev->name , pdev->description?pdev->description:"" ) ;
    }
    printf( "***   ***\n" ) ;


    dev = pcap_lookupdev( errbuf ) ;
    if( dev == NULL )
    {
        printf( "%s\n" , errbuf ) ;
        exit( 1 ) ;
    }
    printf( "DEV: %s\n" , dev ) ;
    ret = pcap_lookupnet( dev , &netp , &maskp , errbuf ) ;
    if( ret == -1 ) 
    {
        printf( "%s\n" , errbuf ) ;
        exit( 1 ) ;
    }
    addr.s_addr = netp ;
    net = inet_ntoa( addr ) ;
    if( net == NULL ) 
    {
        perror( "inet_ntoa" ) ;
        exit( 1 ) ;
    }
    printf( "NET: %s\n" , net ) ;
    addr.s_addr = maskp ;
    mask = inet_ntoa( addr ) ;
    if( mask == NULL )
    {
        perror( "inet_ntoa" ) ;
        exit( 1 ) ;
    }
    printf( "MASK: %s\n" , mask ) ;

    return 0 ;
}


