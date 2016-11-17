#include <windows.h>
#include <shlobj.h>
#include "resource.h"

#define BUF_SIZE (1024*128)

static HWND hwnd_main = NULL ;
static HWND g_hToolbar = NULL ;
static HWND hwnd_pro = NULL ;
static HWND hwnd_log = NULL ;
static HWND hwnd_dump = NULL ;
static HWND hwnd_cmd = NULL ;

const char g_szClassName[] = "qyh_adb" ;

void wassup( char* str ) {
  MessageBox( NULL , str , "wassup" , NULL ) ;
}

BOOL CALLBACK AboutDlgProc( HWND hwnd ,
                            UINT Message ,
                            WPARAM wParam ,
                            LPARAM lParam ) {
  switch( Message ) {
  case WM_INITDIALOG :
    return TRUE ;
  case WM_COMMAND :
    switch( LOWORD( wParam ) ) {
    case IDOK :
      EndDialog( hwnd , IDOK ) ;
      break ;
    case IDCANCEL :
      EndDialog( hwnd , IDCANCEL ) ;
      break ;
    } // end of switch( LOWORD( wParam ) )
    break ;
  default :
    return FALSE ;
  } // end of switch( Message )
  return TRUE ;
}


BOOL CALLBACK ToolDlgProc( HWND hwnd ,
                           UINT Message ,
                           WPARAM wParam ,
                           LPARAM lParam ) {
  static BOOL CHECKED_BK = FALSE ;
  static BOOL CHECKED_PL = FALSE ;
  static BOOL CHECKED_DL = FALSE ;
  static BOOL CHECKED_DS = FALSE ;
  static BOOL CHECKED_DJ = FALSE ;
  static BOOL CHECKED_DP = FALSE ;
  static BOOL CHECKED_DR = FALSE ;
  static BOOL CHECKED_DD = FALSE ;
  switch( Message ) {
  case WM_COMMAND :
    switch( LOWORD( wParam ) ) {
    case IDC_CHECK_BK : {
      switch( HIWORD( wParam ) ) {
      case BN_CLICKED : {
        if( SendDlgItemMessage( hwnd , IDC_CHECK_BK , BM_GETCHECK , 0 , 0 ) ) {
          CHECKED_BK = TRUE ;
        }
        else {
          CHECKED_BK = FALSE ;
        }
        break ;
      } // end of case BN_CLICKED
      } // end of switch( HIWORD( wParam ) )
      break ;
    } // end of case IDC_CHECK_BK
    case IDC_CHECK_PL : {
      switch( HIWORD( wParam ) ) {
      case BN_CLICKED : {
        if( SendDlgItemMessage( hwnd , IDC_CHECK_PL , BM_GETCHECK , 0 , 0 ) ) {
          CHECKED_PL = TRUE ;
        }
        else {
          CHECKED_PL = FALSE ;
        }
        break ;
      } // end of case BN_CLICKED
      } // end of switch( HIWORD( wParam ) )
      break ;
    } // end of case IDC_CHECK_PL
    case IDC_CHECK_DL : {
      switch( HIWORD( wParam ) ) {
      case BN_CLICKED : {
        if( SendDlgItemMessage( hwnd , IDC_CHECK_DL , BM_GETCHECK , 0 , 0 ) ) {
          CHECKED_DL = TRUE ;
        }
        else {
          CHECKED_DL = FALSE ;
        }
        break ;
      } // end of case BN_CLICKED
      } // end of switch( HIWORD( wParam ) )
      break ;
    } // end of case IDC_CHECK_DL
    case IDC_CHECK_DS : {
      switch( HIWORD( wParam ) ) {
      case BN_CLICKED : {
        if( SendDlgItemMessage( hwnd , IDC_CHECK_DS , BM_GETCHECK , 0 , 0 ) ) {
          CHECKED_DS = TRUE ;
        }
        else {
          CHECKED_DS = FALSE ;
        }
        break ;
      } // end of case BN_CLICKED
      } // end of switch( HIWORD( wParam ) )
      break ;
    } // end of case IDC_CHECK_DS
    case IDC_CHECK_DJ : {
      switch( HIWORD( wParam ) ) {
      case BN_CLICKED : {
        if( SendDlgItemMessage( hwnd , IDC_CHECK_DJ , BM_GETCHECK , 0 , 0 ) ) {
          CHECKED_DJ = TRUE ;
        }
        else {
          CHECKED_DJ = FALSE ;
        }
        break ;
      } // end of case BN_CLICKED
      } // end of switch( HIWORD( wParam ) )
      break ;
    } // end of case IDC_CHECK_DJ
    case IDC_CHECK_DP : {
      switch( HIWORD( wParam ) ) {
      case BN_CLICKED : {
        if( SendDlgItemMessage( hwnd , IDC_CHECK_DP , BM_GETCHECK , 0 , 0 ) ) {
          CHECKED_DP = TRUE ;
        }
        else {
          CHECKED_DP = FALSE ;
        }
        break ;
      } // end of case BN_CLICKED
      } // end of switch( HIWORD( wParam ) )
      break ;
    } // end of case IDC_CHECK_DP
    case IDC_CHECK_DR : {
      switch( HIWORD( wParam ) ) {
      case BN_CLICKED : {
        if( SendDlgItemMessage( hwnd , IDC_CHECK_DR , BM_GETCHECK , 0 , 0 ) ) {
          CHECKED_DR = TRUE ;
        }
        else {
          CHECKED_DR = FALSE ;
        }
        break ;
      } // end of case BN_CLICKED
      } // end of switch( HIWORD( wParam ) )
      break ;
    } // end of case IDC_CHECK_DR
    case IDC_CHECK_DD : {
      switch( HIWORD( wParam ) ) {
      case BN_CLICKED : {
        if( SendDlgItemMessage( hwnd , IDC_CHECK_DD , BM_GETCHECK , 0 , 0 ) ) {
          CHECKED_DD = TRUE ;
        }
        else {
          CHECKED_DD = FALSE ;
        }
        break ;
      } // end of case BN_CLICKED
      } // end of switch( HIWORD( wParam ) )
      break ;
    } // end of case IDC_CHECK_DD
    case IDC_SELECT_PROJECT : {
      HWND hEdit ;
      LPITEMIDLIST pidlBrowse ;
      BROWSEINFO BRinfo ;
      hEdit = GetDlgItem( hwnd_main , ID_PROJECT_FOLDER ) ;
      BRinfo.hwndOwner = hwnd ;
      BRinfo.pidlRoot = NULL ;
      BRinfo.pszDisplayName = NULL ;
      BRinfo.lpszTitle = "选择项目根目录" ;
      BRinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE ;
      BRinfo.lpfn = NULL ;
      BRinfo.lParam = NULL ;
      pidlBrowse = SHBrowseForFolder( &BRinfo ) ;
      if( pidlBrowse != NULL ) {
        char szPath[MAX_PATH] ;
        SHGetPathFromIDList( pidlBrowse , szPath ) ;
        SetWindowText( hEdit , szPath ) ;
      }
      break ;
    } // end of case IDC_SELECT_PROJECT
    case IDC_SELECT_LOG : {
      HWND hEdit ;
      OPENFILENAMEA ofn ;
      char szFileName[MAX_PATH] = "" ;
      hEdit = GetDlgItem( hwnd_main , ID_COMPILE_LOG ) ;
      ZeroMemory( &ofn , sizeof( ofn ) ) ;
      ofn.lStructSize = sizeof( ofn ) ;
      ofn.hwndOwner = hwnd ;
      ofn.lpstrFilter = "Log File (*.log)\0*.log\0All Files (*.*)\0*.*\0" ;
      ofn.lpstrFile = szFileName ;
      ofn.nMaxFile = MAX_PATH ;
      ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ;
      ofn.lpstrDefExt = "log" ;
      if( GetOpenFileNameA( &ofn ) ) {
        SetWindowText( hEdit , szFileName ) ;
      }
      break ;
    } // end of case IDC_SELECT_LOG
    case IDC_SELECT_DUMP_DIR : {
      break ;
    } // end of case IDC_SELECT_DUMP_DIR :
    case IDC_GENERATE_CMD : {
      HWND hPro ;
      HWND hLog ;
      HWND hDump ;
      HWND hCmd ;
      HANDLE hFile ;
      char projDir[MAX_PATH] ;
      char logFileName[MAX_PATH] ;
      char dumpDir[MAX_PATH] ;
      char* cmd ;
      cmd = ( char* )malloc( sizeof( char ) * BUF_SIZE ) ;
      memset( cmd , 0 , BUF_SIZE ) ;
      
      hPro = GetDlgItem( hwnd_main , ID_PROJECT_FOLDER ) ;
      hLog = GetDlgItem( hwnd_main , ID_COMPILE_LOG ) ;
      hDump = GetDlgItem( hwnd_main , ID_DUMP_DIR ) ;
      hCmd = GetDlgItem( hwnd_main , ID_CMD ) ;
      
      GetWindowText( hPro , projDir , MAX_PATH ) ;
      GetWindowText( hLog , logFileName , MAX_PATH ) ;
      GetWindowText( hDump , dumpDir , MAX_PATH ) ;
      if( CHECKED_PL || CHECKED_DL || CHECKED_DS || CHECKED_BK ) {
        hFile = CreateFile( logFileName , GENERIC_READ , FILE_SHARE_READ , NULL ,
                            OPEN_EXISTING , 0 , NULL ) ;
        if( hFile != INVALID_HANDLE_VALUE ) {
          DWORD dwFileSize ;
          dwFileSize = GetFileSize( hFile , NULL ) ;
          if( dwFileSize != 0xffffffff ) {
            LPSTR logFileText ;
            LPSTR logFileText_ ;
            logFileText = GlobalAlloc( GPTR , dwFileSize + 1 ) ;
            logFileText_ = GlobalAlloc( GPTR , dwFileSize + 1 ) ;
            if( logFileText != NULL ) {
              DWORD dwRead ;
              if( ReadFile( hFile , logFileText_ , dwFileSize , &dwRead , NULL ) ) {
                logFileText_[dwFileSize] = 0 ;
                char* pch = NULL ;
                char* ch ;
                if( CHECKED_BK ) {
                  char* tmpDir ;
                  DWORD dwAttr ;
                  char* ch ;
                  tmpDir = ( char* )malloc( sizeof( char ) * BUF_SIZE ) ;
                  memset( tmpDir , 0 , BUF_SIZE ) ;
                  memcpy( logFileText , logFileText_ , dwFileSize + 1 ) ;
                  pch = strtok( logFileText , "\n" ) ;
                  while( pch != NULL ) {
                    if( strstr( pch , "Install:" ) ) {
                      strcat( cmd , "adb pull " ) ;
                      strcat( cmd , "/" ) ;
                      strcat( cmd , pch + 39 ) ;
                      strcat( cmd , " " ) ;
                      strcat( cmd , dumpDir ) ;
                      ch = pch ;
                      while( *ch != '\0' ) {
                        if( *ch == '/' ) *ch = '\\' ;
                        ch ++ ;
                      }
                      strcat( cmd , pch + 38 ) ;
                      strcat( tmpDir , dumpDir ) ;
                      strcat( tmpDir , pch + 38 ) ;
                      ch = tmpDir ;
                      while( *ch++ )  ;
                      while( *ch != '\\' ) ch-- ;
                      *ch = '\0' ;
                      dwAttr = GetFileAttributes( tmpDir ) ;
                      if( dwAttr != 0xffffffff && ( dwAttr & FILE_ATTRIBUTE_DIRECTORY ) ) {
                      } // exists
                      else {
                        char* path ;
                        char* end ;
                        char* folder ;
                        path = ( char* )malloc( sizeof( char ) * ( strlen( tmpDir ) + 2 ) ) ;
                        folder = ( char* )malloc( sizeof( char ) * BUF_SIZE ) ;
                        memset( path , 0 , strlen( tmpDir ) + 2 ) ;
                        memset( folder , 0 , BUF_SIZE ) ;
                        memcpy( path , tmpDir , strlen( tmpDir ) ) ;
                        path[ strlen( tmpDir ) ] = '\\' ;
                        path[ strlen( tmpDir ) + 1 ] = '\0' ;
                        end = strchr( path , '\\' ) ;
                        while( end != NULL ) {
                          strncpy( folder , path , end - path + 1 ) ;
                          if( !CreateDirectory( folder , NULL ) ) {
                            DWORD err = GetLastError( ) ;
                            if( err != ERROR_ALREADY_EXISTS ) {
                            }
                          }
                          end = strchr( ++end , '\\' ) ;
                        }
                        free( folder ) ;                        
                      } // not exists
                      tmpDir[0] = '\0' ;
                      strcat( cmd , "\r\n" ) ;
                    } // end of if( strstr( pch , "Install" ) )
                    pch = strtok( NULL , "\n" ) ;
                  } // end of while( pch != NULL )
                  strcat( cmd , "\r\n" ) ;
                  free( tmpDir ) ;
                } // end of if( CHECKED_BK )
                if( CHECKED_PL ) {
                  memcpy( logFileText , logFileText_ , dwFileSize + 1 ) ;
                  pch = strtok( logFileText , "\n" ) ;
                  while( pch != NULL ) {
                    if( strstr( pch , "Install:" ) ) {
                      strcat( cmd , "adb push " ) ;
                      strcat( cmd , projDir ) ;
                      strcat( cmd , "\\" ) ;
                      ch = pch ;
                      while( *ch != '\0' ) {
                        if( *ch == '/' ) *ch = '\\' ;
                        ch ++ ;
                      }
                      strcat( cmd , pch + 9 ) ;
                      strcat( cmd , " /" ) ;
                      ch = pch ;
                      while( *ch != '\0' ) {
                        if( *ch == '\\' ) *ch = '/' ;
                        ch ++ ;
                      }
                      strcat( cmd , pch + 39 ) ;
                      strcat( cmd , "\r\n" ) ;
                    } // end of if( strstr( pch , "Install" ) )
                    pch = strtok( NULL , "\n" ) ;
                  } // end of while( pch != NULL )
                  strcat( cmd , "\r\n" ) ;
                } // end of if( CHECKED_PL )
                if( CHECKED_DL ) {
                  char* tmpDir ;
                  tmpDir = ( char* )malloc( sizeof( char ) * BUF_SIZE ) ;
                  memset( tmpDir , 0 , BUF_SIZE ) ;
                  memcpy( logFileText , logFileText_ , dwFileSize + 1 ) ;
                  pch = strtok( logFileText , "\n" ) ;
                  while( pch != NULL ) {
                    if( strstr( pch , "Install:" ) ) {
                      strcat( cmd , "xcopy " ) ;
                      strcat( cmd , projDir ) ;
                      strcat( cmd , "\\" ) ;
                      ch = pch ;
                      while( *ch != '\0' ) {
                        if( *ch == '/' ) *ch = '\\' ;
                        ch ++ ;
                      }
                      strcat( cmd , pch + 9 ) ;
                      strcat( cmd , " " ) ;
                      
                      ch = pch ;
                      while( *ch != '\0' ) {
                        if( *ch == '/' ) *ch = '\\' ;
                        ch ++ ;
                      }
                      strcat( tmpDir , dumpDir ) ;
                      strcat( tmpDir , pch + 38 ) ;
                      ch = tmpDir ;
                      while( *ch++ )  ;
                      while( *ch != '\\' ) ch-- ;
                      ch++ ;
                      *ch = '\0' ;
                      strcat( cmd , tmpDir ) ;
                      memset( tmpDir , 0 , BUF_SIZE ) ;
                      strcat( cmd , "\r\n" ) ;
                    } // end of if( strstr( pch , "Install" ) )
                    pch = strtok( NULL , "\n" ) ;
                  } // end of while( pch != NULL )
                  strcat( cmd , "\r\n" ) ;
                  free( tmpDir ) ;
                } // end of if( CHECKED_DL )
                if( CHECKED_DS ) {
                  char* tmpDir ;
                  tmpDir = ( char* )malloc( sizeof( char ) * BUF_SIZE ) ;
                  memset( tmpDir , 0 , BUF_SIZE ) ;
                  memcpy( logFileText , logFileText_ , dwFileSize + 1 ) ;
                  pch = strtok( logFileText , "\n" ) ;
                  while( pch != NULL ) {
                    if( strstr( pch , "target Symbolic:" ) ) {
                      strcat( cmd , "xcopy " ) ;
                      strcat( cmd , projDir ) ;
                      strcat( cmd , "\\" ) ;
                      ch = pch ;
                      while( *ch != '\0' ) {
                        if( *ch == '/' ) *ch = '\\' ;
                        ch ++ ;
                      }
                      strcat( cmd , strchr( pch , '(' ) + 1 ) ;
                      ch = cmd ;
                      while( *ch++ ) ;
                      ch -= 2 ;
                      *ch = '\0' ;
                      strcat( cmd , " " ) ;

                      ch = pch ;
                      while( *ch != '\0' ) {
                        if( *ch == '/' ) *ch = '\\' ;
                        ch ++ ;
                      }
                      strcat( tmpDir , dumpDir ) ;
                      strcat( tmpDir , strstr( pch , "symbols" ) - 1 ) ;
                      ch = tmpDir ;
                      while( *ch++ )  ;
                      while( *ch != '\\' ) ch-- ;
                      ch++ ;
                      *ch = '\0' ;
                      strcat( cmd , tmpDir ) ;
                      memset( tmpDir , 0 , BUF_SIZE ) ;
                      strcat( cmd , "\r\n" ) ;
                    } // end of if( strstr( pch , "Install" ) )
                    pch = strtok( NULL , "\n" ) ;
                  } // end of while( pch != NULL )
                  strcat( cmd , "\r\n" ) ;
                  free( tmpDir ) ;
                } // end of if( CHECKED_DS )
              } // end of if( ReadFile( ...
            } // end of if( logFileText != NULL )
            GlobalFree( logFileText ) ;
            GlobalFree( logFileText_ ) ;
          } // end of if( dwFileSize != 0xffffff )
          CloseHandle( hFile ) ;
        } // end of if( hFile != INVALID_HANDLE_VALUE )
      } // end of if( CHECKED_PL || CHECKED_DL || CHECKED_DS || CHECKED_BK )
      if( CHECKED_DJ ) {
      } // end of if( CHECKED_DJ )
      if( CHECKED_DP ) {
      } // end of if( CHECKED_DP )
      if( CHECKED_DR ) {
      } // end of if( CHECKED_DR )
      if( CHECKED_DD ) {
      } // end of if( CHECKED_DD )
      SetWindowText( hCmd , cmd ) ;
      free( cmd ) ;
      break ;
    } // end of case IDC_GENERATE_CMD
    case IDC_EXECUTE_CMD : {
      HWND hCmd ;
      char* cmd ;
      char* cmd_buf ;
      char* pch = NULL ;
      STARTUPINFO si ;
      PROCESS_INFORMATION pi ;

      cmd = ( char* )malloc( sizeof( char ) * BUF_SIZE ) ;
      cmd_buf = ( char* )malloc( sizeof( char ) * BUF_SIZE ) ;
      memset( cmd , 0 , BUF_SIZE ) ;
      memset( cmd_buf , 0 , BUF_SIZE ) ;

      hCmd = GetDlgItem( hwnd_main , ID_CMD ) ;
      GetWindowText( hCmd , cmd_buf , 1024*128 ) ;
      ZeroMemory( &si , sizeof( si ) ) ;
      si.cb = sizeof( si ) ;
      ZeroMemory( &pi , sizeof( pi ) ) ;

      strcat( cmd , "cmd /c @echo off & " ) ;
      pch = strtok( cmd_buf , "\r\n" ) ;
      while( pch != NULL ) {
        strcat( cmd , "echo " ) ;
        strcat( cmd , pch ) ;
        strcat( cmd , " & " ) ;
        strcat( cmd , pch ) ;
        strcat( cmd , " & " ) ;
        pch = strtok( NULL , "\r\n" ) ;
      }
      strcat( cmd , " echo DONE & " ) ;
      strcat( cmd , " pause>nul" ) ;
      CreateProcess( 0 ,
                     cmd ,
                     NULL ,
                     NULL ,
                     TRUE ,
                     0 ,
                     NULL ,
                     "C:\\windows\\system32\\" ,
                     &si ,
                     &pi ) ;
      WaitForSingleObject( pi.hProcess , INFINITE ) ;
      CloseHandle( pi.hThread ) ;
      CloseHandle( pi.hProcess ) ;
      /* MessageBox( hwnd , cmd , "CMD" , MB_OK ) ; */
      free( cmd ) ;
      free( cmd_buf ) ;
      break ;
    } // end of case IDC_GENERATE_CMD
    } // end of switch( LOWORD( wParam ) )
    break ;
  default :
    return FALSE ;
  } // end of switch( LOWORD( wParam ) ) 
  return TRUE ;
}


LRESULT CALLBACK WndProc( HWND hwnd ,
                          UINT msg ,
                          WPARAM wParam ,
                          LPARAM lParam ) {
  switch( msg ) {
  case WM_CREATE : {
    HMENU hMenu , hSubMenu ;
    HICON hIcon , hIconSm ;
    HFONT hfDefault ;

    hMenu = CreateMenu( ) ;
    hSubMenu = CreatePopupMenu( ) ;
    AppendMenu( hSubMenu ,
                MF_STRING ,
                ID_FILE_EXIT ,
                "E&xit" ) ;
    AppendMenu( hMenu ,
                MF_STRING | MF_POPUP ,
                ( UINT )hSubMenu ,
                "&File" ) ;

    hSubMenu = CreatePopupMenu( ) ;
    AppendMenu( hSubMenu ,
                MF_STRING ,
                ID_DIALOG_SHOW ,
                "&Show Toolbar" ) ;
    AppendMenu( hSubMenu ,
                MF_STRING ,
                ID_DIALOG_HIDE ,
                "&Hide Toolbar" ) ;
    AppendMenu( hMenu ,
                MF_STRING | MF_POPUP ,
                ( UINT )hSubMenu ,
                "&Toolbar" ) ;

    hSubMenu = CreatePopupMenu( ) ;
    AppendMenu( hSubMenu ,
                MF_STRING ,
                ID_HELP_ABOUT ,
                "&About" ) ;
    AppendMenu( hMenu ,
                MF_STRING | MF_POPUP ,
                ( UINT )hSubMenu ,
                "&Help" ) ;

    SetMenu( hwnd , hMenu ) ;

    g_hToolbar = CreateDialog( GetModuleHandle( NULL ) ,
                               MAKEINTRESOURCE( IDD_TOOLBAR ) ,
                               hwnd ,
                               ToolDlgProc ) ;
    if( g_hToolbar != NULL ) {
      ShowWindow( g_hToolbar , SW_SHOW ) ;
    }
    else {
      MessageBox( hwnd ,
                  "[-] CreateDialog" ,
                  "Warning" ,
                  MB_OK | MB_ICONEXCLAMATION ) ;
    } // end of if( g_hToolbar != NULL )
    
    hfDefault = GetStockObject( DEFAULT_GUI_FONT ) ;
    hwnd_pro = CreateWindowEx( WS_EX_CLIENTEDGE ,
                                "edit" ,
                                /* "[项目根目录]" , */
                               "Z:\\proj\\PD1616_slf" ,
                                WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | ES_MULTILINE ,
                                0 , 0 , 0 , 0 ,
                                hwnd ,
                                ( HMENU )( ID_PROJECT_FOLDER ) ,
                                GetModuleHandle( NULL ) ,
                                NULL ) ;
    if( hwnd_pro == NULL ) {
      MessageBox( hwnd ,
                  "CreateWindowEx pro" ,
                  "Error" ,
                  MB_OK | MB_ICONERROR ) ;
    } // end of if( hwnd_pro == NULL ) 
    SendMessage( hwnd_pro , WM_SETFONT , ( WPARAM)hfDefault , MAKELPARAM( FALSE , 0 ) ) ;

    hwnd_log = CreateWindowEx( WS_EX_CLIENTEDGE ,
                                "edit" ,
                                /* "[编译日志]" , */
                               "Z:\\proj\\PD1616_slf\\compile_lib.log" ,
                                WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | ES_MULTILINE ,
                                0 , 0 , 0 , 0 ,
                                hwnd ,
                                ( HMENU )( ID_COMPILE_LOG ) ,
                                GetModuleHandle( NULL ) ,
                                NULL ) ;
    if( hwnd_log == NULL ) {
      MessageBox( hwnd ,
                  "CreateWindowEx log" ,
                  "Error" ,
                  MB_OK | MB_ICONERROR ) ;
    } // end of if( hwnd_log == NULL )
    SendMessage( hwnd_log , WM_SETFONT , ( WPARAM)hfDefault , MAKELPARAM( FALSE , 0 ) ) ;

    hwnd_dump = CreateWindowEx( WS_EX_CLIENTEDGE ,
                                "edit" ,
                                /* "[复制/导出目录]" , */
                                "D:\\Desktop\\dump_dir" ,
                                WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | ES_MULTILINE ,
                                0 , 0 , 0 , 0 ,
                                hwnd ,
                                ( HMENU )( ID_DUMP_DIR ) ,
                                GetModuleHandle( NULL ) ,
                                NULL ) ;
    if( hwnd_dump == NULL ) {
      MessageBox( hwnd ,
                  "CreateWindowEx cmd" ,
                  "Error" ,
                  MB_OK | MB_ICONERROR ) ;
    } // end of if( hwnd_dump == NULL )
    SendMessage( hwnd_dump , WM_SETFONT , ( WPARAM)hfDefault , MAKELPARAM( FALSE , 0 ) ) ;

    hwnd_cmd = CreateWindowEx( WS_EX_CLIENTEDGE ,
                                "edit" ,
                                "[生成的待执行的命令]" ,
                                WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | ES_MULTILINE ,
                                0 , 0 , 0 , 0 ,
                                hwnd ,
                                ( HMENU )( ID_CMD ) ,
                                GetModuleHandle( NULL ) ,
                                NULL ) ;
    if( hwnd_cmd == NULL ) {
      MessageBox( hwnd ,
                  "CreateWindowEx cmd" ,
                  "Error" ,
                  MB_OK | MB_ICONERROR ) ;
    } // end of if( hwnd_cmd == NULL )
    SendMessage( hwnd_cmd , WM_SETFONT , ( WPARAM)hfDefault , MAKELPARAM( FALSE , 0 ) ) ;

    break ;
  } // end of case WM_CREATE
  case WM_SIZE : {
    HWND hEdit ;
    RECT rcClient ;
    GetClientRect( hwnd , &rcClient ) ;
    hEdit = GetDlgItem( hwnd , ID_PROJECT_FOLDER ) ;
    SetWindowPos( hEdit ,
                  NULL ,
                  210 , // x
                  4 , // y
                  rcClient.right - 210 - 4 ,
                  44 ,
                  SWP_NOZORDER ) ;
    hEdit = GetDlgItem( hwnd , ID_COMPILE_LOG ) ;
    SetWindowPos( hEdit ,
                  NULL ,
                  210 , // x
                  4 + 44 + 4 , // y
                  rcClient.right - 210 - 4 ,
                  44 ,
                  SWP_NOZORDER ) ;
    hEdit = GetDlgItem( hwnd , ID_DUMP_DIR ) ;
    SetWindowPos( hEdit ,
                  NULL ,
                  210 , // x
                  4 + 44 + 4 + 44 + 4 , // y
                  rcClient.right - 210 - 4 ,
                  44 ,
                  SWP_NOZORDER ) ;
    hEdit = GetDlgItem( hwnd , ID_CMD ) ;
    SetWindowPos( hEdit ,
                  NULL ,
                  210 , // x
                  4 + 44 + 4 + 44 + 4 + 44 + 4 , // y
                  rcClient.right - 210 - 4 ,
                  rcClient.bottom - ( 4 + 44 + 4 + 44 + 4 + 44 + 4 ) - 4 ,
                  SWP_NOZORDER ) ;
    break ;
  } // end of case WM_SIZE
  case WM_COMMAND : {
    switch( LOWORD( wParam ) ) {
    case ID_FILE_EXIT :
      PostMessage( hwnd , WM_CLOSE , 0 , 0 ) ;
      break ;
    case ID_HELP_ABOUT : {
      int ret = DialogBox( GetModuleHandle ( NULL ) ,
                           MAKEINTRESOURCE( IDD_ABOUT ) ,
                           hwnd ,
                           AboutDlgProc ) ;
      if( ret == IDOK ) {
      }
      else if( ret == IDCANCEL ) {
      }
      else if( ret == -1 ) {
        MessageBox( hwnd ,
                    "Dialog failed" ,
                    "Error" ,
                    MB_OK | MB_ICONINFORMATION ) ;
      }// end of if( ret == IDOK )
      break ;
    } // end of case ID_HELP_ABOUT
    case ID_DIALOG_SHOW :
      ShowWindow( g_hToolbar , SW_SHOW ) ;
      break ;
    case ID_DIALOG_HIDE :
      ShowWindow( g_hToolbar , SW_HIDE ) ;
      break ;
    } // end of switch( LOWORD( wParam ) )
    break ;
  } // end of case WM_COMMAND :
  /* case WM_LBUTTONDOWN : { */
  /*   char szFileName[ MAX_PATH ] ; */
  /*   HINSTANCE hInstance = GetModuleHandle( NULL ) ; */
  /*   GetModuleFileName( hInstance , */
  /*                      szFileName , */
  /*                      MAX_PATH ) ; */
  /*   MessageBox( hwnd , */
  /*               szFileName , */
  /*               "This program is : " , */
  /*               MB_OK | MB_ICONEXCLAMATION ) ; */
  /* } */
  /*   break ; */
  case WM_CLOSE :
    DestroyWindow( hwnd ) ;
    break ;
  case WM_DESTROY :
    DestroyWindow( g_hToolbar ) ;
	DestroyWindow( hwnd_pro ) ;
	DestroyWindow( hwnd_log ) ;
    DestroyWindow( hwnd_dump ) ;
	DestroyWindow( hwnd_cmd ) ;
    PostQuitMessage( 0 ) ;
    break ;
  default :
    return DefWindowProc( hwnd , msg , wParam , lParam ) ;
  } // end of switch( msg )
} // end of LRESULT CALLBACK WndProc( HWND hwnd 




int WINAPI WinMain( HINSTANCE hInstance ,
                    HINSTANCE hPrevInstance ,
                    LPSTR lpCmdLine ,
                    int nCmdShow ) {
  WNDCLASSEX wc ;
  HWND hwnd ;
  MSG Msg ;
  void* hAccelerator ;

  hAccelerator = LoadAccelerators( hInstance , IDA_MAIN ) ;

  wc.cbSize = sizeof( WNDCLASSEX ) ;
  wc.style = 0 ;
  wc.lpfnWndProc = WndProc ;
  wc.cbClsExtra = 0 ;
  wc.cbWndExtra = 0 ;
  wc.hInstance = hInstance ;
  wc.hIcon = LoadIcon( GetModuleHandle( NULL ) ,
                       MAKEINTRESOURCE( IDI_MYICON ) ) ;
  /* wc.hIconSm = ( HICON )LoadIcon( NULL , */
  /*                                 MAKEINTRESOURCE( IDI_MYICON ) , */
  /*                                 IMAGE_ICON , */
  /*                                 16 , */
  /*                                 16 , */
  /*                                 0 ) ; */
  wc.hIconSm = wc.hIcon ;
  wc.hCursor = LoadCursor( NULL , IDC_ARROW ) ;
  wc.hbrBackground = ( HBRUSH )( COLOR_WINDOW +1 ) ;
  wc.lpszMenuName = MAKEINTRESOURCE( IDR_MYMENU ) ;
  wc.lpszClassName = g_szClassName ;

  if( !RegisterClassEx( &wc ) ) {
    MessageBox( NULL ,
                "[-] RegisterClassEx" ,
                "Error" ,
                MB_ICONEXCLAMATION | MB_OK ) ;
    return 0 ;
  } // end of if( !RegisterClassEx( &wc ) )

  hwnd = CreateWindowEx( WS_EX_CLIENTEDGE ,
                         g_szClassName ,
                         "qyh_adb" ,
                         WS_OVERLAPPEDWINDOW ,
                         CW_USEDEFAULT ,
                         CW_USEDEFAULT ,
                         1200 ,
                         768 ,
                         NULL ,
                         NULL ,
                         hInstance ,
                         NULL ) ;
  if( hwnd == NULL ) {
    MessageBox( NULL ,
                "[-] CreateWindowEx" ,
                "Error" ,
                MB_ICONEXCLAMATION | MB_OK ) ;
    return 0 ;
  } // end of if( hwnd == NULL )
  hwnd_main = hwnd ;

  ShowWindow( hwnd , nCmdShow ) ;
  UpdateWindow( hwnd ) ;

  while( GetMessage( &Msg , NULL , 0 , 0 ) > 0 ) {
    if(
       !IsDialogMessage( g_hToolbar , &Msg ) &&
       !TranslateAccelerator( hwnd , hAccelerator , &Msg )
       ) {
      TranslateMessage( &Msg ) ;
      DispatchMessage( &Msg ) ;
    } // end of if(
  } // end of while( GetMessage( &Msg , NULL , 0 , 0 ) > 0 )
  
  return Msg.wParam ;
}
