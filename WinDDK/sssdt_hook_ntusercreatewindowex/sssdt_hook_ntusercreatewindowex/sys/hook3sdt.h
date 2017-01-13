
#pragma intrinsic(__readmsr)

typedef struct _SYSTEM_SERVICE_TABLE{
	PVOID  		ServiceTableBase; 
	PVOID  		ServiceCounterTableBase; 
	ULONGLONG  	NumberOfServices; 
	PVOID  		ParamTableBase; 
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

typedef ULONG64 (__fastcall *NTUSERQUERYWINDOW)
(
	IN HWND		WindowHandle,
	IN ULONG64	TypeInformation
);

typedef ULONG64 (__fastcall *NTUSERPOSTMESSAGE)
(
	HWND 	hWnd,
	UINT 	Msg,
	WPARAM 	wParam,
	LPARAM 	lParam	 
);

typedef struct _LARGE_STRING
{
  ULONG Length;
  ULONG MaximumLength:31;
  ULONG bAnsi:1;
  PVOID Buffer;
} LARGE_STRING, *PLARGE_STRING;

typedef HWND (__fastcall *NTUSERCREATEWINDOWEX)	//NtUserCreateWindowEx,15 params
( 
    DWORD          dwExStyle,  
    PLARGE_STRING  plstrClassName,  
    PLARGE_STRING  plstrClsVersion,  
    PLARGE_STRING  plstrWindowName,  
    DWORD          dwStyle,  
    int            x,  
    int            y,  
    int            nWidth,  
    int            nHeight,  
    HWND           hWndParent,  
    HMENU          hMenu,  
    HINSTANCE      hInstance,  
    LPVOID         lpParam,  
    DWORD          dwFlags,  
    PVOID          acbiBuffer  
);

PSYSTEM_SERVICE_TABLE KeServiceDescriptorTableShadow = NULL;
ULONG64	ul64W32pServiceTable = 0;
ULONG64	IndexOfNtUserPostMessage = 0x100f;	//<---这是你要修改的ID(查表可知)
ULONG64	IndexOfNtUserQueryWindow = 0x1010;
ULONG64 IndexOfNtUserCreateWindowEx = 0x1076;
NTUSERQUERYWINDOW NtUserQueryWindow = NULL;	//<---则是原始函数的地址
NTUSERPOSTMESSAGE NtUserPostMessage = NULL;
NTUSERCREATEWINDOWEX NtUserCreateWindowEx = NULL;
ULONG64	MyProcessId = 0;
ULONG64	Win32kBase = 0;
ULONG	Win32kSize = 0;

KIRQL WPOFFx64()
{
	KIRQL irql=KeRaiseIrqlToDpcLevel();
	UINT64 cr0=__readcr0();
	cr0 &= 0xfffffffffffeffff;
	__writecr0(cr0);
	_disable();
	return irql;
}

void WPONx64(KIRQL irql)
{
	UINT64 cr0=__readcr0();
	cr0 |= 0x10000;
	_enable();
	__writecr0(cr0);
	KeLowerIrql(irql);
}

void SafeMemcpy(PVOID dst, PVOID src, DWORD length)
{
	KIRQL irql;
	irql=WPOFFx64();
	memcpy(dst,src,length);
	WPONx64(irql);
}

ULONGLONG GetKeServiceDescriptorTableShadow64()
{
	PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
    PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
	PUCHAR i = NULL;
	UCHAR b1=0,b2=0,b3=0;
	ULONG templong=0;
	ULONGLONG addr=0;
	for(i=StartSearchAddress;i<EndSearchAddress;i++)
	{
		if( MmIsAddressValid(i) && MmIsAddressValid(i+1) && MmIsAddressValid(i+2) )
		{
			b1=*i;
			b2=*(i+1);
			b3=*(i+2);
			if( b1==0x4c && b2==0x8d && b3==0x1d ) //4c8d1d
			{
				memcpy(&templong,i+3,4);
				addr = (ULONGLONG)templong + (ULONGLONG)i + 7;
				return addr;
			}
		}
	}
	return 0;
}

ULONGLONG GetSSSDTFuncCurAddr64(ULONG64 Index)
{
	ULONGLONG				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE	pWin32k;
	pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)KeServiceDescriptorTableShadow + sizeof(SYSTEM_SERVICE_TABLE));
	W32pServiceTable=(ULONGLONG)(pWin32k->ServiceTableBase);
	ul64W32pServiceTable = W32pServiceTable;
	qwTemp = W32pServiceTable + 4 * (Index-0x1000);	//这里是获得偏移地址的位置，要HOOK的话修改这里即可
	dwTemp = *(PLONG)qwTemp;
	dwTemp = dwTemp >> 4;
	qwTemp = W32pServiceTable + (LONG64)dwTemp;
	return qwTemp;
}

#define SETBIT(x,y) x|=(1<<y) //将X的第Y位置1
#define CLRBIT(x,y) x&=~(1<<y) //将X的第Y位清0
#define GETBIT(x,y) (x & (1 << y)) //取X的第Y位，返回0或非0

VOID ModifySSSDT(ULONG64 Index, ULONG64 Address, CHAR ParamCount)
{
	CHAR b=0,bits[4]={0};
	LONG i;

	ULONGLONG				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE	pWin32k;
	KIRQL					irql;
	pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)KeServiceDescriptorTableShadow + sizeof(SYSTEM_SERVICE_TABLE));	//4*8
	W32pServiceTable=(ULONGLONG)(pWin32k->ServiceTableBase);
	qwTemp = W32pServiceTable + 4 * (Index-0x1000);
	dwTemp = (LONG)(Address - W32pServiceTable);
	dwTemp = dwTemp << 4;	//DbgPrint("*(PLONG)qwTemp: %x, dwTemp: %x",*(PLONG)qwTemp,dwTemp);

	//处理参数
	if(ParamCount>4)
		ParamCount=ParamCount-4;
	else
		ParamCount=0;
	//获得dwtmp的第一个字节
	memcpy(&b,&dwTemp,1);
	//处理低四位，填写参数个数
	for(i=0;i<4;i++)
	{
		bits[i]=GETBIT(ParamCount,i);
		if(bits[i])
			SETBIT(b,i);
		else
			CLRBIT(b,i);
	}
	//把数据复制回去
	memcpy(&dwTemp,&b,1);
	
	irql=WPOFFx64();
	*(PLONG)qwTemp = dwTemp;
	WPONx64(irql);
}

ULONG64 FindFreeSpace(ULONG64 StartAddress, ULONG64 Length)
{
	UCHAR 	c=0;
	ULONG64 i=0,qw=0;
	for(i=StartAddress;i<StartAddress+Length;i++)
	{
		if(*(PUCHAR)i==0xC3)
		{
			RtlMoveMemory(&qw,(PVOID)(i+1),8);
			if(qw==0x9090909090909090)
			{
				return i+1;
			}
		}
	}
	return 0;
}

VOID HOOK_SSSDT(ULONG64 FunctionId, ULONG64 ProxyFunctionAddress, CHAR ParamCount)	//return OriFunctionAddress
{
	ULONG64 FreeSpace=0,OriFunctionAddress=0;
	LONG lng=0;
	UCHAR jmp_code[]="\xFF\x25\x00\x00\x00\x00";
	DbgPrint("ProxyFunctionAddress: %p",ProxyFunctionAddress);
	GetKernelModuleBase("win32k.sys",&Win32kBase,&Win32kSize);
	DbgPrint("Win32kBase: %p",Win32kBase);
	DbgPrint("Win32kBase: %ld",Win32kSize);
	if(Win32kBase==0 || Win32kSize==0)
		return;
	FreeSpace=FindFreeSpace(Win32kBase,Win32kSize);
	DbgPrint("FreeSpace: %p",FreeSpace);
	if(FreeSpace==0)
		return;
	SafeMemcpy((PVOID)FreeSpace,&ProxyFunctionAddress,8);
	OriFunctionAddress=GetSSSDTFuncCurAddr64(FunctionId);
	DbgPrint("OriFunctionAddress: %p",OriFunctionAddress);
	lng=(LONG)(FreeSpace-(OriFunctionAddress-6)-6);
	memcpy(&jmp_code[2],&lng,4);
	SafeMemcpy((PVOID)(OriFunctionAddress-6),jmp_code,6);
	ModifySSSDT(FunctionId,OriFunctionAddress-6,ParamCount);
	DbgPrint("HOOK_SSSDT OK!");
}

VOID UNHOOK_SSSDT(ULONG64 FunctionId, ULONG64 OriFunctionAddress, CHAR ParamCount)
{
	ModifySSSDT(FunctionId, (ULONG64)OriFunctionAddress, ParamCount);
	DbgPrint("UNHOOK_SSSDT OK!");
}

/*
ProxyNtUserPostMessage：这是你的代理函数
HOOK_SSSDT(函数ID,代理函数地址)
UNHOOK_SSSDT(函数ID,原始函数地址)
*/

ULONG64 ProxyNtUserPostMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if( NtUserQueryWindow(hWnd,0)==MyProcessId && PsGetCurrentProcessId()!=(HANDLE)MyProcessId )
	{
		DbgPrint("Do not fuck with me!");
		return 0;
	}
	else
	{	
		//DbgPrint("OriNtUserPostMessage called!");
		return NtUserPostMessage(hWnd,Msg,wParam,lParam);
	}
}

HWND ProxyNtUserCreateWindowEx
( 
    DWORD          dwExStyle,  
    PLARGE_STRING  plstrClassName,  
    PLARGE_STRING  plstrClsVersion,  
    PLARGE_STRING  plstrWindowName,  
    DWORD          dwStyle,  
    int            x,  
    int            y,  
    int            nWidth,  
    int            nHeight,  
    HWND           hWndParent,  
    HMENU          hMenu,  
    HINSTANCE      hInstance,  
    LPVOID         lpParam,  
    DWORD          dwFlags,  
    PVOID          acbiBuffer  
)
{
	HWND st=NtUserCreateWindowEx(dwExStyle,plstrClassName,plstrClsVersion,plstrWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam,dwFlags,acbiBuffer);
	/*if(plstrClassName && MmIsAddressValid(plstrClassName))
	{
		if(plstrClassName->Buffer && MmIsAddressValid(plstrClassName->Buffer))
		{
			char *buf1=NULL;
			WCHAR *buf2=NULL;//
			if(plstrWindowName->bAnsi)	
			{
				buf1=kmalloc(plstrWindowName->Length+1);
				//memcpy(buf1,plstrWindowName->Buffer,plstrWindowName->Length);
				//DbgPrint("NtUserCreateWindowEx HWND: [%p]%s\n",st,buf1);
				kfree(buf1);
			}
			else
			{
				buf2=kmalloc(plstrWindowName->Length+2);
				//memcpy(buf2,plstrWindowName->Buffer,plstrWindowName->Length);
				//DbgPrint("NtUserCreateWindowEx HWND: [%p]%S\n",st,buf2);
				kfree(buf2);
			}
			DbgPrint("[STR_OK]NtUserCreateWindowEx HWND: %llx\n",st);
			return st;
		}
	}*/
	DbgPrint("NtUserCreateWindowEx HWND: %llx\n",st);
	return st;
}

void TestHook()
{
	HOOK_SSSDT(IndexOfNtUserPostMessage,(ULONG64)ProxyNtUserPostMessage,4);
	HOOK_SSSDT(IndexOfNtUserCreateWindowEx,(ULONG64)ProxyNtUserCreateWindowEx,15);
}

void TestUnhook()
{
	UNHOOK_SSSDT(IndexOfNtUserPostMessage,(ULONG64)NtUserPostMessage,4);
	UNHOOK_SSSDT(IndexOfNtUserCreateWindowEx,(ULONG64)NtUserCreateWindowEx,15);
}