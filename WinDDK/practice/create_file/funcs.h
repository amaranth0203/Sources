#ifndef _FUNCS_H_
#define _FUNCS_H_

#include <ntifs.h>
#include <windef.h>
#include <ntstrsafe.h>
#include "Loader.h"

#define DEVICE_NAME 		L"\\Device\\_driver_name_"
#define LINK_NAME 			L"\\DosDevices\\_driver_name_"
#define LINK_GLOBAL_NAME 	L"\\DosDevices\\Global\\_driver_name_"

#define IOCTL_IO_TEST		CTL_CODE( FILE_DEVICE_UNKNOWN , 0x800 , METHOD_BUFFERED , FILE_ANY_ACCESS )
#define IOCTL_SAY_HELLO		CTL_CODE( FILE_DEVICE_UNKNOWN , 0x801 , METHOD_BUFFERED , FILE_ANY_ACCESS )

#define DELAY_ONE_MICROSECOND (-10)
#define DELAY_ONE_MILLISECOND (DELAY_ONE_MICROSECOND*1000)

VOID DriverUnload( PDRIVER_OBJECT pDriverObj ) ;
NTSTATUS DispatchCreate( PDEVICE_OBJECT pDevObj , PIRP pIrp ) ;
NTSTATUS DispatchClose( PDEVICE_OBJECT pDevObj , PIRP pIrp ) ;
NTSTATUS DispatchIoctl( PDEVICE_OBJECT pDevObj , PIRP pIrp ) ;
VOID MySleep(LONG msec) ;
VOID CharToWchar(PCHAR src, PWCHAR dst) ;
void ZwCreateFolder( char* FolderPath ) ;


#define DPRINT(format, ...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, format, __VA_ARGS__)
#define BB_POOL_TAG 'enoB'

NTKERNELAPI
PVOID
NTAPI
PsGetProcessWow64Process( IN PEPROCESS Process );

/// <summary>
/// Input for IOCTL_BLACKBONE_SET_PROTECTION
/// </summary>
typedef struct _SET_PROC_PROTECTION
{
  ULONG   pid;            // Process ID
  BOOLEAN enableState;    // TRUE to enable, FALSE to disable
} SET_PROC_PROTECTION, *PSET_PROC_PROTECTION;

typedef enum _InjectType
{
  IT_Thread,      // CreateThread into LdrLoadDll
  IT_Apc,         // Force user APC into LdrLoadDll
  IT_MMap,        // Manual map
} InjectType;

typedef enum _MmapFlags
{
  KNoFlags         = 0x00,    // No flags
  KManualImports   = 0x01,    // Manually map import libraries
  KWipeHeader      = 0x04,    // Wipe image PE headers
  KHideVAD         = 0x10,    // Make image appear as PAGE_NOACESS region
  KRebaseProcess   = 0x40,    // If target image is an .exe file, process base address will be replaced with mapped module value
  KNoExceptions    = 0x01000, // Do not create custom exception handler
  KNoSxS           = 0x08000, // Do not apply SxS activation context
  KNoTLS           = 0x10000, // Skip TLS initialization and don't execute TLS callbacks
} KMmapFlags;

/// <summary>
/// Input for IOCTL_BLACKBONE_INJECT_DLL
/// </summary>
typedef struct _INJECT_DLL
{
  InjectType type;                // Type of injection
  wchar_t    FullDllPath[512];    // Fully-qualified path to the target dll
  wchar_t    initArg[512];        // Init routine argument
  ULONG      initRVA;             // Init routine RVA, if 0 - no init routine
  ULONG      pid;                 // Target process ID
  BOOLEAN    wait;                // Wait on injection thread
  BOOLEAN    unlink;              // Unlink module after injection
  BOOLEAN    erasePE;             // Erase PE headers after injection   
  KMmapFlags flags;               // Manual map flags
  ULONGLONG  imageBase;           // Image address in memory to manually map
  ULONG      imageSize;           // Size of memory image
  BOOLEAN    asImage;             // Memory chunk has image layout
} INJECT_DLL, *PINJECT_DLL;
#endif
