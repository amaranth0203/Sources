#include "funcs.h"
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <vector>
#include <string>
#include <sstream>
using namespace std ;

void test( ) {
  printf( "[+] wassup test\n" ) ;
#ifdef _DEBUG_
  printf( "[+] wassup test -- debug\n" ) ;
#endif
}


void init_ops( const char* dll_file_name , Snap7_ops* ops ) {

  HINSTANCE hDll ;

  hDll = LoadLibrary( dll_file_name ) ;
  if( hDll == 0 ) {
    printf( "[-] LoadLibrary failed\n" ) ;
    exit( -1 ) ;
  }

  ops->Cli_Create = ( f_Cli_Create )GetProcAddress( hDll , "Cli_Create" ) ;
  if( ops->Cli_Create == 0 ) {
    printf( "[-] load Cli_Create failed\n" ) ;
    exit( -1 ) ;
  }
  ops->Cli_Destroy = ( f_Cli_Destroy )GetProcAddress( hDll , "Cli_Destroy" ) ;
  if( ops->Cli_Destroy == 0 ) {
    printf( "[-] load Cli_Destroy failed\n" ) ;
    exit( -1 ) ;
  }

  ops->Cli_ConnectTo = ( f_Cli_ConnectTo )GetProcAddress( hDll , "Cli_ConnectTo" ) ;
  if( ops->Cli_ConnectTo == 0 ) {
    printf( "[-] load Cli_ConnectTo failed\n" ) ;
    exit( -1 ) ;
  }
  ops->Cli_Disconnect = ( f_Cli_Disconnect )GetProcAddress( hDll , "Cli_Disconnect" ) ;
  if( ops->Cli_Disconnect == 0 ) {
    printf( "[-] load Cli_Disconnect failed\n" ) ;
    exit( -1 ) ;
  }

  ops->Cli_ListBlocks = ( f_Cli_ListBlocks )GetProcAddress( hDll , "Cli_ListBlocks" ) ;
  if( ops->Cli_ListBlocks == 0 ) {
    printf( "[-] load Cli_ListBlock failed\n" ) ;
    exit( -1 ) ;
  }
  ops->Cli_DBRead = ( f_Cli_DBRead )GetProcAddress( hDll , "Cli_DBRead" ) ;
  if( ops->Cli_DBRead == 0 ) {
    printf( "[-] load Cli_DBRead failed\n" ) ;
    exit( -1 ) ;
  }
  ops->Cli_DBWrite = ( f_Cli_DBWrite )GetProcAddress( hDll , "Cli_DBWrite" ) ;
  if( ops->Cli_DBWrite == 0 ) {
    printf( "[-] load Cli_Write failed\n" ) ;
    exit( -1 ) ;
  }

}

void process( payload* p ) {
  if( 
    ( p->raw_input[0] == 'g' || p->raw_input[0] == 'G' ) &&
    ( p->raw_input[1] == 'e' || p->raw_input[0] == 'E' ) &&
    ( p->raw_input[2] == 't' || p->raw_input[0] == 'T' ) 
    )
    getMem( p ) ;
}

void getMem( payload* p ) {
#if 0
  if( // p->mem stand for var in config.ini like "DB20.DBB32.0"
    ( p->mem[0] == 'd' || p->mem[0] == 'D' ) &&
    ( p->mem[1] == 'b' || p->mem[1] == 'B' )
    ) {
    vector<string> arr ;
    string token ;
    istringstream ss( string( p->mem ) ) ;
    while( getline( ss , token , '.' ) ) {
      arr.push_back( token ) ;
    } // arr : [ "DB20" , "DBB32" , "0" ]
    p->db_number = atoi( &(arr[0].c_str()[2]) ) ; // 20 in "DB20"
    p->start = atoi( &(arr[1].c_str()[3] ) ) ; // 32 in "DBB32"
    switch( arr[1].c_str()[2] ) { // case 'B' in DBB32 first 'B'
    case 'x' : case 'X' :
      p->size = 1 ;
      p->out_type = T_BOOL ;
      break ;
    case 'b' : case 'B' : // this case 
      p->size = 1 ;
      p->out_type = T_BYTE ;
      break ;
    case 'w' : case 'W' :
      p->size = 2 ;
      p->out_type = T_WORD ;
      break ;
    case 'd' : case 'D' :
      p->size = 4 ;
      p->out_type = T_DWORD ;
      break ;
    }
  } // end of if( "db" )
#endif
}



string to_string( int i ) {
  std::stringstream ss ;
  ss << i ;
  return ss.str( ) ;
}

void split( vector<string> &v , string s , char token ) {
  string token_ ;
  istringstream ss1( s ) ;
  while( getline( ss1 , token_ , token ) ) {
    v.push_back( token_ ) ;
  }
}

Bundle::Bundle( const char* ip , int rank , int slot ) {
  this->Client = new TS7Client();
  this->Client->ConnectTo( ip , rank , slot ) ;
  this->result = string( "" ) ;
}

Bundle::~Bundle( ) {
  delete this->Client ;
}

void Bundle::process_input( const char* input ) {
  string raw_input = string( input ) ;
  vector<string> v1 ;
  vector<string> v2 ;
  vector<string> v3 ;
  int i ;
  this->result.clear( ) ;
  split( v1 , raw_input , ',' ) ; // v1 : [ "get:ip" , "DB21.dbx0.0" , "xx.xx" , "xx.xx" ... ]
  if(
    ( v1[0].c_str( )[0] == 'g' || v1[0].c_str( )[0] == 'G' ) &&
    ( v1[0].c_str( )[1] == 'e' || v1[0].c_str( )[0] == 'E' ) &&
    ( v1[0].c_str( )[2] == 't' || v1[0].c_str( )[0] == 'T' )
    ) {
    for( i = 1 ; i < v1.size( ) ; i ++ ) {
      if (
        ( v1[i].c_str( )[0] == 'd' || v1[i].c_str( )[0] == 'D' ) &&
        ( v1[i].c_str( )[1] == 'b' || v1[i].c_str( )[1] == 'B' )
        ) {
        v2.clear( ) ;
        split( v2 , v1[i] , '.' ) ; // v2 : [ "DB21" , "dbx0" , "0" ]
        this->p.db_number = atoi( &( v2[0].c_str( )[2] ) ) ;
        this->p.start = atoi( &( v2[1].c_str( )[3] ) ) ;
        switch( v2[1].c_str( )[2] ) { // case 'B' in "DBB32" first'B"
        case 'x' : case 'X' :
          this->p.size = 1 ;
          this->p.out_type = T_BOOL ;
          break ;
        case 'b' : case 'B' :
          this->p.size = 1 ;
          this->p.out_type = T_BYTE ;
          break ;
        case 'w' : case 'W' :
          this->p.size = 2 ;
          this->p.out_type = T_WORD ;
          break ;
        case 'd' : case 'D' :
          this->p.size = 4 ;
          this->p.out_type = T_DWORD ;
          break ;
        } // end of switch
        this->Client->DBRead(
          this->p.db_number ,
          this->p.start ,
          this->p.size ,
          this->p.raw_readed
          ) ;
        if( this->p.out_type == T_BOOL ) {
          // printf( "[+] %d\n" ,
          //         ( this->p.raw_readed[0] & ( 1 << atoi( v2[2].c_str( ) ) ) ) != 0
          // ) ;
          
          if( ( this->p.raw_readed[0] & ( 1 << atoi( v2[2].c_str( ) ) ) ) != 0 ) {
            this->result += "True" ;
          } 
          else {
            this->result += "False" ;
          }
          
          
          this->result += ":" ;
        }
        if( this->p.out_type == T_BYTE ) {
          // printf( "[+] %d\n" ,
          //         this->p.raw_readed[0]
          // ) ;
          this->result += to_string(
            this->p.raw_readed[0]
            ) ;
          this->result += ":" ;
        }
        if( this->p.out_type == T_WORD ) {
          // printf( "[+] %d\n" ,
          //         this->p.raw_readed[0] << 8 |
          //         this->p.raw_readed[1]
          // ) ;
          this->result += to_string(
                  this->p.raw_readed[0] << 8 |
                  this->p.raw_readed[1]
            ) ;
          this->result += ":" ;
        }
        if( this->p.out_type == T_DWORD ) {
          // printf( "[+] %d\n" ,
          //         this->p.raw_readed[0] << 24 |
          //         this->p.raw_readed[1] << 16 |
          //         this->p.raw_readed[2] << 8 |
          //         this->p.raw_readed[3]
          // ) ;
          this->result += to_string(
                  this->p.raw_readed[0] << 24 |
                  this->p.raw_readed[1] << 16 |
                  this->p.raw_readed[2] << 8 |
                  this->p.raw_readed[3]
            ) ;
          this->result += ":" ;
        }
      } // end of if 'db'
      else {
        this->p.area = 131 ;
        this->p.length = 1 ;
        this->p.bit = 0 ;
        this->p.start = 0 ;
        if( ( v1[i].c_str( )[0] == 'm' || v1[i].c_str( )[0] == 'M' ) ) {
          this->p.area = 131 ;
        }
        if( ( v1[i].c_str( )[0] == 'q' || v1[i].c_str( )[0] == 'Q' ) ) {
          this->p.area = 130 ;
        }
        if( ( v1[i].c_str( )[1] == 'x' || v1[i].c_str( )[1] == 'X' ) ) {
          v2.clear( ) ;
          split( v2 , v1[i] , '.' ) ; 
          this->p.length = 1 ;
          this->p.out_type = T_BOOL ;
          this->p.start = atoi( &( v2[0].c_str( )[2] ) ) ;
        }
        if( ( v1[i].c_str( )[1] == 'd' || v1[i].c_str( )[1] == 'D' ) ) {
          v2.clear( ) ;
          split( v2 , v1[i] , '.' ) ; 
          this->p.length = 4 ;
          this->p.out_type = T_DWORD ;
          this->p.start = atoi( &( v2[0].c_str( )[2] ) ) ;
        }
        if( this->p.out_type = T_BOOL ) {
          v2.clear( ) ;
          split( v2 , v1[i] , '.' ) ; 
          this->p.bit = atoi( &( v2[1].c_str( )[0] ) ) ;
        }
        this->Client->ReadArea( 
          this->p.area ,
          0 ,
          this->p.start ,
          this->p.length ,
          S7WLByte ,
          this->p.raw_readed
          ) ;
        if( this->p.out_type == T_BOOL ) {
          // printf( "[+] %d\n" ,
          //         ( this->p.raw_readed[0] & ( 1 << atoi( v2[2].c_str( ) ) ) ) != 0
          // ) ;
          this->result += to_string(
            ( this->p.raw_readed[0] & ( 1 << atoi( v2[2].c_str( ) ) ) ) != 0
            ) ;
          this->result += ":" ;
        }
        if( this->p.out_type == T_DWORD ) {
          // printf( "[+] %d\n" ,
          //         this->p.raw_readed[0] << 24 |
          //         this->p.raw_readed[1] << 16 |
          //         this->p.raw_readed[2] << 8 |
          //         this->p.raw_readed[3]
          // ) ;
          this->result += to_string(
            this->p.raw_readed[0] << 24 |
            this->p.raw_readed[1] << 16 |
            this->p.raw_readed[2] << 8 |
            this->p.raw_readed[3]
            ) ;
          this->result += ":" ;
        }
      } // end of if 'db' - else
    } // end of for v1
    this->result = this->result.substr( 0 , this->result.size( ) - 1 ) ;
  } // end of if 'get'
  else if (
    ( v1[0].c_str( )[0] == 's' || v1[0].c_str( )[0] == 'S' ) &&
    ( v1[0].c_str( )[1] == 'e' || v1[0].c_str( )[0] == 'E' ) &&
    ( v1[0].c_str( )[2] == 't' || v1[0].c_str( )[0] == 'T' )
    ) {
    for( i = 1 ; i < v1.size( ) ; i ++ ) {
      if (
        ( v1[i].c_str( )[0] == 'd' || v1[i].c_str( )[0] == 'D' ) &&
        ( v1[i].c_str( )[1] == 'b' || v1[i].c_str( )[1] == 'B' )
        ) { // v1 : [ "set:ip" , "db21.dbx0.0:1" , ... ]
        v3.clear( ) ;
        split( v3 , v1[i] , ':' ) ; // v3 : [ "db21.dbx0.0" , "1" ]
        long long value = atoi( &( v3[1].c_str( )[0] ) ) ;
        v2.clear( ) ;
        split( v2 , v3[0] , '.' ) ; // v2 : [ "db21" , "dbx0" , "0" ]
        this->p.db_number = atoi( &( v2[0].c_str( )[2] ) ) ;
        this->p.start = atoi( &( v2[1].c_str( )[3] ) ) ;
        switch( v2[1].c_str( )[2] ) { // case 'B' in "DBB32" first'B"
        case 'x' : case 'X' :
          this->p.size = 1 ;
          this->p.out_type = T_BOOL ;
          break ;
        case 'b' : case 'B' :
          this->p.size = 1 ;
          this->p.out_type = T_BYTE ;
          break ;
        case 'w' : case 'W' :
          this->p.size = 2 ;
          this->p.out_type = T_WORD ;
          break ;
        case 'd' : case 'D' :
          this->p.size = 4 ;
          this->p.out_type = T_DWORD ;
          break ;
        } // end of switch
        this->Client->DBRead(
          this->p.db_number ,
          this->p.start ,
          this->p.size ,
          this->p.raw_readed
          ) ;
        // ------ above read firset -----
        // ------ below start to write --
        v2.clear( ) ;
        split( v2 , v1[i] , '.' ) ; // v2 : [ "DB21" , "dbx0" , "0" ]
        this->p.db_number = atoi( &( v2[0].c_str( )[2] ) ) ;
        this->p.start = atoi( &( v2[1].c_str( )[3] ) ) ;
        switch( v2[1].c_str( )[2] ) { // case 'B' in "DBB32" first'B"
        case 'x' : case 'X' :
          this->p.size = 1 ;
          this->p.out_type = T_BOOL ;
          this->p.bit = atoi( &( v2[2].c_str( )[0] ) ) ;
          if( 
            !(
              ( 1 << this->p.bit ) & ( this->p.raw_readed[0] ) == // current value
              ( value ) // target value
              )
            ) {
            if(
              ( 1 << this->p.bit ) & ( this->p.raw_readed[0] ) 
              ) {
              this->p.raw_readed[0] += value ;
            }
            else {
              this->p.raw_readed[0] -= value ;
            }
          }
          break ;
        case 'b' : case 'B' :
          this->p.size = 1 ;
          this->p.out_type = T_BYTE ;
          this->p.raw_readed[0] = value ;
          break ;
        case 'w' : case 'W' :
          this->p.size = 2 ;
          this->p.out_type = T_WORD ;
          this->p.raw_readed[0] = 
            ( value & 0xff00 ) >> 8 ;
          this->p.raw_readed[1] = 
            value & 0xff ;
          break ;
        case 'd' : case 'D' :
          this->p.size = 4 ;
          this->p.out_type = T_DWORD ;
          this->p.raw_readed[0] = 
            ( value & 0xff000000 ) >> 24 ;
          this->p.raw_readed[1] = 
            ( value & 0xff0000 ) >> 16 ;
          this->p.raw_readed[2] = 
            ( value & 0xff00 ) >> 8 ;
          this->p.raw_readed[3] = 
            value & 0xff ;
          break ;
        } // end of switch
        this->Client->DBWrite( 
          this->p.db_number ,
          this->p.start ,
          this->p.size ,
          this->p.raw_readed
          ) ;
      } // end of if 'db' - else
      else {
// v1 : [ "set:ip" , "qx0.0:1" , ... ]
        this->p.area = 131 ;
        this->p.length = 1 ;
        this->p.bit = 0 ;
        this->p.start = 0 ;
        v3.clear( ) ;
        split( v3 , v1[i] , ':' ) ; // v3 : [ "qx0.0" , "1" ]
        if( ( v1[i].c_str( )[0] == 'm' || v1[i].c_str( )[0] == 'M' ) ) {
          this->p.area = 131 ;
        }
        if( ( v1[i].c_str( )[0] == 'q' || v1[i].c_str( )[0] == 'Q' ) ) {
          this->p.area = 130 ;
        }
        if( ( v1[i].c_str( )[1] == 'x' || v1[i].c_str( )[1] == 'X' ) ) {
          v2.clear( ) ;
          split( v2 , v3[0] , '.' ) ; 
          this->p.length = 1 ;
          this->p.out_type = T_BOOL ;
          this->p.start = atoi( &( v2[0].c_str( )[2] ) ) ;
        }
        if( ( v1[i].c_str( )[1] == 'd' || v1[i].c_str( )[1] == 'D' ) ) {
          v2.clear( ) ;
          split( v2 , v1[i] , '.' ) ; 
          this->p.length = 4 ;
          this->p.out_type = T_DWORD ;
          this->p.start = atoi( &( v2[0].c_str( )[2] ) ) ;
        }
        if( this->p.out_type = T_BOOL ) {
          v2.clear( ) ;
          split( v2 , v1[i] , '.' ) ; 
          this->p.bit = atoi( &( v2[1].c_str( )[0] ) ) ;
        }
        this->Client->ReadArea( 
          this->p.area ,
          0 ,
          this->p.start ,
          this->p.length ,
          S7WLByte ,
          this->p.raw_readed
          ) ;
// above read first
// below start to write
        long long value = atoi( &( v3[1].c_str( )[0] ) ) ;
        v2.clear( ) ;
        split( v2 , v3[0] , '.' ) ; 
// v1 : [ "set:ip" , "qx0.0:1" , ... ]
// v2 : [ "qx0" , "0" ] - ( mem in py )
// v3 : [ "qx0.0" , "1" ]
        if( ( v1[i].c_str( )[0] == 'm' || v1[i].c_str( )[0] == 'M' ) ) {
          this->p.area = 131 ;
        }
        if( ( v1[i].c_str( )[0] == 'q' || v1[i].c_str( )[0] == 'Q' ) ) {
          this->p.area = 130 ;
        }
        if( ( v1[i].c_str( )[1] == 'x' || v1[i].c_str( )[1] == 'X' ) ) {
          this->p.length = 1 ;
          this->p.out_type = T_BOOL ;
          this->p.start = atoi( &( v2[0].c_str( )[2] ) ) ;
          this->p.bit = atoi( &( v2[1].c_str( )[0] ) ) ;
          if( 
            !(
              ( 1 << this->p.bit ) & ( this->p.raw_readed[0] ) == // current value
              ( value ) // target value
              )
            ) {
            if(
              ( 1 << this->p.bit ) & ( this->p.raw_readed[0] ) 
              ) {
              this->p.raw_readed[0] += value ;
            }
            else{ 
              this->p.raw_readed[0] -= value ;
            }
          }
        }
        if( ( v1[i].c_str( )[1] == 'd' || v1[i].c_str( )[1] == 'D' ) ) {
          this->p.length = 4 ;
          this->p.out_type = T_DWORD ;
          this->p.start = atoi( &( v2[0].c_str( )[2] ) ) ;
          this->p.raw_readed[0] = 
            ( value & 0xff000000 ) >> 24 ;
          this->p.raw_readed[1] = 
            ( value & 0xff0000 ) >> 16 ;
          this->p.raw_readed[2] = 
            ( value & 0xff00 ) >> 8 ;
          this->p.raw_readed[3] = 
            value & 0xff ;
        }
        this->Client->WriteArea( 
          this->p.area ,
          this->p.db_number ,
          this->p.start ,
          this->p.size ,
          S7WLByte ,
          this->p.raw_readed
          ) ;
      } // end of if 'db' - else
    } // end of for
    this->result += "ok" ;
  } // end of else if 'set' 
  else if (
    ( v1[0].c_str( )[0] == 'e' || v1[0].c_str( )[0] == 'E' ) &&
    ( v1[0].c_str( )[1] == 'x' || v1[0].c_str( )[0] == 'X' ) &&
    ( v1[0].c_str( )[2] == 'i' || v1[0].c_str( )[0] == 'I' ) &&
    ( v1[0].c_str( )[2] == 't' || v1[0].c_str( )[0] == 'T' )  
    ) {
    this->result += "exit" ;
  }
}
