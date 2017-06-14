#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <string>
#include "snap7.h"
#include "funcs.h"
using namespace std ;

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN (65536)
#define DEFAULT_PORT "20176"

#ifdef OS_WINDOWS
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

TS7Client *Client;

byte Buffer[65536]; // 64 K buffer
int SampleDBNum = 1000;

char *Address;     // PLC IP Address
int Rack=0,Slot=2; // Default Rack and Slot

int ok = 0; // Number of test pass
int ko = 0; // Number of test failure

bool JobDone=false;
int JobResult=0;
//------------------------------------------------------------------------------
//  Async completion callback 
//------------------------------------------------------------------------------
// This is a simply text demo, we use callback only to set an internal flag...
void S7API CliCompletion(void *usrPtr, int opCode, int opResult)
{
    JobResult=opResult;
    JobDone = true;
}
//------------------------------------------------------------------------------
// Check error
//------------------------------------------------------------------------------
bool Check(int Result, const char * function)
{
    printf("\n");
    printf("+-----------------------------------------------------\n");
    printf("| %s\n",function);
    printf("+-----------------------------------------------------\n");
    if (Result==0) {
        printf("| Result         : OK\n");
        printf("| Execution time : %d ms\n",Client->ExecTime());
        printf("+-----------------------------------------------------\n");
        ok++;
    }
    else {
        printf("| ERROR !!! \n");
        if (Result<0)
            printf("| Library Error (-1)\n");
        else
            printf("| %s\n",CliErrorText(Result).c_str());
        printf("+-----------------------------------------------------\n");
        ko++;
    }
    return Result==0;
}
//------------------------------------------------------------------------------
// Unit Connection
//------------------------------------------------------------------------------
bool CliConnect()
{
    int res = Client->ConnectTo(Address,Rack,Slot);
    if (Check(res,"UNIT Connection")) {
          printf("  Connected to   : %s (Rack=%d, Slot=%d)\n",Address,Rack,Slot);
          printf("  PDU Requested  : %d bytes\n",Client->PDURequested());
          printf("  PDU Negotiated : %d bytes\n",Client->PDULength());
    };
    return res==0;
}

int __cdecl main(void) 
{
  // const char* input = "get:127.0.0.1,DB21.dbx0.0,DB21.dbx0.1,DB21.dbx0.2,DB21.dbb0,DB21.dbw0,DB21.dbd0" ;
  // const char* input = "set:127.0.0.1,db21.dbx0.0:1,db21.dbx0.1:1,db21.dbx0.2:1" ;
  // const char* input = "set:127.0.0.1,db21.dbb0:4" ;
  // const char* input = "set:127.0.0.1,db21.dbw0:44" ;
  // const char* input = "set:127.0.0.1,db21.dbd0:4444" ;
  // Bundle bundle( "127.0.0.1" , 0 , 1 ) ;
  // bundle.process_input( input ) ;
  // printf( "%s\n" , bundle.get_result( ).c_str( ) ) ;
  // exit( 0 ) ;
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);
    
    // Client Creation
    Client= new TS7Client();
    Client->SetAsCallback(CliCompletion,NULL);

    // Receive until the peer shuts down the connection
    do {
      memset( recvbuf , 0 , DEFAULT_BUFLEN ) ;
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
#if 1
            Bundle bundle( 
              "127.0.0.1" , 0 , 1 // config.ini plc ip rank slot
              ) ;
              /* 
               * only accept three format data from php :
               * 1. get:ip,DB20.DBB30.0,DB20.DBB32.0 ...
               * 2. set:ip,DB20.DBB30:1,DB20.DBB32.2 ...
               * 3. exit
               *
               * return value :
               * 1. value:value:value ...
               * 2. "ok" in binary
               * 3. "exit" in binary
               */
            // "duo lu tong" logic here
            bundle.process_input( recvbuf ) ;
            // end of "duo lu tong" logic block
#endif
        // Echo the buffer back to the sender
#if 0            
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
#else
            iSendResult = send( 
              ClientSocket, 
              bundle.get_result( ).c_str( ), 
              strlen( bundle.get_result( ).c_str( ) ) , 
              0 
              );
#endif
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // Deletion
    delete Client;

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
