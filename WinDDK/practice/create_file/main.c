#include "funcs.h"

KEVENT kEvent ;
//线程函数
VOID MyThreadFunc(IN PVOID context)
{
  PUNICODE_STRING str = (PUNICODE_STRING)context;
  while( 1 ) {
    MySleep( 3000 ) ;
    ZwCreateFolder( "\\??\\C:\\test____" ) ;
  }
  KeSetEvent(&kEvent, 0, TRUE);
  PsTerminateSystemThread(STATUS_SUCCESS);
}

NTSTATUS DriverEntry( PDRIVER_OBJECT pDriverObj , PUNICODE_STRING pRegistryString ) {
  NTSTATUS status = STATUS_SUCCESS ;
  UNICODE_STRING ustrLinkName ;
  UNICODE_STRING ustrDevName ;
  PDEVICE_OBJECT pDevObj ;
  HANDLE hThread ;
  UNICODE_STRING ustrTest = RTL_CONSTANT_STRING(L"This is a string for test!");
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
  // 初始化事件
  KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);
  status = PsCreateSystemThread(&hThread, 0, NULL, NULL, NULL, MyThreadFunc,
                                (PVOID)&ustrTest);
  if (!NT_SUCCESS(status))
  {
    DbgPrint("PsCreateSystemThread failed!");
    return STATUS_SUCCESS ;
  }
  ZwClose(hThread);
  // 等待事件
  /* KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL); */
  DbgPrint("CreateThreadTest OVER!\n");
  return STATUS_SUCCESS ;
}
