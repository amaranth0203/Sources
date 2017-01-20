#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>

#include "funcs.h"

ZWRENAMEKEY MyZwRenameKey = NULL ;

VOID RegDeleteKey( LPWSTR KeyName ) {
  OBJECT_ATTRIBUTES objectAttributes ;
  UNICODE_STRING usKeyName ;
  NTSTATUS ntStatus ;
  HANDLE hRegister ;
  RtlInitUnicodeString( &usKeyName , KeyName ) ;
  InitializeObjectAttributes(
    &objectAttributes ,
    &usKeyName ,
    OBJ_CASE_INSENSITIVE ,
    NULL ,
    NULL
    ) ;
  ntStatus = ZwOpenKey( &hRegister , KEY_ALL_ACCESS , &objectAttributes ) ;
  if( NT_SUCCESS( ntStatus ) ) {
    DbgPrint( "ZwOpenKey success!\n" ) ;
    ntStatus = ZwDeleteKey( hRegister ) ;
    if( NT_SUCCESS( ntStatus ) ) {
      ZwFlushKey( hRegister ) ;
      DbgPrint( "ZwDeleteKey success!\n" ) ;
    }
    else {
      DbgPrint( "ZwDeleteKey failed\n" ) ;
    }
    ZwClose( hRegister ) ;
  } // end of if( NT_SUCCESS( ntStatus ) )
  else {
    DbgPrint( "ZwOpenKey failed\n" ) ;
  }  
}

VOID RegDeleteValueKey( LPWSTR KeyName , LPWSTR ValueName ) {
  OBJECT_ATTRIBUTES objectAttributes ;
  UNICODE_STRING usKeyName , usValueName ;
  NTSTATUS ntStatus ;
  HANDLE hRegister ;
  RtlInitUnicodeString( &usKeyName , KeyName ) ;
  RtlInitUnicodeString( &usValueName , ValueName ) ;
  InitializeObjectAttributes(
    &objectAttributes ,
    &usKeyName ,
    OBJ_CASE_INSENSITIVE ,
    NULL ,
    NULL
    ) ;
  ntStatus = ZwOpenKey( &hRegister , KEY_ALL_ACCESS , &objectAttributes ) ;
  if( NT_SUCCESS( ntStatus ) ) {
    DbgPrint( "ZwOpenKey success!\n" ) ;
    ntStatus = ZwDeleteValueKey( hRegister , &usValueName ) ;
    if( NT_SUCCESS( ntStatus ) ) {
      ZwFlushKey( hRegister ) ;
      DbgPrint( "ZwDeleteValueKey success!\n" ) ;
    }
    else {
      DbgPrint( "ZwDeleteValueKey failed\n" ) ;
    }
    ZwClose( hRegister ) ;
  } // end of if( NT_SUCCESS( ntStatus ) )
  else {
    DbgPrint( "ZwOpenKey failed\n" ) ;
  }  
}

NTSTATUS RegQueryValueKey( LPWSTR KeyName , LPWSTR ValueName , PKEY_VALUE_PARTIAL_INFORMATION *pkvpi ) {
  ULONG ulSize ;
  NTSTATUS ntStatus ;
  PKEY_VALUE_PARTIAL_INFORMATION pvpi ;
  OBJECT_ATTRIBUTES objectAttributes ;
  HANDLE hRegister ;
  UNICODE_STRING usKeyName ;
  UNICODE_STRING usValueName ;
  RtlInitUnicodeString( &usKeyName , KeyName ) ;
  RtlInitUnicodeString( &usValueName , ValueName ) ;
  InitializeObjectAttributes(
    &objectAttributes ,
    &usKeyName ,
    OBJ_CASE_INSENSITIVE ,
    NULL ,
    NULL
    ) ;
  ntStatus = ZwOpenKey( &hRegister , KEY_ALL_ACCESS , &objectAttributes ) ;
  if( !NT_SUCCESS( ntStatus ) ) {
    DbgPrint( "[RegQueryValueKey]ZwOpenKey failed\n" ) ;
    return ntStatus ;
  } // end of if( !NT_SUCCESS( ntStatus ) )
  ntStatus = ZwQueryValueKey(
    hRegister ,
    &usValueName ,
    KeyValuePartialInformation ,
    NULL ,
    0 ,
    &ulSize
    ) ;
  if( STATUS_OBJECT_NAME_NOT_FOUND == ntStatus || 0 == ulSize ) {
    DbgPrint( "ZwQueryValueKey 1 failed!\n" ) ;
    return STATUS_UNSUCCESSFUL ;
  } // end of if( STATUS_OBJECT_NAME_NOT_FOUND == ntStatus || 0 == ulSize )
  pvpi = ( PKEY_VALUE_PARTIAL_INFORMATION )ExAllocatePool( PagedPool , ulSize ) ;
  ntStatus = ZwQueryValueKey(
    hRegister ,
    &usValueName ,
    KeyValuePartialInformation ,
    pvpi ,
    ulSize ,
    &ulSize
    ) ;
  if( !NT_SUCCESS( ntStatus ) ) {
    DbgPrint( "ZwQueryValueKey 2 failed!\n" ) ;
    return STATUS_UNSUCCESSFUL ;
  } // end of if( !NT_SUCCESS( ntStatus ) )
  *pkvpi = pvpi ;
  DbgPrint( "ZwQueryValueKey success!\n" ) ;
  return STATUS_SUCCESS ;
}

VOID RegSetValueKey( LPWSTR KeyName , LPWSTR ValueName , DWORD DataType , PVOID DataBuffer , DWORD DataLength ) {
  OBJECT_ATTRIBUTES objectAttributes ;
  UNICODE_STRING usKeyName , usValueName ;
  NTSTATUS ntStatus ;
  HANDLE hRegister ;
  ULONG Type ;
  RtlInitUnicodeString( &usKeyName , KeyName ) ;
  RtlInitUnicodeString( &usValueName , ValueName ) ;
  InitializeObjectAttributes(
    &objectAttributes ,
    &usKeyName ,
    OBJ_CASE_INSENSITIVE ,
    NULL ,
    NULL
    ) ;
  ntStatus = ZwOpenKey( &hRegister , KEY_ALL_ACCESS , &objectAttributes ) ;
  if( NT_SUCCESS( ntStatus ) ) {
    DbgPrint( "ZwOpenKey success!\n" ) ;
    ntStatus = ZwSetValueKey( hRegister , &usValueName , 0 , DataType , DataBuffer , DataLength ) ;
    if( NT_SUCCESS( ntStatus ) ) {
      ZwFlushKey( hRegister ) ;
      DbgPrint( "ZwSetValueKey success!\n" ) ;
    }
    else {
      DbgPrint( "ZwSetValueKey failed\n" ) ;
    }
    ZwClose( hRegister ) ;
  } // end of if( NT_SUCCESS( ntStatus ) )
  else {
    DbgPrint( "ZwOpenKey failed\n" ) ;
  }
}

VOID RegRenameKey( LPWSTR OldKeyName , LPWSTR NewKeyName ) {
  OBJECT_ATTRIBUTES objectAttributes ;
  HANDLE hRegister ;
  NTSTATUS ntStatus ;
  UNICODE_STRING usOldKeyName , usNewKeyName ;
  RtlInitUnicodeString( &usOldKeyName , OldKeyName ) ;
  RtlInitUnicodeString( &usNewKeyName , NewKeyName ) ;

  MyZwRenameKey = ( ZWRENAMEKEY )GetFunctionAddr( L"ZwRenameKey" ) ;
  DbgPrint( "ZwRenameKey : %p\n" , MyZwRenameKey ) ;

  InitializeObjectAttributes(
    &objectAttributes ,
    &usOldKeyName ,
    OBJ_CASE_INSENSITIVE ,
    NULL ,
    NULL
    ) ;
  ntStatus = ZwOpenKey( &hRegister , KEY_ALL_ACCESS , &objectAttributes ) ;
  if( NT_SUCCESS( ntStatus ) ) {
    DbgPrint( "ZwOpenKey success!\n" ) ;
    ntStatus = MyZwRenameKey( hRegister , &usNewKeyName ) ;
    if( NT_SUCCESS( ntStatus ) ) {
      ZwFlushKey( hRegister ) ;
      DbgPrint( "ZwRenameKey success!\n" ) ;
    }
    else {
      DbgPrint( "ZwRenameKey failed\n" ) ;
    }
    ZwClose( hRegister ) ;
  } // end of if( NT_SUCCESS( ntStatus ) )
  else {
    DbgPrint( "ZwOpenKey failed\n" ) ;
  }
}

VOID RegCreateKey( LPWSTR KeyName ) {
  OBJECT_ATTRIBUTES objectAttributes ;
  UNICODE_STRING usKeyName ;
  NTSTATUS ntStatus ;
  HANDLE hRegister ;
  RtlInitUnicodeString( &usKeyName , KeyName ) ;
  InitializeObjectAttributes(
    &objectAttributes ,
    &usKeyName ,
    OBJ_CASE_INSENSITIVE ,
    NULL ,
    NULL
    ) ;
  ntStatus =
    ZwCreateKey(
      &hRegister ,
      KEY_ALL_ACCESS ,
      &objectAttributes ,
      0 ,
      NULL ,
      REG_OPTION_NON_VOLATILE ,
      NULL
      ) ;
  if( NT_SUCCESS( ntStatus ) ) {
    ZwClose( hRegister ) ;
    DbgPrint( "ZwCreateKey success!\n" ) ;
  } // end of if( NT_SUCCESS( hRegister ) )
  else {
    DbgPrint( "ZwCreateKey failed!\n" ) ;
  }
}

VOID EnumSubValueTest( ) {
  WCHAR MY_KEY_NAME[] = L"\\Registry\\Machine\\Software\\Microsoft\\.NETFramework" ;
  UNICODE_STRING RegUnicodeString ;
  HANDLE hRegister ;
  OBJECT_ATTRIBUTES objectAttributes ;
  ULONG ulSize , i ;
  UNICODE_STRING uniKeyName ;
  PKEY_FULL_INFORMATION pfi ;
  NTSTATUS ntStatus ;
  RtlInitUnicodeString( &RegUnicodeString , MY_KEY_NAME ) ;
  InitializeObjectAttributes(
    &objectAttributes ,
    &RegUnicodeString ,
    OBJ_CASE_INSENSITIVE ,
    NULL ,
    NULL
    ) ;
  ntStatus = ZwOpenKey( &hRegister , KEY_ALL_ACCESS , &objectAttributes ) ;
  if( NT_SUCCESS( ntStatus ) ) {
    DbgPrint( "Open register successfully\n" ) ;
  } // end of if( NT_SUCCESS( ntStatus ) )
  ZwQueryKey( hRegister , KeyFullInformation , NULL , 0 , &ulSize ) ;
  pfi = ( PKEY_FULL_INFORMATION )ExAllocatePool( PagedPool , ulSize ) ;
  ZwQueryKey( hRegister , KeyFullInformation , pfi , ulSize , &ulSize ) ;
  for( i = 0 ; i < pfi->Values ; i ++ ) {
    PKEY_VALUE_BASIC_INFORMATION pvbi ;
    ZwEnumerateValueKey( hRegister , i , KeyValueBasicInformation , NULL , 0 , &ulSize ) ;
    pvbi = ( PKEY_VALUE_BASIC_INFORMATION )ExAllocatePool( PagedPool , ulSize ) ;
    ZwEnumerateValueKey( hRegister , i , KeyValueBasicInformation , pvbi , ulSize , &ulSize ) ;
    uniKeyName.Length			=	( USHORT )pvbi->NameLength ;
    uniKeyName.MaximumLength	=	( USHORT )pvbi->NameLength ;
    uniKeyName.Buffer			=	pvbi->Name ;
    DbgPrint( "The %d sub value name : %wZ\n" , i , &uniKeyName ) ;
    if( REG_SZ				==	pvbi->Type ) {
      DbgPrint( "The sub value type : REG_SZ\n" ) ;
    } // end of if( REG_SZ				==	pvbi->Type )
    else if( REG_MULTI_SZ	==	pvbi->Type ) {
      DbgPrint( "The sub value type : REG_MULTI_SZ\n" ) ;
    } // end of if( REG_MULTI_SZ	==	pvbi->Type )
    else if( REG_DWORD		==	pvbi->Type ) {
      DbgPrint( "The sub value type : REG_DWORD\n" ) ;
    } // end of if( REG_DWORD		==	pvbi->Type )
    else if( REG_BINARY		==	pvbi->Type ) {
      DbgPrint( "The sub value type : REG_BINARY\n" ) ;
    } // end of if( REG_BINARY		==	pvbi->Type )
    ExFreePool( pvbi ) ;
  }
  ExFreePool( pfi ) ;
  ZwClose( hRegister ) ;
}

VOID EnumSubKeyTest( ) {
  WCHAR MY_KEY_NAME[] = L"\\Registry\\Machine\\Software" ;
  UNICODE_STRING RegUnicodeString ;
  HANDLE hRegister ;
  OBJECT_ATTRIBUTES objectAttributes ;
  NTSTATUS ntStatus ;
  ULONG ulSize , i ;
  UNICODE_STRING uniKeyName ;
  PKEY_FULL_INFORMATION pfi ;

  RtlInitUnicodeString( &RegUnicodeString , MY_KEY_NAME ) ;
  InitializeObjectAttributes(
    &objectAttributes ,
    &RegUnicodeString ,
    OBJ_CASE_INSENSITIVE ,
    NULL ,
    NULL
    ) ;
  ntStatus = ZwOpenKey(
    &hRegister ,
    KEY_ALL_ACCESS ,
    &objectAttributes
    ) ;
  if( NT_SUCCESS( ntStatus ) ) {
    DbgPrint( "Open register successfully\n" ) ;
  }
  ZwQueryKey( hRegister , KeyFullInformation , NULL , 0 , &ulSize ) ;
  pfi = ( PKEY_FULL_INFORMATION )ExAllocatePool( PagedPool , ulSize ) ;
  ZwQueryKey( hRegister , KeyFullInformation , pfi , ulSize , &ulSize ) ;
  for( i = 0 ; i < pfi->SubKeys ; i ++ ) {
    PKEY_BASIC_INFORMATION pbi ;
    ZwEnumerateKey( hRegister , i , KeyBasicInformation , NULL , 0 , &ulSize ) ;
    pbi = ( PKEY_BASIC_INFORMATION )ExAllocatePool( PagedPool , ulSize ) ;
    ZwEnumerateKey( hRegister , i , KeyBasicInformation , pbi , ulSize , &ulSize ) ;
    uniKeyName.Length = ( USHORT )pbi->NameLength ;
    uniKeyName.MaximumLength = ( USHORT )pbi->NameLength ;
    uniKeyName.Buffer = pbi->Name ;
    DbgPrint( "The %d sub item name : %wZ\n" , i , &uniKeyName ) ;
    ExFreePool( pbi ) ;
  }
  ExFreePool( pfi ) ;
  ZwClose( hRegister ) ;
}

PVOID GetFunctionAddr( PCWSTR FunctionName ) {
  UNICODE_STRING UnicodeFunctionName ;
  RtlInitUnicodeString( &UnicodeFunctionName , FunctionName ) ;
  return MmGetSystemRoutineAddress( &UnicodeFunctionName ) ;
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
