//驱动开发模板_Win64
//作者：Tesla.Angela(GDUT.HWL)

#include <ntddk.h>
#include <windef.h>
#include "MyDriver.h"
#include "EnumDrvs.h"
#include "hook3sdt.h"

VOID DriverUnload(PDRIVER_OBJECT pDriverObj)
{	
	UNICODE_STRING strLink;
	RtlInitUnicodeString(&strLink, LINK_NAME);
	IoDeleteSymbolicLink(&strLink);
	IoDeleteDevice(pDriverObj->DeviceObject);
}

NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	PIO_STACK_LOCATION pIrpStack;
	ULONG uIoControlCode;
	PVOID pIoBuffer;
	ULONG uInSize;
	ULONG uOutSize;
	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;
	switch(uIoControlCode)
	{
		case IOCTL_HOOK:
		{	
			//获得自己的PID
			MyProcessId = (ULONG64)PsGetCurrentProcessId();	DbgPrint("MyProcessId: %ld",MyProcessId);
			//获得原始函数地址
			NtUserQueryWindow = (NTUSERQUERYWINDOW)GetSSSDTFuncCurAddr64(IndexOfNtUserQueryWindow);	DbgPrint("NtUserQueryWindow: %llx",(ULONG64)NtUserQueryWindow);
			//获得原始函数地址
			NtUserPostMessage = (NTUSERPOSTMESSAGE)GetSSSDTFuncCurAddr64(IndexOfNtUserPostMessage);	DbgPrint("NtUserPostMessage: %llx",(ULONG64)NtUserPostMessage);
			//获得原始函数地址
			NtUserCreateWindowEx = (NTUSERCREATEWINDOWEX)GetSSSDTFuncCurAddr64(IndexOfNtUserCreateWindowEx);	DbgPrint("NtUserCreateWindowEx: %llx",(ULONG64)NtUserCreateWindowEx);
			TestHook();
			status = STATUS_UNSUCCESSFUL;
			break;
		}
		case IOCTL_UNHOOK:
		{
			TestUnhook();
			status = STATUS_UNSUCCESSFUL;
			break;			
		}
	}
	if(status == STATUS_SUCCESS)
		pIrp->IoStatus.Information = uOutSize;
	else
		pIrp->IoStatus.Information = 0;	
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
	RTL_OSVERSIONINFOW	osi;
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING ustrLinkName;
	UNICODE_STRING ustrDevName;  
	PDEVICE_OBJECT pDevObj;
	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
	pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	pDriverObj->DriverUnload = DriverUnload;
	RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
	status = IoCreateDevice(pDriverObj, 0, &ustrDevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);
	if(!NT_SUCCESS(status))	return status;
	if(IoIsWdmVersionAvailable(1, 0x10))
		RtlInitUnicodeString(&ustrLinkName, LINK_GLOBAL_NAME);
	else
		RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
	status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);  	
	if(!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevObj); 
		return status;
	}
	//get hardcode
	osi.dwOSVersionInfoSize=sizeof(RTL_OSVERSIONINFOW);
	RtlFillMemory(&osi,sizeof(RTL_OSVERSIONINFOW),0);
	RtlGetVersion(&osi);
	switch(osi.dwBuildNumber)
	{
		case 7600:
		case 7601:	//82
		{
			break;
		}
		case 9200:		//83
		{
			IndexOfNtUserPostMessage = 0x1000+16;	//<---这是你要修改的ID(查表可知)
			IndexOfNtUserQueryWindow = 0x1000+17;
			break;
		}
		case 9600:		//84
		{
			IndexOfNtUserPostMessage = 0x1000+17;	//<---这是你要修改的ID(查表可知)
			IndexOfNtUserQueryWindow = 0x1000+18;
			break;
		}
		default:
		{
			return STATUS_UNSUCCESSFUL;
		}
	}
	//get sssdt base
	KeServiceDescriptorTableShadow = (PSYSTEM_SERVICE_TABLE)GetKeServiceDescriptorTableShadow64();
	return STATUS_SUCCESS;
}