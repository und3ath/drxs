#include "notify_reg.h"

ULONG * CmpCallBackCount = NULL;
PVOID * CmpCallBackVector = NULL;
PLIST_ENTRY CallbackListHead = NULL;

NTSTATUS kListNotifyRegistry(PDRX_BUFFER pszDest)
{
	NTSTATUS status;
	ULONG i;
	PDRX_CALLBACK monCallBack;
	PLIST_ENTRY maListe;
	PDRX_REGISTRY6_CALLBACK monCallBack6;


	status = getNotifyRegistryRoutine();
	if (NT_SUCCESS(status))
	{
		if (INDEX_OS < INDEX_VISTA)
		{
			for (i = 0; (i < *CmpCallBackCount) && NT_SUCCESS(status); i++)
			{
				monCallBack = (PDRX_CALLBACK)DRX_mask3bits(CmpCallBackVector[i]);
				if (monCallBack != NULL)
				{					
					status = getModuleFromAddr((ULONG_PTR)monCallBack->callback, pszDest);
					if (NT_SUCCESS(status) || status == STATUS_NOT_FOUND)
					{
						status = zprintf(pszDest, L" - cookie %#.I64x\n", *(monCallBack->opt_cookie));
					}
						
				}
			}
		}
		else
		{
			for (maListe = CallbackListHead->Flink, i = 0; (maListe != CallbackListHead) && NT_SUCCESS(status); maListe = maListe->Flink, i++)
			{
				monCallBack6 = (PDRX_REGISTRY6_CALLBACK)(((ULONG_PTR)maListe) + sizeof(LIST_ENTRY) + 2 * ((INDEX_OS < INDEX_7) ? sizeof(PVOID) : sizeof(ULONG)));
					
				status = getModuleFromAddr((ULONG_PTR)monCallBack6->callback, pszDest);
				if (NT_SUCCESS(status) || status == STATUS_NOT_FOUND)
				{
					status = zprintf(pszDest, L" - alt %wZ - cookie %#.I64x\n", &(monCallBack6->altitude), monCallBack6->cookie);
				}				
			}
		}
	}
	
	return status;
}

NTSTATUS getNotifyRegistryRoutine()
{
	NTSTATUS retour = STATUS_NOT_FOUND;
#ifdef _M_X64
	UCHAR PTRN_WNT5_Vector[] = { 0x4c, 0x8d, 0x3d };
	UCHAR PTRN_WNT5_Count[] = { 0x0f, 0xc1, 0x05 };

	UCHAR PTRN_WN60_Head[] = { 0x48, 0x8b, 0xf0, 0x48 };
	LONG OFFS_WN60_Head = -9;
	UCHAR PTRN_WALL_Head[] = { 0x48, 0x8b, 0xf8, 0x48 };
	LONG OFFS_WALL_Head = -9;
#elif defined _M_IX86
	UCHAR PTRN_WNT5_Vector[] = { 0x53, 0x56, 0x57, 0xbb };
	UCHAR PTRN_WNT5_Count[] = { 0xff, 0xb9 };

	UCHAR PTRN_WN60_Head[] = { 0x8b, 0xcb, 0xe8 };
	LONG OFFS_WN60_Head = 12;
	UCHAR PTRN_WN61_Head[] = { 0x8b, 0xc7, 0xe8 };
	LONG OFFS_WN61_Head = -4;
	UCHAR PTRN_WIN8_Head[] = { 0x53, 0x8d, 0x55 };
	LONG OFFS_WIN8_Head = -4;
#endif
	PUCHAR refDebut = (PUCHAR)CmUnRegisterCallback, refFin = refDebut + PAGE_SIZE;
	PUCHAR pattern = NULL; SIZE_T taille = 0; LONG offsetTo = 0;

	if ((CmpCallBackVector && CmpCallBackCount) || CallbackListHead)
	{
		retour = STATUS_SUCCESS;
	}
	else
	{
		if (INDEX_OS < INDEX_VISTA)
		{
			retour = genericPointerSearch((PUCHAR *)&CmpCallBackVector, refDebut, refFin, PTRN_WNT5_Vector, sizeof(PTRN_WNT5_Vector), sizeof(PTRN_WNT5_Vector));
			if (NT_SUCCESS(retour))
			{
				retour = genericPointerSearch((PUCHAR *)&CmpCallBackCount, refDebut, refFin, PTRN_WNT5_Count, sizeof(PTRN_WNT5_Count), sizeof(PTRN_WNT5_Count));
			}
		}
		else
		{
			if (INDEX_OS < INDEX_7)
			{
				pattern = PTRN_WN60_Head;
				taille = sizeof(PTRN_WN60_Head);
				offsetTo = OFFS_WN60_Head;
			}
			else
			{
#ifdef _M_X64
				pattern = PTRN_WALL_Head;
				taille = sizeof(PTRN_WALL_Head);
				offsetTo = OFFS_WALL_Head;
#elif defined _M_IX86
				if (INDEX_OS < INDEX_8)
				{
					pattern = PTRN_WN61_Head;
					taille = sizeof(PTRN_WN61_Head);
					offsetTo = OFFS_WN61_Head;
				}
				else
				{
					pattern = PTRN_WIN8_Head;
					taille = sizeof(PTRN_WIN8_Head);
					offsetTo = OFFS_WIN8_Head;
				}
#endif
			}
			retour = genericPointerSearch((PUCHAR *)&CallbackListHead, refDebut, refFin, pattern, taille, offsetTo);
		}
	}
	return retour;
}
