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

typedef struct _KAPC_STATE {
  LIST_ENTRY ApcListHead[2] ;
  PKPROCESS Process ;
  UCHAR KernelApcInProgress ;
  UCHAR KernelApcPending ;
  UCHAR UserApcPending ;
} KAPC_STATE , *PKAPC_STATE ;

typedef struct _LDR_DATA_TABLE_ENTRY {
  LIST_ENTRY64		InLoadOrderLinks ;
  LIST_ENTRY64		InMemoryOrderLinks ;
  LIST_ENTRY64		InInitializationOrderLinks ;
  PVOID				DllBase ;
  PVOID				EntryPoint ;
  ULONG				SizeOfImage ;
  UNICODE_STRING	FullDllName ;
  UNICODE_STRING	BaseDllName ;
  ULONG				Flags ;
  USHORT			LoadCount ;
  USHORT			TlsIndex ;
  PVOID				SectionPointer ;
  ULONG				CheckSum ;
  PVOID				LoadedImports ;
  PVOID				EntryPointActivationContext ;
  PVOID				PatchInformation ;
  LIST_ENTRY64		ForwarderLinks ;
  LIST_ENTRY64		ServiceTagLinks ;
  LIST_ENTRY64		StaticLinks ;
  PVOID				ContextInformation ;
  ULONG64			OriginalBase ;
  LARGE_INTEGER		LoadTime ;
} LDR_DATA_TABLE_ENTRY , *PLDR_DATA_TABLE_ENTRY ;

NTKERNELAPI UCHAR* PsGetProcessImageFileName( IN PEPROCESS Process ) ;
NTKERNELAPI HANDLE PsGetProcessInheritedFromUniqueProcessId( IN PEPROCESS Process ) ;
NTKERNELAPI PPEB PsGetProcessPeb( PEPROCESS Process ) ;
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId( HANDLE Id , PEPROCESS *Process ) ;
NTKERNELAPI NTSTATUS PsLookupThreadByThreadId( HANDLE Id , PETHREAD *Thread ) ;
NTKERNELAPI PEPROCESS IoThreadToProcess( PETHREAD Thread ) ;
NTKERNELAPI VOID NTAPI KeAttachProcess( PEPROCESS Process ) ;
NTKERNELAPI VOID NTAPI KeDetachProcess( ) ;
NTKERNELAPI VOID NTAPI KeStackAttachProcess( PEPROCESS Process , PKAPC_STATE ApcState ) ;
NTKERNELAPI VOID NTAPI KeUnstackDetachProcess( PKAPC_STATE ApcState ) ;

static ULONG64 LdrInPebOffset = 0x018 ; /* peb.ldr */
static ULONG64 ModListInPebOffset = 0x010 ; /* peb.ldr.InLoadOrderModuleList */

PEPROCESS LookupProcess( HANDLE Pid ) ;
PETHREAD LookupThread( HANDLE Tid ) ;
VOID EnumThread( PEPROCESS Process ) ;
VOID EnumModule( PEPROCESS Process ) ;
VOID EnumProcess( ) ;
VOID ZwKillProcess( HANDLE Pid ) ;
VOID ZwKillThread( HANDLE Tid ) ;
