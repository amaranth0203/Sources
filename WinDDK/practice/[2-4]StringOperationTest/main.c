#include <ntddk.h>

VOID StringInitTest( ) {
  ANSI_STRING AnsiString1 ;
  CHAR* string1 = "hello" ;
  UNICODE_STRING UnicodeString1 = {0} ;
  WCHAR* wideString = L"hellooo" ;
  RtlInitAnsiString( &AnsiString1 , string1 ) ;
  DbgPrint( "AnsiString1:%Z\n" , &AnsiString1 ) ;
  string1[0]='H' ;
  string1[1]='E' ;
  string1[2]='L' ;
  string1[3]='L' ;
  string1[4]='O' ;
  DbgPrint( "AnsiString1:%Z\n" , &AnsiString1 ) ;
  DbgPrint( "%p - AnsiString1.Buffer\n%p - string1\n" , AnsiString1.Buffer , string1 ) ;
#define BUFFER_SIZE 1024
  UnicodeString1.MaximumLength = BUFFER_SIZE ;
  UnicodeString1.Buffer = ( PWSTR )ExAllocatePool( PagedPool , BUFFER_SIZE ) ;
  UnicodeString1.Length = 2*wcslen( wideString ) ;
  ASSERT( UnicodeString1.MaximumLength >= UnicodeString1.Length ) ;
  RtlCopyMemory( UnicodeString1.Buffer , wideString , UnicodeString1.Length ) ;
  DbgPrint( "UnicodeString:%wZ\n" , &UnicodeString1 ) ;
  ExFreePool( UnicodeString1.Buffer ) ;
  UnicodeString1.Buffer = NULL ;
  UnicodeString1.Length = UnicodeString1.MaximumLength = 0 ;
  return ;
}

VOID StringCopyTets( ) {
  UNICODE_STRING UnicodeString1 ;
  UNICODE_STRING UnicodeString2 = {0} ;
  RtlInitUnicodeString( &UnicodeString1 , L"Hello World_" ) ;
  UnicodeString2.Buffer = ( PWSTR )ExAllocatePool( PagedPool , BUFFER_SIZE ) ;
  UnicodeString2.MaximumLength = BUFFER_SIZE ;
  RtlCopyUnicodeString( &UnicodeString2 , &UnicodeString1 ) ;
  DbgPrint( "UnicodeString1:%wZ\n" , &UnicodeString1 ) ;
  DbgPrint( "UnicodeString2:%wZ\n" , &UnicodeString2 ) ;
  RtlFreeUnicodeString( &UnicodeString2 ) ;
}

VOID StringCompareTest( ) {
  UNICODE_STRING UnicodeString1 ;
  UNICODE_STRING UnicodeString2 ;
  RtlInitUnicodeString( &UnicodeString1 , L"Hello World" ) ;
  RtlInitUnicodeString( &UnicodeString2 , L"Hello" ) ;
  if( RtlEqualUnicodeString( &UnicodeString1 , &UnicodeString2 , TRUE ) ) {
    DbgPrint( "UnicodeString1 and UnicodeString2 are equal\n" ) ;
  }
  else {
    DbgPrint( "UnicodeString1 and UnicodeString2 are NOT equal\n" ) ;
  }
}

VOID  StringToUpperTest( ) {
  UNICODE_STRING UnicodeString1 ;
  UNICODE_STRING UnicodeString2 ;
  RtlInitUnicodeString( &UnicodeString1 , L"Hello World" ) ;
  DbgPrint( "UnicodeString1:%wZ\n" , &UnicodeString1 ) ;
  RtlUpcaseUnicodeString( &UnicodeString2 , &UnicodeString1 , TRUE ) ;
  DbgPrint( "UnicodeString2:%wZ\n" , &UnicodeString2 ) ;
  RtlFreeUnicodeString( &UnicodeString2 ) ;
}

VOID StringToIntegerTest( ) {
  UNICODE_STRING UnicodeString1 ;
  UNICODE_STRING UnicodeString2 = {0} ;
  ULONG lNumber ;
  NTSTATUS nStatus ;
  RtlInitUnicodeString( &UnicodeString1 , L"-100" ) ;
  nStatus = RtlUnicodeStringToInteger( &UnicodeString1 , 10 , &lNumber ) ;
  if( NT_SUCCESS( nStatus ) ) {
    DbgPrint( "Convert to integer successfully!\n" ) ;
    DbgPrint( "Result:%d\n" , lNumber ) ;
  }
  else {
    DbgPrint( "Convert to integer unsuccessfully!\n" ) ;
  }
  UnicodeString2.Buffer = ( PWSTR )ExAllocatePool( PagedPool , BUFFER_SIZE ) ;
  UnicodeString2.MaximumLength = BUFFER_SIZE ;
  nStatus = RtlIntegerToUnicodeString( 200 , 10 , &UnicodeString2 ) ;
  if( NT_SUCCESS( nStatus ) ) {
    DbgPrint( "Convert to string successfully!\n" ) ;
    DbgPrint( "Result:%wZ\n" , &UnicodeString2 ) ;
  }
  else {
    DbgPrint( "Convert to string unsuccessfully!\n" ) ;
  }
  RtlFreeUnicodeString( &UnicodeString2 ) ;
}

VOID StringConvertTest( ) {
  UNICODE_STRING UnicodeString1 ;
  UNICODE_STRING UnicodeString2 ;
  ANSI_STRING AnsiString1 ;
  ANSI_STRING AnsiString2 ;
  NTSTATUS nStatus ;
  RtlInitUnicodeString( &UnicodeString1 , L"Hello World" ) ;
  nStatus = RtlUnicodeStringToAnsiString( &AnsiString1 , &UnicodeString1 , TRUE ) ;
  if( NT_SUCCESS( nStatus ) ) {
    DbgPrint( "Convert successfully!\n" ) ;
    DbgPrint( "Result:%Z\n" , &AnsiString1 ) ;
  }
  else {
    DbgPrint( "Convert unsuccessfully!\n" ) ;
  }
  RtlFreeAnsiString( &AnsiString1 ) ;
  RtlInitString( &AnsiString2 , "Hello World___" ) ;
  nStatus = RtlAnsiStringToUnicodeString( &UnicodeString2 , &AnsiString2 , TRUE ) ;
  if( NT_SUCCESS( nStatus ) ) {
    DbgPrint( "Convert successfully!\n" ) ;
    DbgPrint( "Result:%wZ\n" , &UnicodeString2 ) ;
  }
  else {
    DbgPrint( "Convert unsuccessfully!\n" ) ;
  }  
  RtlFreeUnicodeString( &UnicodeString2 ) ;
}

VOID DriverUnload( PDRIVER_OBJECT pDriverObj ) {
  DbgPrint( "[2_4]DriverUnload\n" ) ;
  return ;
}

NTSTATUS DriverEntry( IN PDRIVER_OBJECT pDriverObj , IN PUNICODE_STRING pRegistryString ) {
  pDriverObj->DriverUnload	=	DriverUnload ;
  DbgPrint( "[2_4]DriverEntry\n" ) ;
  DbgPrint( "[2_4]--------------------\n" ) ;
  StringInitTest( ) ;
  DbgPrint( "[2_4]--------------------\n" ) ;
  StringCopyTets( ) ;
  DbgPrint( "[2_4]--------------------\n" ) ;
  StringCompareTest( ) ;
  DbgPrint( "[2_4]--------------------\n" ) ;
  StringToUpperTest( ) ;
  DbgPrint( "[2_4]--------------------\n" ) ;
  StringToIntegerTest( ) ;
  DbgPrint( "[2_4]--------------------\n" ) ;
  StringConvertTest( ) ;
  DbgPrint( "[2_4]--------------------\n" ) ;
  return STATUS_SUCCESS ;
}
