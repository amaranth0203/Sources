#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>

#include "funcs.h"

#define INVALID_HANDLE_VALUE (HANDLE)-1
#define MAX_PATH2 4096
#define kmalloc(_s) ExAllocatePoolWithTag( NonPagedPool , _s , 'SYSQ' )
#define kfree(_p) ExFreePool( _p )

NTKERNELAPI NTSTATUS ZwQueryDirectoryFile (
  HANDLE FileHandle ,
  HANDLE Event ,
  PIO_APC_ROUTINE ApcRoutine ,
  PVOID ApcContext ,
  PIO_STATUS_BLOCK IoStatusBlock ,
  PVOID FileInformation ,
  ULONG Length ,
  FILE_INFORMATION_CLASS FileInformationClass ,
  BOOLEAN ReturnSingleEntry ,
  PUNICODE_STRING FileName ,
  BOOLEAN RestartScan
  ) ;

typedef struct _FILE_BOTH_DIR_INFORMATION {
  ULONG NextEntryOffset ;
  ULONG FileIndex ;
  LARGE_INTEGER CreationTime ;
  LARGE_INTEGER LastAccessTime ;
  LARGE_INTEGER LastWriteTime ;
  LARGE_INTEGER ChangeTime ;
  LARGE_INTEGER EndOfFile ;
  LARGE_INTEGER AllocationSize ;
  ULONG FileAttributes ;
  ULONG FileNameLength ;
  ULONG EaSize ;
  CCHAR ShortNameLength ;
  WCHAR ShortName[12] ;
  WCHAR FileName[1] ;
} FILE_BOTH_DIR_INFORMATION , *PFILE_BOTH_DIR_INFORMATION ;

HANDLE MyFindFirstFile( LPSTR lpDirectory, PFILE_BOTH_DIR_INFORMATION pDir, ULONG uLength ) {
  char strFolder[MAX_PATH2] = {0} ;
  STRING astrFolder ;
  UNICODE_STRING ustrFolder ;
  OBJECT_ATTRIBUTES oa ;
  IO_STATUS_BLOCK ioStatus ;
  NTSTATUS ntStatus ;
  HANDLE hFind = INVALID_HANDLE_VALUE ;
  memset( strFolder , 0 , MAX_PATH2 ) ;
  strcpy( strFolder , "\\??\\" ) ;
  strcat( strFolder , lpDirectory ) ;
  RtlInitString( &astrFolder , strFolder ) ;
  if( 0 == RtlAnsiStringToUnicodeString( &ustrFolder , &astrFolder , TRUE ) ) {
    InitializeObjectAttributes( &oa , &ustrFolder , OBJ_CASE_INSENSITIVE , NULL , NULL ) ;
    ntStatus = IoCreateFile(
      &hFind ,
      FILE_LIST_DIRECTORY | SYNCHRONIZE | FILE_ANY_ACCESS ,
      &oa ,
      &ioStatus ,
      NULL ,
      FILE_ATTRIBUTE_NORMAL ,
      FILE_SHARE_READ | FILE_SHARE_WRITE ,
      FILE_OPEN ,
      FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT ,
      NULL ,
      0 ,
      CreateFileTypeNone ,
      NULL ,
      IO_NO_PARAMETER_CHECKING ) ;
    RtlFreeUnicodeString( &ustrFolder ) ;
    if( 0 == ntStatus && INVALID_HANDLE_VALUE != hFind ) {
      ntStatus = ZwQueryDirectoryFile(
        hFind ,
        NULL ,
        NULL ,
        NULL ,
        &ioStatus ,
        pDir ,
        uLength ,
        FileBothDirectoryInformation ,
        TRUE ,
        NULL ,
        FALSE
        ) ;
      if( 0 != ntStatus ) {
        ZwClose( hFind ) ;
        hFind = INVALID_HANDLE_VALUE ;
      } // end of if( 0 != ntStatus )
    } // end of if( 0 == ntStatus && INVALID_HANDLE_VALUE != hFind )
  } // end of if( 0 == RtlAnsiStringToUnicodeString( &ustrFolder , &astrFolder , TRUE ) )
  return hFind ;
}

BOOLEAN MyFindNextFile( HANDLE hFind , PFILE_BOTH_DIR_INFORMATION pDir , ULONG uLength ) {
  IO_STATUS_BLOCK ioStatus ;
  NTSTATUS ntStatus ;
  ntStatus = ZwQueryDirectoryFile(
    hFind ,
    NULL ,
    NULL ,
    NULL ,
    &ioStatus ,
    pDir ,
    uLength ,
    FileBothDirectoryInformation ,
    FALSE ,
    NULL ,
    FALSE
    ) ;
  if( 0 == ntStatus ) {
    return TRUE ;
  } // end of if( 0 == ntStatus )
  else {
    return FALSE ;
  } // end of else 
}

ULONG SearchDirectory( LPSTR lpPath ) {
  ULONG muFileCount = 0 ;
  HANDLE hFind = INVALID_HANDLE_VALUE ;
  PFILE_BOTH_DIR_INFORMATION pDir ;
  char *strBuffer = NULL , *lpTmp = NULL ;
  char strFileName[ 255*2 ] ;
  ULONG uLength = MAX_PATH2 * 2 + sizeof( FILE_BOTH_DIR_INFORMATION ) ;
  strBuffer = ( PCHAR )kmalloc( uLength ) ;
  pDir = ( PFILE_BOTH_DIR_INFORMATION )strBuffer ;
  hFind = MyFindFirstFile( lpPath , pDir , uLength ) ;
  if( INVALID_HANDLE_VALUE != hFind ) {
    kfree( strBuffer ) ;
    uLength = ( MAX_PATH2 * 2 + sizeof( FILE_BOTH_DIR_INFORMATION ) ) * 0x2000 ;
    strBuffer = ( PCHAR )kmalloc( uLength ) ;
    pDir = ( PFILE_BOTH_DIR_INFORMATION )strBuffer ;
    if( MyFindNextFile( hFind , pDir , uLength ) ) {
      while( TRUE ) {
        memset( strFileName , 0 , 255 * 2 ) ;
        memcpy( strFileName , pDir->FileName , pDir->FileNameLength ) ;
        if( 0 != strcmp( strFileName , ".." ) && 0 != strcmp( strFileName , "." ) ) {
          if( pDir->FileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
            DbgPrint( "[д©б╪]%S\n" , strFileName ) ;
          } // end of if( pDir->FileAttributes & FILE_ATTRIBUTE_DIRECTORY )
          else {
            DbgPrint( "[нд╪Ч]%S\n" , strFileName ) ;
          } // end of else 
          muFileCount ++ ;
        } // end of if( 0 != strcmp( strFileName , ".." ) && 0 != strcmp( strFileName , "." ) )
        if( 0 == pDir->NextEntryOffset ) break ;
        pDir = ( PFILE_BOTH_DIR_INFORMATION )( ( char* )pDir + pDir->NextEntryOffset ) ;
      } // end of while( TRUE )
      kfree( strBuffer ) ;
    } // end of if( MyFindNextFile( hFind , pDir , uLength ) )
    ZwClose( hFind ) ;
  } // end of if( INVALID_HANDLE_VALUE != hFind )
  return muFileCount ;
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
