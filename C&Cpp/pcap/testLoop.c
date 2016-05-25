#include <pcap/pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
char buffer[1024] ;
int index = 0 ;
void getPacket( u_char *arg , const struct pcap_pkthdr * pkthdr , const u_char * packet )
{
    int* id = (int *)arg ;
    int i , j ;
/*
    printf( "[+] id : %d\n" , ++(*id ) ) ;
    printf( "    packet len : %d\n" , pkthdr -> len ) ;
    printf( "    packet caplen : %d\n" , pkthdr -> caplen ) ;
    printf( "    recieved time : %s" , ctime( (const time_t *) &pkthdr->ts.tv_sec ) ) ;

    printf( "    bytes streams : \n" ) ;
    printf( "   " ) ;
    for( i = 0 ; i < pkthdr->len ; i ++ )
    {
        printf( " %02x" , packet[i] ) ;
        if( ( i + 1 ) % 16 == 0 ) 
        {
            printf( "  " ) ;
            for( j = i - 16 ; j <= i ; j ++ )
                if( packet[j] >= 32 && packet[j] <= 126 ) putchar( packet[j] ) ;
                else putchar( '.' ) ;
            printf ( "\n   " ) ; 
        }
        else if( ( i + 1 ) % 4 == 0 ) printf( " |" ) ;
    }
    if( ( i + 1 ) % 16 != 0 )
    {
        while( 1 )
        {
            printf( "   " ) ;
            if( ( i + 1 ) % 16 == 0 ) break ;
            if( ( i + 1 ) % 4 == 0 ) printf ( "  " ) ;
            i ++ ;
        }
        printf( "  " ) ;
        for( j = i - 16 ; j <= i && j < pkthdr->len ; j ++ )
            if( packet[j] >= 32 && packet[j] <= 126 ) putchar( packet[j] ) ;
            else putchar( '.' ) ;
    }
    puts( "" ) ;
*/
    // ethernet wrap
/*
    printf( "    Mac : %02x:%02x:%02x:%02x:%02x:%02x -> %02x:%02x:%02x:%02x:%02x:%02x\n"  , 
            packet[6] , packet[7] , packet[8] , packet[9] , packet[10] , packet[11] , 
            packet[0] , packet[1] , packet[2] , packet[3] , packet[4] , packet[5] ) ;
*/
    // include a ip header
    if( ( packet[12] & 8 && 1 ) && !( packet[13] | 0 || 0 ) )
    {
/*
        printf( "     IP :   %03d.%03d.%03d.%03d -> %03d.%03d.%03d.%03d \n" , 
                packet[26] , packet[27] , packet[28] , packet[29] , 
                packet[30] , packet[31] , packet[32] , packet[33] ) ;
*/
        // include a tcp header
        if( packet[23] & 6 && 1 )
        {
/*
            printf( "      TCP :           %05d -> %05d \n" , packet[34]*256 + packet[35] , packet[36]*256 + packet[37] ) ;
            puts( "      TCP FLAG : " ) ;
            printf( "      URG , ACK , PSH , RST , SYN , FIN\n       %-6d%-6d%-6d%-6d%-6d%-6d\n" , 
                    packet[47] & 32 && 1 , packet[47] & 16 && 1, packet[47] & 8 && 1 ,
                    packet[47] & 4 && 1, packet[47] & 2 && 1, packet[47] & 1 && 1 ) ;
*/
            // include something for telnet
            if( ( packet[34]==0 && packet[35]==23 ) || ( packet[36]==0 && packet[37]==23 ) )
            {
                printf( "       TELNET : " ) ;
                if( pkthdr->len > 54 ) 
                {
                    for( i = 54 ; i < pkthdr->len ; i ++ ) 
                    {
                        putchar( packet[i] ) ;
                    }
                    if( ( packet[47] & 8 && 1 ) && packet[54] < 128 )
                    {
                        for( i = 54 ; i < pkthdr->len ; i ++ )
                            buffer[index++] = packet[i] ;
                    }
                }
                if( packet[47] & 1 && 1 )
                {
                    puts( "TCP-FIN" ) ;
                    buffer[index++] = '\0' ;
                    printf( "[*] buffer : \n %s" , buffer ) ;
                }
                puts( "" ) ;
            }//end of telnet
        }//end of tcp
    }// end of ip
}
int main( )
{
    char errBuf[PCAP_ERRBUF_SIZE] ;
    char* devStr = "vboxnet0" ;
    pcap_t* dev = pcap_open_live( devStr , 65535 , 1 , 0 , errBuf ) ;
    if( !dev )
    {
        printf( "[-] pcap_open_live( ) : %s\n" , errBuf ) ;
        exit( 1 ) ;
    }

    struct bpf_program filter ;
//    pcap_compile( dev , &filter , "( dst port 23 or dst host 192.168.44.1) and ( tcp ) " , 1 , 0 ) ;
    pcap_compile( dev , &filter , "src port 23 or dst port 23" , 1 , 0 ) ;
//    pcap_compile( dev , &filter , "dst port 23" , 1 , 0 ) ;
//    pcap_compile( dev , &filter , "src port 23" , 1 , 0 ) ;
//    pcap_compile( dev , &filter , "dst port 23 and (tcp[tcpflags] & tcp-push != 0)" , 1 , 0 ) ;
    pcap_setfilter( dev , &filter ) ;

    int id = 0 ;
    pcap_loop( dev , -1 , getPacket , (u_char *)&id ) ;
    pcap_close( dev ) ;
    return 0 ;
}
