#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>

#include "funcs.h"

NTSTATUS DriverEntry( PDRIVER_OBJECT pDriverObj , PUNICODE_STRING pRegistryString ) {
  NTSTATUS status = STATUS_SUCCESS ;
  UNICODE_STRING ustrLinkName ;
  UNICODE_STRING ustrDevName ;
  PDEVICE_OBJECT pDevObj ;
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
  EnumProcess( ) ;
  return STATUS_SUCCESS ;
}
