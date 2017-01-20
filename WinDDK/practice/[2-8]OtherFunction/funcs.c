#include <ntddk.h>
#include <windef.h>

#include "funcs.h"

VOID MySleep( LONG msec ) {
  LARGE_INTEGER my_interval ;
  my_interval.QuadPart = DELAY_ONE_MILLISECOND ;
  my_interval.QuadPart *= msec ;
  KeDelayExecutionThread( KernelMode , 0 , &my_interval ) ;
}

VOID MyThreadFunc( IN PVOID context ) {
  PUNICODE_STRING str = ( PUNICODE_STRING ) context ;
  DbgPrint( "Kernel thread runing : %wZ\n" , str ) ;
  DbgPrint( "Wait 3s!\n" ) ;
  MySleep( 3000 ) ;
  DbgPrint( "Kernel thread exit!\n" ) ;
  KeSetEvent( &kEvent , 0 , TRUE ) ;
  PsTerminateSystemThread( STATUS_SUCCESS ) ;
}

VOID CreateThreadTest( ) {
  HANDLE hThread ;
  UNICODE_STRING ustrTest = RTL_CONSTANT_STRING( L"This is a string for test!" ) ;
  NTSTATUS status ;
  KeInitializeEvent( &kEvent , SynchronizationEvent , FALSE ) ;
  status = PsCreateSystemThread( &hThread , 0 , NULL , NULL , NULL , MyThreadFunc , ( PVOID )&ustrTest ) ;
  if( !NT_SUCCESS( status ) ) {
    DbgPrint( "PsCreateSystemThread failed!\n" ) ;
    return ;
  } // end of if( !NT_SUCCESS( status ) )
  ZwClose( hThread ) ;
  KeWaitForSingleObject( &kEvent , Executive , KernelMode , FALSE , NULL ) ;
  DbgPrint( "CreateThreadTest OVER!\n" ) ;
}

VOID MyGetCurrentTime( ) {
  LARGE_INTEGER CurrentTime ;
  LARGE_INTEGER LocalTime ;
  TIME_FIELDS TimeFiled ;
  KeQuerySystemTime( &CurrentTime ) ;
  ExSystemTimeToLocalTime( &CurrentTime , &LocalTime ) ;
  RtlTimeToTimeFields( &LocalTime , &TimeFiled ) ;
  DbgPrint( 
    "[TimeTest] NowTime : %4d-%2d-%2d %2d:%2d:%2d\n" ,
    TimeFiled.Year ,
    TimeFiled.Month ,
    TimeFiled.Day ,
    TimeFiled.Hour ,
    TimeFiled.Minute ,
    TimeFiled.Second
    ) ;
}

VOID EnumDriver( PDRIVER_OBJECT pDriverObject ) {
  PKLDR_DATA_TABLE_ENTRY entry = ( PKLDR_DATA_TABLE_ENTRY )pDriverObject->DriverSection ;
  PKLDR_DATA_TABLE_ENTRY firstentry ;
  ULONG64 pDrvBase = 0 ;
  KIRQL OldIrql ;
  firstentry = entry ;
  while( firstentry != ( PKLDR_DATA_TABLE_ENTRY )entry->InLoadOrderLinks.Flink ) {
    DbgPrint( "BASE=%p\tPATH=%wZ" , entry->DllBase , entry->FullDllName ) ;
    entry = ( PKLDR_DATA_TABLE_ENTRY )entry->InLoadOrderLinks.Flink ;
  } // end of while( firstentry != ( PKLDR_DATA_TABLE_ENTRY )entry->InLoadOrderLinks.Flink )
}

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
