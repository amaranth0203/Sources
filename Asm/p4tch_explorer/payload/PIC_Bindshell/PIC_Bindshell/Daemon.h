#ifndef _Daemon_H_
#define _Daemon_H_

#include <Windows.h>
#include "WinApiDeclare.h"

	/*
	*/
DWORD Daemon( LPVOID lpThreadParameter ) {
	funcs f ;
	char b1[] = { 'b' , '1' , 0 } ;
	InitialFuncs( &f ) ;	
	f.MessageBox( NULL , b1 , NULL , 0 ) ;
	f.MessageBox( NULL , b1 , NULL , 0 ) ;
	f.MessageBox( NULL , b1 , NULL , 0 ) ;
	f.MessageBox( NULL , b1 , NULL , 0 ) ;
	f.MessageBox( NULL , b1 , NULL , 0 ) ;
	f.Sleep( 1000 ) ;
	return 0 ;
}

#endif // end of _Daemon_H_