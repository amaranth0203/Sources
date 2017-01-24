#include "funcs.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
  MessageBox( NULL , L"dllCaller!wmain" , NULL , 0 ) ;
  return 0;
}

/* int wmain(int argc, wchar_t* argv[]) { */
/*   HINSTANCE hDll ; */
/*   MessageBox( NULL , L"dllCaller!wmain" , NULL , 0 ) ; */
/*   hDll = LoadLibrary( "XJJ.dll" ) ; */
/*   printf( "%p\n" , hDll ) ; */
/*   return 0 ; */
/* } */
