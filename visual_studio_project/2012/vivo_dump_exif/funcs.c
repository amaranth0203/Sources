#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "funcs.h"
#include <windows.h>
#include "Shlwapi.h"
#pragma comment( lib , "Shlwapi.lib" )

void test( ) {
  PRINTF( "[+] wassup test\n" ) ;
#ifdef _DEBUG_
  PRINTF( "[+] wassup test -- debug\n" ) ;
#endif
}

void dump_exif_info( wchar_t* f_name , DWORD* color_temperature , DWORD* lux_index ) {
  HANDLE hFile = CreateFileW( 
    f_name , 
    GENERIC_READ , 
    FILE_SHARE_READ , 
    NULL , 
    OPEN_EXISTING , 
    0 , 
    NULL ) ;
  if( hFile != INVALID_HANDLE_VALUE ) {
    DWORD dwFileSize = GetFileSize(hFile, NULL); 
    if( dwFileSize != INVALID_FILE_SIZE ) {
      BYTE* pszFileText = (BYTE*) LocalAlloc(LMEM_FIXED, dwFileSize); 
      if( pszFileText != NULL ) {
        DWORD dwRead ;
        JPEG_HEADER* jpeg_header ;
        QDEBUG_HEADER* qdebug_header ;
        QCAEC_HEADER* qcaec_header ;
        QCAWB_HEADER* qcawb_header ;
        DWORD size ;
        float f ;

        ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL);
        if( dwRead < dwFileSize ) {
          *color_temperature = *lux_index = INVALID_VALUE ;
          LocalFree(pszFileText) ;
          CloseHandle( hFile ) ;
          return ;
        } // end of if( dwRead < dwFileSize )

        /******************/
        /* read exif info */
        jpeg_header = ( JPEG_HEADER* )pszFileText ;
        if( jpeg_header->SOI[0] != 0xff || jpeg_header->SOI[1] != 0xd8 ) {
          *color_temperature = *lux_index = INVALID_VALUE ;
          LocalFree(pszFileText) ;
          CloseHandle( hFile ) ;
          return ;
        }
        qdebug_header = ( QDEBUG_HEADER* )( 
          pszFileText 
          + ( jpeg_header->size[0] << 8 | jpeg_header->size[1] ) 
          + sizeof( JPEG_HEADER ) 
          ) ;
        if( 0 != strcmp( qdebug_header->marker_2 , QDEBUG_MARKER ) ) {
          *color_temperature = *lux_index = INVALID_VALUE ;
          LocalFree(pszFileText) ;
          CloseHandle( hFile ) ;
          return ;
        }
        PRINTF( "[+] qdebug marker [%s]\n" , qdebug_header->marker_2 ) ;
        qcaec_header = ( QCAEC_HEADER* )( 
          ( BYTE* )qdebug_header 
          + sizeof( QDEBUG_HEADER ) 
          ) ;
        size = 
          qcaec_header->size[0] | 
          qcaec_header->size[1] << 8 | 
          qcaec_header->size[2] << 16 | 
          qcaec_header->size[3] << 24 ;
        PRINTF( "[+] qcaec marker [%s] size [0x%x] data offset in file [0x%x]\n" ,
                qcaec_header->marker_1 ,
                size ,
                qcaec_header->data - pszFileText
          ) ;
        *lux_index = 
          qcaec_header->lux_index[1] << 8 | 
          qcaec_header->lux_index[0] ;
        PRINTF( "[+] lux index : %d\n" , *lux_index ) ;
        qcawb_header = ( QCAWB_HEADER* )( 
          ( BYTE* )qcaec_header 
          + sizeof( qcaec_header->marker_1 ) 
          + sizeof( qcaec_header->size ) + size 
          ) ;
        size = 
          qcawb_header->size[0] | 
          qcawb_header->size[1] << 8 | 
          qcawb_header->size[2] << 16 |
          qcawb_header->size[3] << 24 ;
        PRINTF( "[+] qcawb marker [%s] size [0x%x] data offset in file [0x%x]\n" ,
                qcawb_header->marker_1 ,
                size ,
                qcawb_header->data_qcom - pszFileText
          ) ;
        if( /* ALC marker found */
          'A' == qcawb_header->alcs[0] &&
          'L' == qcawb_header->alcs[1] &&
          'C' == qcawb_header->alcs[2] &&
          'S' == qcawb_header->alcs[3]
          ) {
          PRINTF( "[+] alc marker [%c%c%c%c] data offset in file [0x%x]\n" , 
                  qcawb_header->alcs[0] ,
                  qcawb_header->alcs[1] ,
                  qcawb_header->alcs[2] ,
                  qcawb_header->alcs[3] ,
                  qcawb_header->alcs - pszFileText
            ) ;
          PRINTF( "[+] alc marker [%x%x] data offset in file [0x%x]\n" , 
                  qcawb_header->color_temperature_alc[0] ,
                  qcawb_header->color_temperature_alc[1] ,
                  qcawb_header->color_temperature_alc - pszFileText
            ) ;
          *color_temperature = 
            qcawb_header->color_temperature_alc[1] << 8 |
            qcawb_header->color_temperature_alc[0] ;
        }
        else { /* awb goes qcom way */
          *color_temperature = 
            qcawb_header->color_temperature_qcom[1] << 8 | 
            qcawb_header->color_temperature_qcom[0] ;
        }
        PRINTF( "[+] color temperature : %d\n" , *color_temperature ) ;
        /* read exif info end */
        /**********************/
        
        LocalFree(pszFileText);
      } // end of if( pszFileText != NULL )
      else {
        *color_temperature = *lux_index = INVALID_VALUE ;
        PRINTF( "[-] pszFileText == NULL\n" ) ;
      }
    } // end of if( dwFileSize != INVALID_FILE_SIZE )
    else {
      *color_temperature = *lux_index = INVALID_VALUE ;
      PRINTF( "[-] dwFileSize == INVALID_FILE_SIZE\n" ) ;
    }
    CloseHandle( hFile ); 
  } // end of if( hFile != INVALID_HANDLE_VALUE )
  else {
    *color_temperature = *lux_index = INVALID_VALUE ;
    PRINTF( "[-] INVALID_HANDLE_VALUE\n" ) ;
  }
  return 0;
}


void write_to_file( wchar_t* f_name , DWORD color_temperature , DWORD lux_index , char* output_f_name ) {
  HANDLE hFile_out = CreateFileW(
    output_f_name ,
    FILE_APPEND_DATA , //GENERIC_WRITE ,
    0 ,
    NULL ,
    OPEN_ALWAYS , //CREATE_ALWAYS ,
    FILE_ATTRIBUTE_NORMAL ,
    NULL
    ) ;
  if( hFile_out != INVALID_HANDLE_VALUE ) {
    DWORD dwWrite ;
    DWORD buf_size =
      2 *
      (
        wcslen( PathFindFileNameW( f_name ) )
        + 1 // \t
        + sizeof( DWORD ) // color temperature
        + 1 // \t
        + sizeof( DWORD ) // lux index
        + 2 // \r\n
        )
      - 4 // end of line is null
      ;
    BYTE* buf = ( BYTE* )LocalAlloc( 
      LMEM_FIXED , 
      buf_size
      ) ;
    BYTE* ptr = buf ;

    if( buf != NULL ) {
      swprintf( 
        ptr , 
        buf_size ,
        L"%s" , 
        PathFindFileNameW( f_name ) 
        ) ;
      ptr = ptr + 2*wcslen( PathFindFileNameW( f_name ) ) ;
      swprintf(
        ptr ,
        buf_size ,
        L"\t%d\t%d\r\n" ,
        color_temperature ,
        lux_index
        ) ;
      WriteFile(
        hFile_out ,
        buf ,
        buf_size ,
        &dwWrite ,
        NULL
        ) ;
      LocalFree( buf ) ;
    } // end of if( buf != NULL )
    else {
      PRINTF( "[-] buf == NULL\n" ) ;
    }
  } // end of if( hFile_out != INVALID_HANDLE_VALUE )
  else {
    PRINTF( "[-] INVALID_HANDLE_VALUE\n" ) ;
  }
  CloseHandle( hFile_out ) ;
}

void write_header( ) {
  static const BYTE g_pnByteOrderMark[] = { 0xFF, 0xFE };
  static const wchar_t info[] =
    L"\r\n" // 2
    L"    数据格式: 文件名\\t色温\\tlux_index\r\n" // 30
    L"    无效数据: 用 -44 表示\r\n" // 20
    L"    生成时间: xxxx年xx月xx日 xx时xx分xx秒\r\n" // 12 to xxxx
    L"    By qyh -- 2017.03.22\r\n"
    L"\r\n"
    L"\r\n"
    ;
  HANDLE hFile_out = CreateFileW(
    OUTPUT_FILE_NAME ,
    FILE_APPEND_DATA , //GENERIC_WRITE ,
    0 ,
    NULL ,
    OPEN_ALWAYS , //CREATE_ALWAYS ,
    FILE_ATTRIBUTE_NORMAL ,
    NULL
    ) ;
  if( hFile_out != INVALID_HANDLE_VALUE ) {
    DWORD dwWrite ;
    wchar_t buf_date[] = L"dead年be月ef日 xx时xx分xx秒" ;
    SYSTEMTIME lt ;
    DWORD buf_size = 2 * wcslen( info ) ;
    BYTE* buf = ( BYTE* )LocalAlloc(
      LMEM_FIXED ,
      buf_size
      ) ;
    BYTE* ptr = buf ;
    swprintf( 
      ptr , 
      buf_size ,
      L"%s" , 
      info
      ) ;
    ptr += 124 ;
    GetLocalTime( &lt ) ;
    swprintf( 
      buf_date , 
      2 * wcslen( buf_date ) ,
      L"%04d年%02d月%02d日 %02d时%02d分%02d秒" ,
      lt.wYear ,
      lt.wMonth ,
      lt.wDay ,
      lt.wHour ,
      lt.wMinute ,
      lt.wSecond
      ) ;
    wmemcpy( ptr , buf_date , wcslen( buf_date ) ) ;
    WriteFile(
      hFile_out ,
      g_pnByteOrderMark ,
      sizeof( g_pnByteOrderMark ) ,
      &dwWrite ,
      NULL
      ) ;
    WriteFile(
      hFile_out ,
      buf ,
      buf_size ,
      &dwWrite ,
      NULL
      ) ;
    LocalFree( buf ) ;
  } // end of if( hFile_out != INVALID_HANDLE_VALUE )
  else {
    PRINTF( "[-] INVALID_HANDLE_VALUE\n" ) ;
  }
  CloseHandle( hFile_out ) ;
}
