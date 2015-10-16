//#include <ntddk.h>

#include "notify_process.h"
#include "notify_thread.h"
#include "notify_image.h"
#include "notify_reg.h"
#include "mod_process.h"

#include "drx.h"

#include "ioctl_def.h"


DRIVER_INITIALIZE DriverEntry;
VOID OnUnload(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS Function_IRP_MJ_CREATE(PDEVICE_OBJECT pDeviceObject, PIRP Irp);
NTSTATUS Function_IRP_MJ_CLOSE(PDEVICE_OBJECT pDeviceObject, PIRP Irp);
NTSTATUS Function_IRP_DEVICE_CONTROL(PDEVICE_OBJECT pDeviceObject, PIRP Irp);

ULONG getWindowsIndex();
extern PSHORT NtBuildNumber;

ULONG getWindowsIndex()
{
	switch (*NtBuildNumber)
	{
	case 2600:
		return INDEX_XP;
		break;
	case 3790:
		return INDEX_2K3;
		break;
	case 6000:
	case 6001:
		return INDEX_VISTA;
	case 6002:
		return INDEX_2K8;
		break;
	case 7600:
	case 7601:
		return INDEX_7;
		break;
	case 8102:
	case 8250:
	case 9200:
		return INDEX_8;
		break;
	default:
		return 0;
	}
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;
	UNICODE_STRING usDriverName, usDosDeviceName;

	RtlInitUnicodeString(&usDriverName, L"\\Device\\drx");
	RtlInitUnicodeString(&usDosDeviceName, L"\\DosDevices\\drx");

	status = IoCreateDevice(DriverObject, 0, &usDriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_UNKNOWN, FALSE, &g_myDevice);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoCreateDevice Failed.");
	}


	// create the symlink \\.\drx wich can be user by userland app to comunicate with the 
	// driver instance with deviceiocontrol function . 
	status = IoCreateSymbolicLink(&usDosDeviceName, &usDriverName);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoCreateSymbolicLink Failed.");
	}

	// IRP dispach table
	DriverObject->DriverUnload = OnUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = Function_IRP_MJ_CREATE;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = Function_IRP_MJ_CLOSE;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Function_IRP_DEVICE_CONTROL;

	// Getting the os build number from the extern call
	INDEX_OS = getWindowsIndex();

	status = STATUS_SUCCESS;
	return status;
}

// Delete the symlink on unload ' avoiding re registering the driver failing '
VOID OnUnload(IN PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING symLink;
	NTSTATUS status;
	RtlInitUnicodeString(&symLink, L"\\Device\\drx");

	status = IoDeleteSymbolicLink(&symLink);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoDeleteSymbolicLink Failed.");
	}
	IoDeleteDevice(pDriverObject->DeviceObject);

	DbgPrint("OnUnload");
}

NTSTATUS Function_IRP_MJ_CREATE(PDEVICE_OBJECT pDeviceObject, PIRP Irp)
{
	DbgPrint("IRP_MJ_CREATE.");
	return STATUS_SUCCESS;
}

NTSTATUS Function_IRP_MJ_CLOSE(PDEVICE_OBJECT pDeviceObject, PIRP Irp)
{
	DbgPrint("IRP_MJ_CLOSE.");
	return STATUS_SUCCESS;
}


// IRP dispatch routine. 
NTSTATUS Function_IRP_DEVICE_CONTROL(PDEVICE_OBJECT pDeviceObject, PIRP Irp)
{
	NTSTATUS status = STATUS_NOT_SUPPORTED;
	PIO_STACK_LOCATION pIoStackIrp = NULL;
	size_t szBufferIn, szBufferOut, szReallyOut = 0;
	PVOID bufferIn, bufferOut;
	DRX_BUFFER kOutputBuffer = { &szBufferOut, (PWSTR *)&bufferOut };
	ULONG i;
	PMDL pMdl;

	DbgPrint("IRP_DEVICE_CONTROL.");

	pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);
	if (pIoStackIrp)
	{
		szBufferIn = pIoStackIrp->Parameters.DeviceIoControl.InputBufferLength;
		szBufferOut = pIoStackIrp->Parameters.DeviceIoControl.OutputBufferLength;

		bufferIn = pIoStackIrp->Parameters.DeviceIoControl.Type3InputBuffer;
		bufferOut = Irp->UserBuffer;

		switch (pIoStackIrp->Parameters.DeviceIoControl.IoControlCode)
		{

		case IOCTL_LIST_PROCESS_NOTIF:
			status = kListNotifyProcesses(&kOutputBuffer);
			break;
		case IOCTL_DEREF_PROCESS_NOTIF:
			status = kUnregisterNotifyProcess(szBufferIn, bufferIn, &kOutputBuffer);
			break;

		case IOCTL_LIST_THREAD_NOTIF:
			status = kListNotifyThread(&kOutputBuffer);
			break;
		case IOCTL_DEREF_THREAD_NOTIF:
			status = kRemoveNotifyThread(szBufferIn, bufferIn, &kOutputBuffer);
			break;

		case IOCTL_LIST_IMAGE_NOTIF:
			status = kListNotifyImages(&kOutputBuffer);
			break;
		case IOCTL_DEREF_IMAGE_NOTIF:
			status = kRemoveNotifyImage(szBufferIn, bufferIn, &kOutputBuffer);
			break;

		case IOCTL_LIST_REG_NOTIF:
			status = kListNotifyRegistry(&kOutputBuffer);
			break;
		case IOCTL_DEREF_REG_NOTIF:
			break;

		case IOCTL_PROC_DUP_TOKEN:
			status = sysProcessToken(szBufferIn, bufferIn, &kOutputBuffer);
			break;

		case IOCTL_PROC_PROTECT:
			status = sysUnprotectProcess(szBufferIn, bufferIn, &kOutputBuffer);
			break;

		}

		if (NT_SUCCESS(status))
			szReallyOut = pIoStackIrp->Parameters.DeviceIoControl.OutputBufferLength - szBufferOut;
	}

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = szReallyOut;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;

}