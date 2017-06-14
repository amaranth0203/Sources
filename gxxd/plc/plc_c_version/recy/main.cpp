#include "funcs.h"
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
using namespace std ;

int main(int argc, char *argv[])
{
  payload p ;
  memcpy( p.mem , "DB20.DBB32.0" , 13 ) ;
  getMem( &p ) ;
  exit( -1 ) ;
  int rc ;
  S7Object s7o ;
  Snap7_ops* ops ;
  char buff[128] = "\xd\xe\xa\xd\xb\xe\xe\xf";
  // test
  string str = "1,2,3,4" ;
  // vector<string> vect ;
  // string token ;
  // istringstream ss( str ) ;
  // while( getline( ss , token , ',' ) ) {
  //   vect.push_back( token ) ;
  // }
  // int size = vect.size( ) ;
  // int i ;
  // for( i = 0 ; i < size ; i ++ ) {
  //   printf( "%s\n" , vect[i].c_str( ) ) ;
  // }
  // test end
  ops = ( Snap7_ops* )malloc( sizeof( Snap7_ops ) ) ;
  init_ops( DLL_NAME , ops ) ;
  s7o = ops->Cli_Create( ) ;
  /* do something here */
  try {
    rc = ops->Cli_ConnectTo( s7o , S7_ADDR , LOCAL_TSAP , REMOTE_SAP ) ;
    if( rc != 0 ) {
      printf( "[-] ConnectTo failed on 0x%08x\n" , rc ) ;
      exit( -1 ) ; 
    }
    rc = ops->Cli_DBRead( s7o , 20 , 32 , 1 , buff ) ;
    if( rc != 0 ) {
      printf( "[-] DBRead failed on 0x%08x\n" , rc ) ;
      exit( -1 ) ; 
    }
    printf( "readed : [%x %x %x %x %x %x %x %x]\n" , buff[0] , buff[1] , buff[2] , buff[3] , buff[4] , buff[5] , buff[6] , buff[7] ) ;
    char ch_arr[8] = "\x00\x00" ;
    rc = ops->Cli_DBWrite( s7o , 20 , 30 , 2 , ch_arr ) ;
    rc = ops->Cli_Disconnect( s7o ) ;
    if( rc != 0 ) {
      printf( "[-] Disconnect failed on 0x%08x\n" , rc ) ;
      exit( -1 ) ; 
    }
  }
  catch( ... ) {
  }
  /* done something */
  ops->Cli_Destroy( s7o ) ;
  free( ops ) ;
  return 0;
}

