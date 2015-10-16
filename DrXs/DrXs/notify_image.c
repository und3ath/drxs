#include "notify_image.h"

ULONG * PspLoadImageNotifyRoutineCount = NULL;
PVOID * PspLoadImageNotifyRoutine = NULL;

NTSTATUS getPspLoadImageNotifyRoutine()
{
	NTSTATUS retour = STATUS_NOT_FOUND;
#ifdef _M_X64
	UCHAR PTRN_WNT5_Image[] = { 0x48, 0x8d, 0x35 };
	LONG OFFS_WNT5_Image = sizeof(PTRN_WNT5_Image);
	UCHAR PTRN_WNT6_Image[] = { 0x48, 0x8d, 0x0d };
	LONG OFFS_WNT6_Image = sizeof(PTRN_WNT6_Image);

	LONG OFFS_WNT5_Count = -0x0c;
	LONG OFFS_WNT6_Count = sizeof(PVOID) * MAX_NT_PspLoadImageNotifyRoutine;
#elif defined _M_IX86
	UCHAR PTRN_WNT5_Image[] = { 0x6a, 0x00, 0x53, 0x56 };
	UCHAR PTRN_WNO8_Image[] = { 0x6a, 0x00, 0x8b, 0xcb, 0x8b, 0xc6 };
	UCHAR PTRN_WIN8_Image[] = { 0x33, 0xff, 0x6a, 0x00, 0x53, 0x8b, 0xc6 };
	LONG OFFS_WALL_Image = -(LONG) sizeof(PVOID);

	LONG OFFS_WNT5_Count = -0x18;
	LONG OFFS_WNO8_Count = sizeof(PVOID) * MAX_NT_PspLoadImageNotifyRoutine;
	LONG OFFS_WIN8_Count = -0x20;
#endif

	PUCHAR pointeur = NULL, pattern = NULL, refDebut = (PUCHAR)PsSetLoadImageNotifyRoutine, refFin = refDebut + PAGE_SIZE; SIZE_T taille = 0; LONG offsetTo = 0;
	LONG offsetToCountEx = 0, offsetToCount = 0;

	if (PspLoadImageNotifyRoutine && PspLoadImageNotifyRoutineCount)
	{
		retour = STATUS_SUCCESS;
	}
	else
	{
		if (INDEX_OS < INDEX_VISTA)
		{
			pattern = PTRN_WNT5_Image;
			taille = sizeof(PTRN_WNT5_Image);
#ifdef _M_X64
			offsetTo = OFFS_WNT5_Image;
#endif
			offsetToCount = OFFS_WNT5_Count;
		}
		else
		{
#ifdef _M_X64
			pattern = PTRN_WNT6_Image;
			taille = sizeof(PTRN_WNT6_Image);
			offsetTo = OFFS_WNT6_Image;
			offsetToCount = OFFS_WNT6_Count;
#elif defined _M_IX86
			if (INDEX_OS < INDEX_8)
			{
				pattern = PTRN_WNO8_Image;
				taille = sizeof(PTRN_WNO8_Image);
				offsetToCount = OFFS_WNO8_Count;
			}
			else
			{
				pattern = PTRN_WIN8_Image;
				taille = sizeof(PTRN_WIN8_Image);
				offsetToCount = OFFS_WIN8_Count;
			}
#endif
		}
#ifdef _M_IX86
		offsetTo = OFFS_WALL_Image;
#endif

		retour = genericPointerSearch(&pointeur, refDebut, refFin, pattern, taille, offsetTo);
		if (NT_SUCCESS(retour))
		{
			PspLoadImageNotifyRoutine = (PVOID)(pointeur);
			PspLoadImageNotifyRoutineCount = (PULONG)(pointeur + offsetToCount);

			if (PspLoadImageNotifyRoutine && PspLoadImageNotifyRoutineCount)
				retour = STATUS_SUCCESS;
		}
	}
	return retour;
}

NTSTATUS kListNotifyImages(PDRX_BUFFER pszDest)
{
	NTSTATUS status;
	ULONG i;
	PDRX_CALLBACK monCallBack;


	status = getPspLoadImageNotifyRoutine();
	if (NT_SUCCESS(status))
	{
		for (i = 0; (i < *PspLoadImageNotifyRoutineCount) && NT_SUCCESS(status); i++)
		{
			monCallBack = (PDRX_CALLBACK)DRX_mask3bits(PspLoadImageNotifyRoutine[i]);
			if (monCallBack != NULL)
			{					
				status = getModuleFromAddr((ULONG_PTR)monCallBack->callback, pszDest);
				if (NT_SUCCESS(status) || status == STATUS_NOT_FOUND)
				{
					status = zprintf(pszDest, L"\n");
				}				
			}
		}
	}
	
	return status;
}

NTSTATUS kRemoveNotifyImage(SIZE_T szBufferIn, PVOID bufferIn, PDRX_BUFFER outBuffer)
{
	NTSTATUS status = STATUS_INVALID_HANDLE;
	UNICODE_STRING uString;

	if (bufferIn && (szBufferIn == sizeof(PLOAD_IMAGE_NOTIFY_ROUTINE)))
	{
		status = PsRemoveLoadImageNotifyRoutine((PLOAD_IMAGE_NOTIFY_ROUTINE *)bufferIn);

		if (NT_SUCCESS(status))
		{
			kListNotifyImages(outBuffer);
		}

	}
	return status;

}