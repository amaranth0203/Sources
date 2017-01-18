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
ULONG SearchDirectory( LPSTR lpPath ) ;
