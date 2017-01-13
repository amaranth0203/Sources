//作者：Tesla.Angela [www.vb-asm.com]

#include <ntddk.h>

typedef struct _SYSTEM_SERVICE_TABLE{
	PVOID  		ServiceTableBase; 
	PVOID  		ServiceCounterTableBase; 
	ULONGLONG  	NumberOfServices; 
	PVOID  		ParamTableBase; 
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

typedef struct _SERVICE_DESCRIPTOR_TABLE{
	SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe (native api)
	SYSTEM_SERVICE_TABLE win32k;    // win32k.sys   (gdi/user)
	SYSTEM_SERVICE_TABLE Table3;    // not used
	SYSTEM_SERVICE_TABLE Table4;    // not used
}SERVICE_DESCRIPTOR_TABLE,*PSERVICE_DESCRIPTOR_TABLE;

typedef NTSTATUS (__fastcall *NTCREATEFILE)(
  __out     PHANDLE FileHandle,
  __in      ACCESS_MASK DesiredAccess,
  __in      POBJECT_ATTRIBUTES ObjectAttributes,
  __out     PIO_STATUS_BLOCK IoStatusBlock,
  __in_opt  PLARGE_INTEGER AllocationSize,
  __in      ULONG FileAttributes,
  __in      ULONG ShareAccess,
  __in      ULONG CreateDisposition,
  __in      ULONG CreateOptions,
  __in      PVOID EaBuffer,
  __in      ULONG EaLength
);

NTKERNELAPI
UCHAR *
PsGetProcessImageFileName(PEPROCESS Process);

PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable;
NTCREATEFILE NtCreateFile=NULL;
ULONG OldTpVal;
ULONG index=82;	//待HOOK函数的INDEX

NTSTATUS __fastcall Fake_NtCreateFile(
  __out     PHANDLE FileHandle,
  __in      ACCESS_MASK DesiredAccess,
  __in      POBJECT_ATTRIBUTES ObjectAttributes,
  __out     PIO_STATUS_BLOCK IoStatusBlock,
  __in_opt  PLARGE_INTEGER AllocationSize,
  __in      ULONG FileAttributes,
  __in      ULONG ShareAccess,
  __in      ULONG CreateDisposition,
  __in      ULONG CreateOptions,
  __in      PVOID EaBuffer,
  __in      ULONG EaLength
)
{
	NTSTATUS st;
	st=NtCreateFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock, AllocationSize,FileAttributes,ShareAccess,CreateDisposition,CreateOptions,EaBuffer,EaLength);
	DbgPrint("Fake_NtCreateFile called: %x",st);
	return st;
}

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

ULONGLONG GetKeServiceDescriptorTable64() //鬼佬的方法
{
	char KiSystemServiceStart_pattern[13] = "\x8B\xF8\xC1\xEF\x07\x83\xE7\x20\x25\xFF\x0F\x00\x00";	//睇唔明系么春特征码
	ULONGLONG CodeScanStart = (ULONGLONG)&_strnicmp;
	ULONGLONG CodeScanEnd = (ULONGLONG)&KdDebuggerNotPresent;
	UNICODE_STRING Symbol;
	ULONGLONG i, tbl_address, b;
	for (i = 0; i < CodeScanEnd - CodeScanStart; i++)
	{
		if (!memcmp((char*)(ULONGLONG)CodeScanStart +i, (char*)KiSystemServiceStart_pattern,13))
		{ 
			for (b = 0; b < 50; b++)
			{
				tbl_address = ((ULONGLONG)CodeScanStart+i+b);
				if (*(USHORT*) ((ULONGLONG)tbl_address ) == (USHORT)0x8d4c)
					return ((LONGLONG)tbl_address +7) + *(LONG*)(tbl_address +3);
			}
		}
	}
	return 0;
}

void ShowStuff0(LONG int32num)
{
	CHAR b[4]={0};
	memcpy(&b[0],(PUCHAR)(&int32num)+0,1);DbgPrint("b[0] & 0xF=%ld\n",b[0] & 0xF);	//这个数参数的个数-4
	memcpy(&b[1],(PUCHAR)(&int32num)+1,1);DbgPrint("b[1] & 0xF=%ld\n",b[1] & 0xF);
	memcpy(&b[2],(PUCHAR)(&int32num)+2,1);DbgPrint("b[2] & 0xF=%ld\n",b[2] & 0xF);
	memcpy(&b[3],(PUCHAR)(&int32num)+3,1);DbgPrint("b[3] & 0xF=%ld\n",b[3] & 0xF);
}

ULONGLONG GetSSDTFuncCurAddr(ULONG id)
{
	LONG dwtmp=0;
	ULONGLONG add = 0;
	PULONG ServiceTableBase=NULL;
	ServiceTableBase=(PULONG)KeServiceDescriptorTable->ServiceTableBase;
	dwtmp=ServiceTableBase[id];
	ShowStuff0(dwtmp);
	dwtmp=dwtmp>>4;
	add=((LONGLONG)dwtmp + (ULONGLONG)ServiceTableBase);//&0xFFFFFFF0;
	return add;
}

#define SETBIT(x,y) x|=(1<<y) //将X的第Y位置1
#define CLRBIT(x,y) x&=~(1<<y) //将X的第Y位清0
#define GETBIT(x,y) (x & (1 << y)) //取X的第Y位，返回0或非0

ULONG GetOffsetAddress(ULONGLONG FuncAddr, CHAR ParamCount)
{
	LONG dwtmp=0,i;
	CHAR b=0,bits[4]={0};
	PULONG ServiceTableBase=NULL;
	ServiceTableBase=(PULONG)KeServiceDescriptorTable->ServiceTableBase;
	dwtmp=(LONG)(FuncAddr-(ULONGLONG)ServiceTableBase);
	dwtmp=dwtmp<<4;
	//处理参数
	if(ParamCount>4)
		ParamCount=ParamCount-4;
	else
		ParamCount=0;
	//获得dwtmp的第一个字节
	memcpy(&b,&dwtmp,1);
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
	memcpy(&dwtmp,&b,1);
	return dwtmp;
}

VOID FuckKeBugCheckEx()
{
	KIRQL irql;
	ULONGLONG myfun;
	UCHAR jmp_code[]="\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	myfun=(ULONGLONG)Fake_NtCreateFile;
	memcpy(jmp_code+6,&myfun,8);
	irql=WPOFFx64();
	memset(KeBugCheckEx,0x90,15);
	memcpy(KeBugCheckEx,jmp_code,14);
	WPONx64(irql);
}

/*
填写KeBugCheckEx的地址
在KeBugCheckEx填写jmp，跳到Fake_NtCreateFile
不能直接填写Fake_NtCreateFile的地址，因为它们不再同一个4GB
*/

VOID HookSSDT()
{
	KIRQL irql;
	LONG dwtmp=0;
	PULONG ServiceTableBase=NULL;
	//get old address
	dprintf("NtCreateFile index: %ld",index);
	NtCreateFile=(NTCREATEFILE)GetSSDTFuncCurAddr(index);
	dprintf("Old_NtCreateFile: %llx",(ULONGLONG)NtCreateFile);
	//show new address
	ServiceTableBase=(PULONG)KeServiceDescriptorTable->ServiceTableBase;
	//get offset value
	dwtmp=GetOffsetAddress((ULONGLONG)KeBugCheckEx,11);
	//set kebugcheckex
	FuckKeBugCheckEx();
	//record old offset value
	OldTpVal=ServiceTableBase[index];	
	irql=WPOFFx64();
	ServiceTableBase[index]=GetOffsetAddress((ULONGLONG)KeBugCheckEx,11);
	WPONx64(irql);
	dprintf("KeBugCheckEx: %llx",(ULONGLONG)KeBugCheckEx);
	dprintf("New_NtCreateFile: %llx",GetSSDTFuncCurAddr(index));
}

VOID UnhookSSDT()
{
	KIRQL irql;
	PULONG ServiceTableBase=NULL;
	ServiceTableBase=(PULONG)KeServiceDescriptorTable->ServiceTableBase;
	//set value
	irql=WPOFFx64();
	ServiceTableBase[index]=OldTpVal;	//GetOffsetAddress((ULONGLONG)NtCreateFile);
	WPONx64(irql);
}