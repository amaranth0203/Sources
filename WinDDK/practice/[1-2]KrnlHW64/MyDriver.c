#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>

#define DEVICE_NAME 		L"\\Device\\KrnlHW64"
#define LINK_NAME 			L"\\DosDevices\\KrnlHW64"
#define LINK_GLOBAL_NAME 	L"\\DosDevices\\Global\\KrnlHW64"

#define IOCTL_IO_TEST		CTL_CODE( FILE_DEVICE_UNKNOWN , 0x800 , METHOD_BUFFERED , FILE_ANY_ACCESS )
#define IOCTL_SAY_HELLO		CTL_CODE( FILE_DEVICE_UNKNOWN , 0x801 , METHOD_BUFFERED , FILE_ANY_ACCESS )

VOID DriverUnload( PDRIVER_OBJECT pDriverObj ) {
  UNICODE_STRING strLink ;
  DbgPrint( "[KrnlHW64]DriverUnload\n" ) ;
  RtlInitUnicodeString( &strLink , LINK_NAME ) ;
  IoDeleteSymbolicLink( &strLink ) ;
  IoDeleteDevice( pDriverObj->DeviceObject ) ;
}

NTSTATUS DispatchCreate( PDEVICE_OBJECT pDevObj , PIRP pIrp ) {
  DbgPrint( "[KrnlHW64]DispatchCreate\n" ) ;
  pIrp->IoStatus.Status = STATUS_SUCCESS ;
  pIrp->IoStatus.Information = 0 ;
  IoCompleteRequest( pIrp , IO_NO_INCREMENT ) ;
  return STATUS_SUCCESS ;
}

NTSTATUS DispatchClose( PDEVICE_OBJECT pDevObj , PIRP pIrp ) {
  DbgPrint( "[KrnlHW64]DispatchClose\n" ) ;
  pIrp->IoStatus.Status = STATUS_SUCCESS ;
  pIrp->IoStatus.Information = 0 ;
  IoCompleteRequest( pIrp , IO_NO_INCREMENT ) ;
  return STATUS_SUCCESS ;
}

NTSTATUS DispatchIoctl( PDEVICE_OBJECT pDevObj , PIRP pIrp ) {
  NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST ;
  PIO_STACK_LOCATION pIrpStack ;
  ULONG uIoControlCode ;
  PVOID pIoBuffer ;
  ULONG uInSize ;
  ULONG uOutSize ;
  DbgPrint( "[KrnlHW64]DispatchIoctl\n" ) ;
  pIrpStack = IoGetCurrentIrpStackLocation( pIrp ) ;
  uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode ;
  pIoBuffer = pIrp->AssociatedIrp.SystemBuffer ;
  uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength ;
  uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength ;
  switch( uIoControlCode ) {
  case IOCTL_IO_TEST : {
    DWORD dw = 0 ;
    memcpy( &dw , pIoBuffer , sizeof( DWORD ) ) ;
    dw ++ ;
    memcpy( pIoBuffer , &dw , sizeof( DWORD ) ) ;
    status = STATUS_SUCCESS ;
    break ;
  } // end of case IOCTL_IO_TEST
  case IOCTL_SAY_HELLO : {
    DbgPrint( "[KrnlHW64]IOCTL_SAY_HELLO___________________\n" ) ;
    status = STATUS_SUCCESS ;
    break ;
  } // end of case IOCTL_SAY_HELLO
  } // end of switch( uIoControlCode )
  if( status == STATUS_SUCCESS )
    pIrp->IoStatus.Information = uOutSize ;
  else
    pIrp->IoStatus.Information = 0 ;
  pIrp->IoStatus.Status = status ;
  IoCompleteRequest( pIrp , IO_NO_INCREMENT ) ;
  return status ;
}

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
  return STATUS_SUCCESS ;
}
