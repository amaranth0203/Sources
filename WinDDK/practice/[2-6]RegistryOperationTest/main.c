#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>

#include "funcs.h"

NTSTATUS DriverEntry( PDRIVER_OBJECT pDriverObj , PUNICODE_STRING pRegistryString ) {
  NTSTATUS status = STATUS_SUCCESS ;
  UNICODE_STRING ustrLinkName ;
  UNICODE_STRING ustrDevName ;
  PDEVICE_OBJECT pDevObj ;
  DWORD dw = 1235 , dw0 = 0 ;
  PKEY_VALUE_PARTIAL_INFORMATION pkvi ;
  pDriverObj->MajorFunction[IRP_MJ_CREATE]			=	DispatchCreate ;
  pDriverObj->MajorFunction[IRP_MJ_CLOSE]			=	DispatchClose ;
  pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL]	=	DispatchIoctl ;
  pDriverObj->DriverUnload							=	DriverUnload ;
  RtlInitUnicodeString( &ustrDevName , DEVICE_NAME ) ;
  status = IoCreateDevice( pDriverObj , 0 , &ustrDevName , FILE_DEVICE_UNKNOWN , 0 , FALSE , &pDevObj ) ;
  if( !NT_SUCCESS( status ) ) return status ;
  if( IoIsWdmVersionAvailable( 1 , 0x10 ) )
    RtlInitUnicodeString( &ustrLinkName , LINK_GLOBAL_NAME ) ;
  else
    RtlInitUnicodeString( &ustrLinkName , LINK_NAME ) ;
  status = IoCreateSymbolicLink( &ustrLinkName , &ustrDevName ) ;
  if( !NT_SUCCESS( status ) ) {
    IoDeleteDevice( pDevObj ) ;
    return status ;
  }
  DbgPrint( "[KrnlHW64]DriverEntry\n" ) ;

  DbgPrint( "Ã¶¾Ù×Ó KEY ²âÊÔ\n" ) ;
  EnumSubKeyTest( ) ;
  DbgPrint( "\n" ) ;

  DbgPrint( "Ã¶¾Ù×Ó VALUE ²âÊÔ\n" ) ;
  EnumSubValueTest( ) ;
  DbgPrint( "\n" ) ;

  DbgPrint( "´´½¨ KEY ²âÊÔ\n" ) ;
  RegCreateKey( L"\\Registry\\Machine\\Software\\0000" ) ;
  DbgPrint( "\n" ) ;

  DbgPrint( "ÖØÃüÃû KEY ²âÊÔ\n" ) ;
  RegRenameKey( L"\\Registry\\Machine\\Software\\0000" , L"1111" ) ;
  DbgPrint( "\n" ) ;

  DbgPrint( "ÐÂ½¨/ÉèÖÃ VALUE ²âÊÔ\n" ) ;
  RegSetValueKey( L"\\Registry\\Machine\\Software\\1111" , L"dww" , REG_DWORD , &dw , sizeof( dw ) ) ;
  DbgPrint( "\n" ) ;

  DbgPrint( "²éÑ¯ VALUE ²âÊÔ\n" ) ;
  RegQueryValueKey( L"\\Registry\\Machine\\Software\\1111" , L"dww" , &pkvi ) ;
  memcpy( &dw0 , pkvi->Data , pkvi->DataLength ) ;
  DbgPrint( "dw0 : %ld\n" , dw0 ) ;
  ExFreePool( pkvi ) ;
  DbgPrint( "\n" ) ;

  DbgPrint( "É¾³ý VALUE ²âÊÔ\n" ) ;
  RegDeleteValueKey( L"\\Registry\\Machine\\Software\\1111" , L"dww" ) ;
  DbgPrint( "\n" ) ;

  DbgPrint( "É¾³ý KYE ²âÊÔ\n" ) ;
  RegDeleteKey( L"\\Registry\\Machine\\Software\\1111" ) ;
  DbgPrint( "\n" ) ;

  return STATUS_SUCCESS ;
}
