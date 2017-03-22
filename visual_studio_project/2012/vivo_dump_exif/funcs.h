#ifndef _FUNCS_H_
#define _FUNCS_H_
#include "data_struct.h"

#define _DEBUG_PRINT_
#define INVALID_VALUE (-44)
#define OUTPUT_FILE_NAME L"__output__.txt"
#ifdef _DEBUG_PRINT_
	#define MESSAGEBOX( ... ) MessageBox( __VA_ARGS__ )
	#define PRINTF( ... ) printf( __VA_ARGS__ )
#else
	#define MESSAGEBOX( ... ) ( void* )0
	#define PRINTF( ... ) ( void* )0
#endif

void test( ) ;
void write_header( ) ;
void dump_exif_info( char* f_name , DWORD* color_temperature , DWORD* lux_index ) ;
void write_to_file( char* f_name , DWORD color_temperature , DWORD lux_index , char* output_f_name ) ;
#endif
