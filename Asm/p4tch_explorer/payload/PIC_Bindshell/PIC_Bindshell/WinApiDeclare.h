#ifndef _WinApiDeclare_H_
#define _WinApiDeclare_H_

#include <Windows.h>
#include <winsock2.h>

// Redefine Win32 function signatures. This is necessary because the output
// of GetProcAddressWithHash is cast as a function pointer. Also, this makes
// working with these functions a joy in Visual Studio with Intellisense.
typedef HMODULE (WINAPI *FuncLoadLibraryA) (
	_In_z_	LPTSTR lpFileName
);

typedef int (WINAPI *FuncWsaStartup) (
	_In_	WORD wVersionRequested,
	_Out_	LPWSADATA lpWSAData
);

typedef SOCKET (WINAPI *FuncWsaSocketA) (
	_In_		int af,
	_In_		int type,
	_In_		int protocol,
	_In_opt_	LPWSAPROTOCOL_INFO lpProtocolInfo,
	_In_		GROUP g,
	_In_		DWORD dwFlags
);

typedef int (WINAPI *FuncBind) (
	_In_	SOCKET s,
	_In_	const struct sockaddr *name,
	_In_	int namelen
);

typedef int (WINAPI *FuncListen) (
	_In_	SOCKET s,
	_In_	int backlog
);

typedef SOCKET (WINAPI *FuncAccept) (
	_In_		SOCKET s,
	_Out_opt_	struct sockaddr *addr,
	_Inout_opt_	int *addrlen
);

typedef int (WINAPI *FuncCloseSocket) (
	_In_	SOCKET s
);

typedef BOOL (WINAPI *FuncCreateProcess) (
	_In_opt_	LPCTSTR lpApplicationName,
	_Inout_opt_	LPTSTR lpCommandLine,
	_In_opt_	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_		BOOL bInheritHandles,
	_In_		DWORD dwCreationFlags,
	_In_opt_	LPVOID lpEnvironment,
	_In_opt_	LPCTSTR lpCurrentDirectory,
	_In_		LPSTARTUPINFO lpStartupInfo,
	_Out_		LPPROCESS_INFORMATION lpProcessInformation
);

typedef DWORD (WINAPI *FuncWaitForSingleObject) (
	_In_	HANDLE hHandle,
	_In_	DWORD dwMilliseconds
);

typedef DWORD (WINAPI *FuncMessageBox) (
  _In_opt_ HWND    hWnd,
  _In_opt_ LPCTSTR lpText,
  _In_opt_ LPCTSTR lpCaption,
  _In_     UINT    uType
);

typedef VOID (WINAPI *FuncSleep)(
  _In_ DWORD dwMilliseconds
);

typedef HANDLE (WINAPI *FuncCreateThread)(
  _In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
  _In_      SIZE_T                 dwStackSize,
  _In_      LPTHREAD_START_ROUTINE lpStartAddress,
  _In_opt_  LPVOID                 lpParameter,
  _In_      DWORD                  dwCreationFlags,
  _Out_opt_ LPDWORD                lpThreadId
);

typedef DWORD (WINAPI *FuncWaitForMultipleObjects)(
  _In_       DWORD  nCount,
  _In_ const HANDLE *lpHandles,
  _In_       BOOL   bWaitAll,
  _In_       DWORD  dwMilliseconds
);

typedef BOOL (WINAPI *FuncCloseHandle)(
  _In_ HANDLE hObject
);

typedef struct _funcs {
	FuncLoadLibraryA LoadLibraryA;
	FuncMessageBox MessageBox;
	FuncSleep Sleep ;
	FuncCreateThread CreateThread ;
	FuncWaitForMultipleObjects WaitForMultipleObjects ;
	FuncCloseHandle CloseHandle ;
} funcs , *pfuncs ;

VOID InitialFuncs( pfuncs pf ) {
	//char module2[] = { 'u', 's', 'e', 'r', '3', '2', '.', 'd', 'l', 'l', 0 } ;
	#pragma warning( push )
	pf->LoadLibraryA				=	( FuncLoadLibraryA )GetProcAddressWithHash( 0x0726774C );
	//pf->LoadLibraryA((LPTSTR) module2);
	pf->MessageBox					=	( FuncMessageBox )GetProcAddressWithHash( 0x07568345 ) ;
	pf->Sleep						=	( FuncSleep )GetProcAddressWithHash( 0xE035F044 ) ;
	pf->CreateThread				=	( FuncCreateThread )GetProcAddressWithHash( 0x160D6838 ) ;
	pf->WaitForMultipleObjects		=	( FuncWaitForMultipleObjects)GetProcAddressWithHash( 0xDEB6FDC4 ) ;
	pf->CloseHandle					=	( FuncCloseHandle )GetProcAddressWithHash( 0x528796C6 ) ;
	#pragma warning( disable : 4055 ) // Ignore cast warnings
	#pragma warning( pop )
}
#endif // end of _WinApiDeclare_H_