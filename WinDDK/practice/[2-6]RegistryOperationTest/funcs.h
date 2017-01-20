#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>

#define DEVICE_NAME 		L"\\Device\\_driver_name_"
#define LINK_NAME 			L"\\DosDevices\\_driver_name_"
#define LINK_GLOBAL_NAME 	L"\\DosDevices\\Global\\_driver_name_"

#define IOCTL_IO_TEST		CTL_CODE( FILE_DEVICE_UNKNOWN , 0x800 , METHOD_BUFFERED , FILE_ANY_ACCESS )
#define IOCTL_SAY_HELLO		CTL_CODE( FILE_DEVICE_UNKNOWN , 0x801 , METHOD_BUFFERED , FILE_ANY_ACCESS )

VOID DriverUnload( PDRIVER_OBJECT pDriverObj ) ;
NTSTATUS DispatchCreate( PDEVICE_OBJECT pDevObj , PIRP pIrp ) ;
NTSTATUS DispatchClose( PDEVICE_OBJECT pDevObj , PIRP pIrp ) ;
NTSTATUS DispatchIoctl( PDEVICE_OBJECT pDevObj , PIRP pIrp ) ;

typedef NTSTATUS ( __fastcall *ZWRENAMEKEY )( HANDLE KeyHandle, PUNICODE_STRING NewName ) ;
PVOID GetFunctionAddr( PCWSTR FunctionName ) ;
VOID EnumSubKeyTest( ) ;
VOID EnumSubValueTest( ) ;
VOID RegCreateKey( LPWSTR KeyName ) ;
VOID RegRenameKey( LPWSTR OldKeyName , LPWSTR NewKeyName ) ;
VOID RegSetValueKey( LPWSTR KeyName , LPWSTR ValueName , DWORD DataType , PVOID DataBuffer , DWORD DataLength ) ;
NTSTATUS RegQueryValueKey( LPWSTR KeyName , LPWSTR ValueName , PKEY_VALUE_PARTIAL_INFORMATION *pkvpi ) ;
VOID RegDeleteValueKey( LPWSTR KeyName , LPWSTR ValueName ) ;
VOID RegDeleteKey( LPWSTR KeyName ) ;

