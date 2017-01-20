#include <ntddk.h>
#include <windef.h>

#define DEVICE_NAME 		L"\\Device\\_driver_name_"
#define LINK_NAME 			L"\\DosDevices\\_driver_name_"
#define LINK_GLOBAL_NAME 	L"\\DosDevices\\Global\\_driver_name_"

#define IOCTL_IO_TEST		CTL_CODE( FILE_DEVICE_UNKNOWN , 0x800 , METHOD_BUFFERED , FILE_ANY_ACCESS )
#define IOCTL_SAY_HELLO		CTL_CODE( FILE_DEVICE_UNKNOWN , 0x801 , METHOD_BUFFERED , FILE_ANY_ACCESS )

VOID DriverUnload( PDRIVER_OBJECT pDriverObj ) ;
NTSTATUS DispatchCreate( PDEVICE_OBJECT pDevObj , PIRP pIrp ) ;
NTSTATUS DispatchClose( PDEVICE_OBJECT pDevObj , PIRP pIrp ) ;
NTSTATUS DispatchIoctl( PDEVICE_OBJECT pDevObj , PIRP pIrp ) ;

typedef struct _KLDR_DATA_TABLE_ENTRY {
  LIST_ENTRY64		InLoadOrderLinks ;
  ULONG64			__Undefined1 ;
  ULONG64			__Undefined2 ;
  ULONG64			__Undefined3 ;
  ULONG64			NonPagedDebugInfo ;
  ULONG64			DllBase ;
  ULONG64			EntryPoint ;
  ULONG				SizeOfImage ;
  UNICODE_STRING	FullDllName ;
  UNICODE_STRING	BaseDllName ;
  ULONG				Flag ;
  USHORT			LoadCount ;
  USHORT			__Undefined5 ;
  ULONG64			__Undefined6 ;
  ULONG				CheckSum ;
  ULONG				__padding1 ;
  ULONG				TimeDataStamp ;
  ULONG				__padding2 ;
} KLDR_DATA_TALBE_ENTRY , *PKLDR_DATA_TABLE_ENTRY ;

#define DELAY_ONE_MICROSECOND (-10)
#define DELAY_ONE_MILLISECOND (DELAY_ONE_MICROSECOND*1000)

static KEVENT kEvent ;

VOID EnumDriver( PDRIVER_OBJECT pDriverObject ) ;
VOID MyGetCurrentTime( ) ;
VOID MySleep( LONG msec ) ;
VOID MyThreadFunc( IN PVOID context ) ;
VOID CreateThreadTest( ) ;

