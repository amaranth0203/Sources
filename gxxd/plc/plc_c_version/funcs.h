#ifndef _FUNCS_H_
#define _FUNCS_H_
#include "snap7.h"
#include <vector>
#include <string>
#include <sstream>
#include <string>
using namespace std ;

#define S7_ADDR "192.168.1.116"
/* #define S7_ADDR "127.1" */
#define S7_PORT (102)
#define LOCAL_TSAP (0)
#define REMOTE_SAP (1)
#define PLC_VAR "DB20.DBB32" // read
#define PLC_VAR2 "DB20.DBW30.0" // write
#define DLL_NAME "snap7.dll"

typedef S7Object ( *f_Cli_Create )() ;
typedef void ( *f_Cli_Destroy)( S7Object &Client );

typedef int ( *f_Cli_ConnectTo )( S7Object Client , const char *Address , int Rank , int Slot ) ;
typedef int ( *f_Cli_Disconnect )( S7Object Client ) ;

typedef int ( *f_Cli_ListBlocks )( S7Object Client , TS7BlocksList *pUsrData ) ;
typedef int ( *f_Cli_DBRead)( S7Object Client, int DBNumber, int Start, int Size, void *pUsrData );
typedef int ( *f_Cli_DBWrite )( S7Object Client, int DBNumber, int Start, int Size, void *pUsrData );

typedef struct Snap7_ops {
  f_Cli_Create Cli_Create ;
  f_Cli_Destroy Cli_Destroy ;

  f_Cli_ConnectTo Cli_ConnectTo ;
  f_Cli_Disconnect Cli_Disconnect ;

  f_Cli_ListBlocks Cli_ListBlocks ;
  f_Cli_DBRead Cli_DBRead ;
  f_Cli_DBWrite Cli_DBWrite ;
} Snap7_ops ;

typedef enum OUT_TYPE {
  T_BOOL ,
  T_BYTE ,
  T_REAL ,
  T_WORD ,
  T_DWORD
} OUT_TYPE ;

typedef struct payload {
  char raw_input[65536] ;
  char raw_readed[65536] ;
// --- [start with 'db']
  int db_number ;
  int start ;
  int size ;
  OUT_TYPE out_type ;
  char buff[65536] ;
// --- [start with 'db'] ends
  int area ;
  int length ;
  int bit ;
} payload ;
  

void test( ) ;
void init_ops( const char* dll_file_name , Snap7_ops* ops ) ;
void process( payload* p ) ;
void getMem( payload* p ) ;
string to_string( int i ) ;
void split( vector<string> &v , string s , char token ) ;

class Bundle {
  TS7Client *Client; // in snap7.h snap7.lib
  payload p ;
  string result ;
public :
  void process_input( const char* raw_input ) ;
  Bundle( const char* ip , int rank , int slot ) ;
  string get_result( ){ return this->result ; } ;
  ~Bundle( ) ;
} ;

#endif
