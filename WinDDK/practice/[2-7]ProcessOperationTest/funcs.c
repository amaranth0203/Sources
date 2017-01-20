#include <ntddk.h>
#include <windef.h>

#include "funcs.h"

VOID EnumProcess( ) {
  ULONG i = 0 ;
  PEPROCESS eproc = NULL ;
  /* for( i = 4 ; i < 1<<18 ; i = i + 4 ) { */
  for( i = 4 ; i < 1<<9 ; i = i + 4 ) {
    eproc = LookupProcess( ( HANDLE ) i ) ;
    if( NULL != eproc ) {
      ObDereferenceObject( eproc ) ;
      DbgPrint( 
        "EPROCESS=%p PID=%ld PPID=%ld Name=%s\n" ,
        eproc ,
        ( DWORD )PsGetProcessId( eproc ) ,
        ( DWORD )PsGetProcessInheritedFromUniqueProcessId( eproc ) ,
        PsGetProcessImageFileName( eproc )
        ) ;
      EnumThread( eproc ) ;
      EnumModule( eproc ) ;
      DbgPrint( "\n" ) ;
    } // end of if( NULL != eproc )
  }
}

VOID EnumModule( PEPROCESS Process ) {
  SIZE_T Peb = 0 ;
  SIZE_T Ldr = 0 ;
  PLIST_ENTRY ModListHead = 0 ;
  PLIST_ENTRY Module = 0 ;
  ANSI_STRING AnsiString ;
  KAPC_STATE ks ;
  if( !MmIsAddressValid( Process ) ) 
    return ;
  Peb = ( SIZE_T )PsGetProcessPeb( Process ) ;
  if( !Peb )
    return ;
  KeStackAttachProcess( Process , &ks ) ;
  __try {
    Ldr = Peb + ( SIZE_T )LdrInPebOffset ;
    ProbeForRead( ( CONST PVOID )Ldr , 8 , 8 ) ;
    ModListHead = ( PLIST_ENTRY )( *( PULONG64 )Ldr + ModListInPebOffset ) ;
    ProbeForRead( ( CONST PVOID )ModListHead , 8 , 8 ) ;
    Module = ModListHead->Flink ;
    while( ModListHead != Module ) {
      DbgPrint( 
        "[MODULE]Base=%p Size=%ld Path=%wZ\n" ,
        ( PVOID )( ( ( PLDR_DATA_TABLE_ENTRY )Module )->DllBase ) ,
        ( ULONG )( ( ( PLDR_DATA_TABLE_ENTRY )Module )->SizeOfImage ) ,
        &( ( (PLDR_DATA_TABLE_ENTRY )Module )->FullDllName )
        ) ;
      Module = Module->Flink ;
      ProbeForRead( ( CONST PVOID )Module , 80 , 8 ) ;
    } // end of while( ModListHead != Module )
  }
  __except( EXCEPTION_EXECUTE_HANDLER ) {
    DbgPrint( "[EnumModule]__except (EXCEPTION_EXECUTE_HANDLER)" ) ;
  }
  KeUnstackDetachProcess( &ks ) ;
}

VOID EnumThread( PEPROCESS Process ) {
  ULONG i = 0 , c = 0 ;
  PETHREAD ethrd = NULL ;
  PEPROCESS eproc = NULL ;
  for( i = 4 ; i < 1<<18 ; i = i + 4 ) {
    ethrd = LookupThread( ( HANDLE ) i ) ;
    if( NULL != ethrd ) {
      eproc = IoThreadToProcess( ethrd ) ;
      if( eproc == Process ) {
        DbgPrint( 
          "[THREAD]ETHREAD=%p Tid=%ld\n" ,
          ethrd ,
          ( ULONG )PsGetThreadId( ethrd )
          ) ;
      } // end of if( eproc == Process )
      ObDereferenceObject( ethrd ) ;
    } // end of if( NULL != ethrd )
  }
}

PETHREAD LookupThread( HANDLE Tid ) {
  PETHREAD ethread ;
  if( NT_SUCCESS( PsLookupThreadByThreadId( Tid , &ethread ) ) ) {
    return ethread ;
  } // end of if( NT_SUCCESS( PsLookupThreadByThreadId( Tid , &ethread ) ) )
  else {
    return NULL ;
  }
}

PEPROCESS LookupProcess( HANDLE Pid ) {
  PEPROCESS eprocess = NULL ;
  if( NT_SUCCESS( PsLookupProcessByProcessId( Pid , &eprocess ) ) ) {
    return eprocess ;
  }
  else {
    return NULL ;
  }
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
