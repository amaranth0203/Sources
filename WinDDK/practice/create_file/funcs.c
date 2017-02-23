#include "funcs.h"

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

VOID MySleep(LONG msec)
{
  LARGE_INTEGER my_interval;
  my_interval.QuadPart = DELAY_ONE_MILLISECOND;
  my_interval.QuadPart *= msec;
  KeDelayExecutionThread(KernelMode,0,&my_interval);
}

//CHAR*转WCHAR*
//输入窄字符串首地址，输出宽字符串，BUFFER需要已经分配好空间
VOID CharToWchar(PCHAR src, PWCHAR dst)
{
	UNICODE_STRING uString;
	ANSI_STRING aString;
	RtlInitAnsiString(&aString,src);
	RtlAnsiStringToUnicodeString(&uString,&aString,TRUE);
	wcscpy(dst,uString.Buffer);
	RtlFreeUnicodeString(&uString);
}

void ZwCreateFolder( char* FolderPath ) {
  NTSTATUS st;
  HANDLE FileHandle;
  OBJECT_ATTRIBUTES ObjectAttributes;
  IO_STATUS_BLOCK IoStatusBlock;
  UNICODE_STRING UniFileName;
  WCHAR wsFileName[2048]= {0};
  CharToWchar(FolderPath,wsFileName);
  RtlInitUnicodeString(&UniFileName, wsFileName);
  InitializeObjectAttributes(&ObjectAttributes,
                             &UniFileName,
                             OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                             NULL,
                             NULL);
  st=IoCreateFile(&FileHandle,
                  GENERIC_READ,
                  &ObjectAttributes,
                  &IoStatusBlock,
                  0,
                  FILE_ATTRIBUTE_NORMAL,
                  0,
                  FILE_CREATE,
                  FILE_DIRECTORY_FILE,
                  NULL,
                  0,
                  0,
                  NULL,
                  IO_NO_PARAMETER_CHECKING);
  if(NT_SUCCESS(st))
    ZwClose(FileHandle);
}

/*
/// <summary>
/// Inject dll into process
/// </summary>
/// <param name="pid">Target PID</param>
/// <param name="pPath">TFull-qualified dll path</param>
/// <returns>Status code</returns>
NTSTATUS BBInjectDll( IN PINJECT_DLL pData )
{
  NTSTATUS status = STATUS_SUCCESS;
  NTSTATUS threadStatus = STATUS_SUCCESS;
  PEPROCESS pProcess = NULL;
  status = PsLookupProcessByProcessId( (HANDLE)pData->pid, &pProcess );
  if (NT_SUCCESS( status ))
  {
    KAPC_STATE apc;
    UNICODE_STRING ustrPath, ustrNtdll;
    SET_PROC_PROTECTION prot = { 0 };
    PVOID pNtdll = NULL;
    PVOID LdrLoadDll = NULL;
    PVOID systemBuffer = NULL;
    BOOLEAN isWow64 = (PsGetProcessWow64Process( pProcess ) != NULL) ? TRUE : FALSE;
    LARGE_INTEGER procTimeout = { 0 };
    // Process in signaled state, abort any operations
    if (KeWaitForSingleObject( pProcess, Executive, KernelMode, FALSE, &procTimeout ) == STATUS_WAIT_0)
    {
      DPRINT( "BlackBone: %s: Process %u is terminating. Abort\n", __FUNCTION__, pData->pid );
      if (pProcess)
        ObDereferenceObject( pProcess );
      return STATUS_PROCESS_IS_TERMINATING;
    }
    // Copy mmap image buffer to system space.
    // Buffer will be released in mapping routine automatically
    if (pData->type == IT_MMap && pData->imageBase)
    {
      __try
      {
        ProbeForRead( (PVOID)pData->imageBase, pData->imageSize, 1 );
        systemBuffer = ExAllocatePoolWithTag( PagedPool, pData->imageSize, BB_POOL_TAG );
        RtlCopyMemory( systemBuffer, (PVOID)pData->imageBase, pData->imageSize );
      }
      __except (EXCEPTION_EXECUTE_HANDLER)
      {
        DPRINT( "BlackBone: %s: AV in user buffer: 0x%p - 0x%p\n", __FUNCTION__,                pData->imageBase, pData->imageBase + pData->imageSize );
        if (pProcess)
          ObDereferenceObject( pProcess );
        return STATUS_INVALID_USER_BUFFER;
      }
    }
    KeStackAttachProcess( pProcess, &apc );
    RtlInitUnicodeString( &ustrPath, pData->FullDllPath );
    RtlInitUnicodeString( &ustrNtdll, L"Ntdll.dll" );
    // Handle manual map separately
    if (pData->type == IT_MMap)
    {
      MODULE_DATA mod = { 0 };
      __try {
        status = BBMapUserImage(
          pProcess, &ustrPath, systemBuffer,
          pData->imageSize, pData->asImage, pData->flags,
          pData->initRVA, pData->initArg, &mod
          );
      }
      __except (EXCEPTION_EXECUTE_HANDLER){
        DPRINT( "BlackBone: %s: Fatal exception in BBMapUserImage. Exception code 0x%x\n", 
                __FUNCTION__, GetExceptionCode() );
      }
      KeUnstackDetachProcess( &apc );
      if (pProcess)
        ObDereferenceObject( pProcess );
      return status;
    }
    // Get ntdll base
    pNtdll = BBGetUserModule( pProcess, &ustrNtdll, isWow64 );
    if (!pNtdll)
    {
      DPRINT( "BlackBone: %s: Failed to get Ntdll base\n", __FUNCTION__ );
      status = STATUS_NOT_FOUND;
    }
    // Get LdrLoadDll address
    if (NT_SUCCESS( status ))
    {
      LdrLoadDll = BBGetModuleExport( pNtdll, "LdrLoadDll", pProcess, NULL );
      if (!LdrLoadDll)
      {
        DPRINT( "BlackBone: %s: Failed to get LdrLoadDll address\n", __FUNCTION__ );
        status = STATUS_NOT_FOUND;
      }
    }
    // If process is protected - temporarily disable protection
    if (PsIsProtectedProcess( pProcess ))
    {
      prot.pid = pData->pid;
      prot.enableState = FALSE;
      BBSetProtection( &prot );
    }
    // Call LdrLoadDll
    if (NT_SUCCESS( status ))
    {
      SIZE_T size = 0;
      PINJECT_BUFFER pUserBuf = isWow64 ? BBGetWow64Code( LdrLoadDll, &ustrPath ) : BBGetNativeCode( LdrLoadDll, &ustrPath );
      if (pData->type == IT_Thread)
      {
        status = BBExecuteInNewThread( pUserBuf, NULL, THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER, pData->wait, &threadStatus );
        // Injection failed
        if (!NT_SUCCESS( threadStatus ))
        {
          status = threadStatus;
          DPRINT( "BlackBone: %s: User thread failed with status - 0x%X\n", __FUNCTION__, status );
        }
        // Call Init routine
        else
        {
          if (pUserBuf->module != 0 && pData->initRVA != 0)
          {
            RtlCopyMemory( pUserBuf->buffer, pData->initArg, sizeof( pUserBuf->buffer ) );
            BBExecuteInNewThread( (PUCHAR)pUserBuf->module + pData->initRVA, pUserBuf->buffer,
                                  THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER, TRUE, &threadStatus );
          }
          else if (pUserBuf->module == 0)
            DPRINT( "BlackBone: %s: Module base = 0. Aborting\n", __FUNCTION__ );
        }
      }
      else if (pData->type == IT_Apc)
      {
        status = BBApcInject( pUserBuf, (HANDLE)pData->pid, pData->initRVA, pData->initArg );
      }
      else
      {
        DPRINT( "BlackBone: %s: Invalid injection type specified - %d\n", __FUNCTION__, pData->type );
        status = STATUS_INVALID_PARAMETER;
      }
      // Post-inject stuff
      if (NT_SUCCESS( status ))
      {
        // Unlink module
        if (pData->unlink)
          BBUnlinkFromLoader( pProcess, pUserBuf->module, isWow64 );
        // Erase header
        if (pData->erasePE)
        {
          __try
          {
            PIMAGE_NT_HEADERS64 pHdr = RtlImageNtHeader( pUserBuf->module );
            if (pHdr)
            {
              ULONG oldProt = 0;
              size = (pHdr->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) ?
                ((PIMAGE_NT_HEADERS32)pHdr)->OptionalHeader.SizeOfHeaders :
                pHdr->OptionalHeader.SizeOfHeaders;
              if (NT_SUCCESS( ZwProtectVirtualMemory( ZwCurrentProcess(), &pUserBuf->module, &size, PAGE_EXECUTE_READWRITE, &oldProt ) ))
              {
                RtlZeroMemory( pUserBuf->module, size );
                ZwProtectVirtualMemory( ZwCurrentProcess(), &pUserBuf->module, &size, oldProt, &oldProt );
                DPRINT( "BlackBone: %s: PE headers erased. \n", __FUNCTION__ );
              }
            }
            else
              DPRINT( "BlackBone: %s: Failed to retrieve PE headers for image\n", __FUNCTION__ );
          }
          __except (EXCEPTION_EXECUTE_HANDLER)
          {
            DPRINT( "BlackBone: %s: Exception during PE header erease: 0x%X\n", __FUNCTION__, GetExceptionCode() );
          }
        }
      }
      ZwFreeVirtualMemory( ZwCurrentProcess(), &pUserBuf, &size, MEM_RELEASE );
    }
    // Restore protection
    if (prot.pid != 0)
    {
      prot.enableState = TRUE;
      BBSetProtection( &prot );
    }
    KeUnstackDetachProcess( &apc );
  }
  else
    DPRINT( "BlackBone: %s: PsLookupProcessByProcessId failed with status 0x%X\n", __FUNCTION__, status );
  if (pProcess)
    ObDereferenceObject( pProcess );
  return status;
}
*/



/// <summary>
/// Create worker thread for user-mode calls
/// </summary>
/// <param name="pContext">Map context</param>
/// <returns>Status code</returns>
NTSTATUS BBCreateWorkerThread( IN PMMAP_CONTEXT pContext )
{
    NTSTATUS status = STATUS_SUCCESS;
    SIZE_T codeSize = 0x1000;
    ASSERT( pContext != NULL );
    if (pContext == NULL)
        return STATUS_INVALID_PARAMETER;
    pContext->pWorkerBuf = NULL;
    status = ZwAllocateVirtualMemory( ZwCurrentProcess(), &pContext->pWorkerBuf, 0, &codeSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
    if (NT_SUCCESS( status ))
    {
        PUCHAR pBuf = pContext->pWorkerBuf;
        UNICODE_STRING ustrNtdll;
        BOOLEAN wow64 = PsGetProcessWow64Process( pContext->pProcess ) != NULL;
        PVOID pNtDelayExec = BBGetModuleExport(
            BBGetUserModule( pContext->pProcess, &ustrNtdll, wow64 ),
            "NtDelayExecution", pContext->pProcess, NULL
            );
        RtlUnicodeStringInit( &ustrNtdll, L"ntdll.dll" );
        if (pNtDelayExec)
        {
            OBJECT_ATTRIBUTES obattr = { 0 };
            PLARGE_INTEGER pDelay = (PLARGE_INTEGER)(pBuf + 0x100);
            pDelay->QuadPart = -(5ll * 10 * 1000);
            ULONG ofst = 0;
            if (wow64)
            {            
                *(PUCHAR)(pBuf + ofst) = 0x68;                      // push pDelay
                *(PULONG)(pBuf + ofst + 1) = (ULONG)(ULONG_PTR)pDelay;      //
                ofst += 5;
                *(PUSHORT)(pBuf + ofst) = 0x016A;                   // push TRUE
                ofst += 2;
                *(PUCHAR)(pBuf + ofst) = 0xB8;                      // mov eax, pFn
                *(PULONG)(pBuf + ofst + 1) = (ULONG)(ULONG_PTR)pNtDelayExec;//
                ofst += 5;
                *(PUSHORT)(pBuf + ofst) = 0xD0FF;                   // call eax
                ofst += 2;
                *(PUSHORT)(pBuf + ofst) = 0xF0EB;                   // jmp
                ofst += 2;
            }
            else
            {
                *(PUSHORT)(pBuf + ofst) = 0xB948;           // mov rcx, TRUE
                *(PULONG_PTR)(pBuf + ofst + 2) = TRUE;      //
                ofst += 10;
                *(PUSHORT)(pBuf + ofst) = 0xBA48;           // mov rdx, pDelay
                *(PVOID*)(pBuf + ofst + 2) = pDelay;        //
                ofst += 10;
                *(PUSHORT)(pBuf + ofst) = 0xB848;           // mov rax, pNtDelayExec
                *(PVOID*)(pBuf + ofst + 2) = pNtDelayExec;  //
                ofst += 10;
                *(PUSHORT)(pBuf + ofst) = 0xD0FF;           // call rax
                ofst += 2;
                *(PUSHORT)(pBuf + ofst) = 0xDEEB;           // jmp
                ofst += 2;
            }
            InitializeObjectAttributes( &obattr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL );
            status = ZwCreateThreadEx(
                &pContext->hWorker, THREAD_ALL_ACCESS, &obattr,
                ZwCurrentProcess(), pContext->pWorkerBuf, NULL, 0,
                0, 0x1000, 0x100000, NULL
                );
            if (NT_SUCCESS( status ))
                ObReferenceObjectByHandle( pContext->hWorker, THREAD_ALL_ACCESS, *PsThreadType, KernelMode, &pContext->pWorker, NULL );
        }
        else
        {
            status = STATUS_NOT_FOUND;
        }
    }
    return status;
}


/// <summary>
/// Map new user module
/// </summary>
/// <param name="pProcess">Target process</param>
/// <param name="path">Image path</param>
/// <param name="buffer">Image buffer</param>
/// <param name="size">Image buffer size</param>
/// <param name="asImage">Buffer has image memory layout</param>
/// <param name="flags">Mapping flags</param>
/// <param name="initRVA">Init routine RVA</param>
/// <param name="initArg">Init argument</param>
/// <param name="pImage">Mapped image data</param>
/// <returns>Status code</returns>
NTSTATUS BBMapUserImage(
    IN PEPROCESS pProcess,
    IN PUNICODE_STRING path,
    IN PVOID buffer, IN ULONG_PTR size,
    IN BOOLEAN asImage,
    IN KMmapFlags flags,
    IN ULONG initRVA,
    IN PWCH initArg,
    OUT PMODULE_DATA pImage
    )
{
    NTSTATUS status = STATUS_SUCCESS;
    MMAP_CONTEXT context = { 0 };
    ASSERT( pProcess != NULL );
    if (pProcess == NULL)
    {
        DPRINT( "BlackBone: %s: No process provided\n", __FUNCTION__ );
        return STATUS_INVALID_PARAMETER;
    }
    context.pProcess = pProcess;
    InitializeListHead( &context.modules );
    DPRINT( "BlackBone: %s: Mapping image '%wZ' with flags 0x%X\n", __FUNCTION__, path, flags );
    // Create worker
    status = BBCreateWorkerThread( &context );
    if (NT_SUCCESS( status ))
    {
        SIZE_T mapSize = 0x2000;
        status = ZwAllocateVirtualMemory( ZwCurrentProcess(), &context.userMem, 0, &mapSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
    }
    // Create sync event
    if (NT_SUCCESS( status ))
    {
        OBJECT_ATTRIBUTES eventAttr = { 0 };
        InitializeObjectAttributes( &eventAttr, 0, 0, 0, 0 );
        status = ZwCreateEvent( &context.hSync, EVENT_ALL_ACCESS, &eventAttr, NotificationEvent, FALSE );
        if (NT_SUCCESS( status ))
            status = ObReferenceObjectByHandle( context.hSync, EVENT_MODIFY_STATE, *ExEventObjectType, UserMode, &context.pSync, NULL );
        if (!NT_SUCCESS( status ))
            DPRINT( "BlackBone: %s: Failed to create sync event. Status 0x%X\n", __FUNCTION__, status );
        UNICODE_STRING ustrNtdll;
        RtlUnicodeStringInit( &ustrNtdll, L"ntdll.dll" );
        PVOID pNtdll = BBGetUserModule( context.pProcess, &ustrNtdll, PsGetProcessWow64Process( pProcess ) != NULL );
        context.pSetEvent = BBGetModuleExport( pNtdll, "NtSetEvent", pProcess, NULL );
        context.pLoadImage = BBGetModuleExport( pNtdll, "LdrLoadDll", pProcess, NULL );
    }
    // Map module
    if (NT_SUCCESS( status ))
        status = BBFindOrMapModule( pProcess, path, buffer, size, asImage, flags, &context, pImage );
    // Enable exceptions for WOW64 process
    if (NT_SUCCESS( status ) && !(flags & KNoExceptions))
    {
        if (dynData.KExecOpt != 0)
        {
            PKEXECUTE_OPTIONS pExecOpt = (PKEXECUTE_OPTIONS)((PUCHAR)pProcess + dynData.KExecOpt);
            pExecOpt->Flags.ImageDispatchEnable = 1;
            pExecOpt->Flags.ExecuteDispatchEnable = 1;
            pExecOpt->Flags.Permanent = 1;
        }
        else
            DPRINT( "BlackBone: %s: Invalid KEXECUTE_OPTIONS offset\n", __FUNCTION__ );
    }
    // Rebase process
    if (NT_SUCCESS( status ) && flags & KRebaseProcess)
    {
        PPEB pPeb = (PPEB)PsGetProcessPeb( pProcess );
        PPEB32 pPeb32 = (PPEB32)PsGetProcessWow64Process( pProcess );
        pPeb->ImageBaseAddress = pImage->baseAddress;
        if (pPeb32)
            pPeb32->ImageBaseAddress = (ULONG)(ULONG_PTR)pImage->baseAddress;
    }
    // Run module initializers
    if (NT_SUCCESS( status ))
    {
        __try{
            BBCallInitializers( &context, (flags & KNoTLS) != 0 );
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            DPRINT( "BlackBone: %s: Exception during initialization phase.Exception code 0x%X\n", __FUNCTION__, GetExceptionCode() );
            status = STATUS_UNHANDLED_EXCEPTION;
        }
    }
    // Run user initializer
    if (NT_SUCCESS( status ) && initRVA != 0)
    {
        PVOID pInitRoutine = (PUCHAR)pImage->baseAddress + initRVA;
        memcpy( context.userMem->buffer, initArg, 512 * sizeof( WCHAR ) );
        BBCallRoutine( FALSE, &context, pInitRoutine, 1, context.userMem->buffer );
    }
    // Cleanup
    // Event
    if (context.pSync)
        ObDereferenceObject( context.pSync );
    if (context.hSync)
        ZwClose( context.hSync );
    // Worker thread
    if (context.pWorker)
        ObDereferenceObject( context.pWorker );
    if (context.hWorker)
    {
        LARGE_INTEGER timeout = { 0 };
        timeout.QuadPart = -(10ll * 10 * 1000);    // 10 ms
        ZwTerminateThread( context.hWorker, 0 );
        ZwWaitForSingleObject( context.hWorker, TRUE, &timeout );
        ZwClose( context.hWorker );
    }
    // Worker code buffer
    if (context.pWorkerBuf)
    {
        SIZE_T mapSize = 0;
        ZwFreeVirtualMemory( ZwCurrentProcess(), &context.pWorkerBuf, &mapSize, MEM_RELEASE );
    }
    // Code buffer
    if (context.userMem)
    {
        SIZE_T mapSize = 0;
        ZwFreeVirtualMemory( ZwCurrentProcess(), &context.userMem, &mapSize, MEM_RELEASE );
    }
    // Cleanup module list
    while (!IsListEmpty( &context.modules ))
    {
        PVOID pListEntry = context.modules.Flink;
        PMODULE_DATA pEntry = (PMODULE_DATA)CONTAINING_RECORD( pListEntry, MODULE_DATA, link );
        RemoveHeadList( &context.modules );
        RtlFreeUnicodeString( &pEntry->fullPath );
        ExFreePoolWithTag( pListEntry, BB_POOL_TAG );
    }
    return status;
}
