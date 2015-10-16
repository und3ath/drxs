#include "notify_process.h"


ULONG * PspCreateProcessNotifyRoutineCount = NULL;
ULONG * PspCreateProcessNotifyRoutineExCount = NULL;
PVOID * PspCreateProcessNotifyRoutine = NULL;

// Getting the unexported internal pointer to the callback array 
NTSTATUS getPspCreateProcessNotifyRoutine()
{
	NTSTATUS retour = STATUS_NOT_FOUND;
	//
	// Some offset comming from IDA decompilation .
	// ntosknl.exe
	//
#ifdef _M_X64
	//NT5 2000 | XP | server 2003
	UCHAR PTRN_WNT5_Process[] = { 0x41, 0xbc, 0x08, 0x00, 0x00, 0x00, 0x48, 0x8b, 0xeb };
	LONG OFFS_WNT5_Process = -4;

	// Windows 7 | Server 2008 r2
	UCHAR PTRN_WNO8_Process[] = { 0x40, 0xc0, 0xed, 0x02, 0x41, 0x22, 0xee, 0xa8, 0x02, 0x0f, 0x84 };
	LONG OFFS_WNO8_Process = sizeof(PTRN_WNO8_Process) + 4 + 3;

	// 
	UCHAR PTRN_WIN8_Process[] = { 0x40, 0xc0, 0xee, 0x02, 0x41, 0x22, 0xf6, 0xa8, 0x02, 0x0f, 0x84 };
	LONG OFFS_WIN8_Process = sizeof(PTRN_WIN8_Process) + 4 + 3;

	PUCHAR REF_D_WNO8_Process = (PUCHAR)CcMdlRead;
	PUCHAR REF_F_WNO8_Process = REF_D_WNO8_Process - 25 * PAGE_SIZE;

	PUCHAR REF_D_WIN8_Process = (PUCHAR)SeImpersonateClientEx;
	PUCHAR REF_F_WIN8_Process = REF_D_WIN8_Process + 25 * PAGE_SIZE;

	LONG OFFS_WNO8_CountEx = sizeof(PVOID) * MAX_NT6_PspCreateProcessNotifyRoutine;
	LONG OFFS_WIN8_CountEx = OFFS_WNO8_CountEx;
	LONG OFFS_WNT5_Count = sizeof(PVOID) * MAX_NT5_PspCreateProcessNotifyRoutine;
	LONG OFFS_WNO8_Count = OFFS_WNO8_CountEx + sizeof(ULONG);
	LONG OFFS_WIN8_Count = -0x18;
#elif defined _M_IX86
	UCHAR PTRN_WNT5_Process[] = { 0x56, 0x57, 0x74 };
	LONG OFFS_WNT5_Process = sizeof(PTRN_WNT5_Process) + 2;
	UCHAR PTRN_WNO8_Process[] = { 0x33, 0xdb, 0xc7, 0x45 };
	LONG OFFS_WNO8_Process = sizeof(PTRN_WNO8_Process) + 1;
	UCHAR PTRN_WIN8_Process[] = { 0x33, 0xdb, 0x89, 0x5d, 0x0c, 0xbe };
	LONG OFFS_WIN8_Process = sizeof(PTRN_WIN8_Process);

	PUCHAR REF_D_WNO8_Process = (PUCHAR)PsSetCreateProcessNotifyRoutine;
	PUCHAR REF_F_WNO8_Process = REF_D_WNO8_Process + 25 * PAGE_SIZE;
	PUCHAR REF_D_WIN8_Process = (PUCHAR)IoConnectInterrupt;
	PUCHAR REF_F_WIN8_Process = REF_D_WIN8_Process - 25 * PAGE_SIZE;

	LONG OFFS_WNO8_CountEx = sizeof(PVOID) * MAX_NT6_PspCreateProcessNotifyRoutine;
	LONG OFFS_WIN8_CountEx = -0x20;
	LONG OFFS_WNT5_Count = sizeof(PVOID) * MAX_NT5_PspCreateProcessNotifyRoutine;
	LONG OFFS_WNO8_Count = OFFS_WNO8_CountEx + sizeof(ULONG);
	LONG OFFS_WIN8_Count = OFFS_WIN8_CountEx - sizeof(ULONG);
#endif

	PUCHAR pointeur = NULL, pattern = NULL, refDebut = NULL, refFin = NULL; SIZE_T taille = 0; LONG offsetTo = 0;
	LONG offsetToCountEx = 0, offsetToCount = 0;

	if (PspCreateProcessNotifyRoutine && ((INDEX_OS < INDEX_VISTA) || PspCreateProcessNotifyRoutineExCount) && PspCreateProcessNotifyRoutineCount)
	{
		retour = STATUS_SUCCESS;
	}
	else
	{
		if (INDEX_OS < INDEX_8)
		{
			if (INDEX_OS < INDEX_VISTA)
			{
				pattern = PTRN_WNT5_Process;
				taille = sizeof(PTRN_WNT5_Process);
				offsetTo = OFFS_WNT5_Process;
				offsetToCount = OFFS_WNT5_Count;
			}
			else
			{
				pattern = PTRN_WNO8_Process;
				taille = sizeof(PTRN_WNO8_Process);
				offsetTo = OFFS_WNO8_Process;
				offsetToCountEx = OFFS_WNO8_CountEx;
				offsetToCount = OFFS_WNO8_Count;
			}
			refDebut = REF_D_WNO8_Process;
			refFin = REF_F_WNO8_Process;
		}
		else
		{
			pattern = PTRN_WIN8_Process;
			taille = sizeof(PTRN_WIN8_Process);
			offsetTo = OFFS_WIN8_Process;
			refDebut = REF_D_WIN8_Process;
			refFin = REF_F_WIN8_Process;
			offsetToCountEx = OFFS_WIN8_CountEx;
			offsetToCount = OFFS_WIN8_Count;
		}

		// Search memory given pattern from ntoskrnl.exe (IDA Decompiler)
		retour = genericPointerSearch(&pointeur, refDebut, refFin, pattern, taille, offsetTo);
		if (NT_SUCCESS(retour))
		{
			PspCreateProcessNotifyRoutine = (PVOID)(pointeur);
			PspCreateProcessNotifyRoutineCount = (PULONG)(pointeur + offsetToCount);
			if (INDEX_OS >= INDEX_VISTA)
				PspCreateProcessNotifyRoutineExCount = (PULONG)(pointeur + offsetToCountEx);

			if (PspCreateProcessNotifyRoutine && ((INDEX_OS < INDEX_VISTA) || PspCreateProcessNotifyRoutineExCount) && PspCreateProcessNotifyRoutineCount)
				retour = STATUS_SUCCESS;
		}
	}
	return retour;
}

// List ProcessNotifyRoutine
NTSTATUS kListNotifyProcesses(PDRX_BUFFER pszDest)
{
	NTSTATUS status;
	ULONG i;
	PDRX_CALLBACK monCallBack;
	ULONG bonusCount;

	
		
		status = getPspCreateProcessNotifyRoutine();
		if (NT_SUCCESS(status))
		{
			// pointing to the pspcreateprocessnotifyroutinecount offset in memory
			bonusCount = *PspCreateProcessNotifyRoutineCount + ((INDEX_OS < INDEX_VISTA) ? 0 : *PspCreateProcessNotifyRoutineExCount);
			for (i = 0; (i < bonusCount) && NT_SUCCESS(status); i++)
			{
				// 3bits mask | memory address of callback (i.e callback routine of cng.sys )
				monCallBack = (PDRX_CALLBACK)DRX_mask3bits(PspCreateProcessNotifyRoutine[i]);
				if (monCallBack != NULL)
				{
					if (NT_SUCCESS(status))
					{
						// resolving module name and outputing to userland
						status = getModuleFromAddr((ULONG_PTR)monCallBack->callback, pszDest);
						// Need to fix ( use separator instead of cariage return ) ( more flexibility for parsing in userland via Qt) 
						if (NT_SUCCESS(status) || status == STATUS_NOT_FOUND)
						{
							status = zprintf(pszDest, L"\n");
						}
					}
				}
			}
		}
	
	return status;
}


// Unregister a callback routine using PsSetCreateProcessNotifyRoutine with second parameter to TRUE for unregistering
NTSTATUS kUnregisterNotifyProcess(SIZE_T szBufferIn, PVOID bufferIn, PDRX_BUFFER outBuffer)
{
	NTSTATUS status = STATUS_INVALID_HANDLE;
	UNICODE_STRING uString;

	if (bufferIn && (szBufferIn == sizeof(PCREATE_PROCESS_NOTIFY_ROUTINE)))
	{
		status = PsSetCreateProcessNotifyRoutine((PCREATE_PROCESS_NOTIFY_ROUTINE *)bufferIn, TRUE);
		// If failed it may be a ProcessNotifyRoutineEx 
		if (!NT_SUCCESS(status) && PsSetCreateProcessNotifyRoutineEx)
			status = PsSetCreateProcessNotifyRoutineEx((PCREATE_PROCESS_NOTIFY_ROUTINE_EX *)bufferIn, TRUE);

		if (NT_SUCCESS(status))
		{
			// Refresh NotifyProcessRoutine 
			kListNotifyProcesses(outBuffer);
		}

	}
	return status;


}

