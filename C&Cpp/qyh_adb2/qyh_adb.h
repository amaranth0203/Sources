#ifndef __qyh_adb_h__
#define __qyh_adb_h__
#include <windows.h>
LRESULT CALLBACK WndProc( HWND hwnd ,
                          UINT msg ,
                          WPARAM wParam ,
                          LPARAM lParam ) ;
BOOL CALLBACK AboutDlgProc( HWND hwnd ,
                            UINT Message ,
                            WPARAM wParam ,
                            LPARAM lParam ) ;
#endif
