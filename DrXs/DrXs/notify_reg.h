#pragma once

#include "notify.h"

ULONG * CmpCallBackCount;
PVOID * CmpCallBackVector;
PLIST_ENTRY CallbackListHead;

typedef struct _DRX_REGISTRY6_CALLBACK
{
	LARGE_INTEGER cookie;
	PVOID context;
	PVOID callback;
	UNICODE_STRING altitude;
} DRX_REGISTRY6_CALLBACK, *PDRX_REGISTRY6_CALLBACK;

NTSTATUS getNotifyRegistryRoutine();
NTSTATUS kListNotifyRegistry(PDRX_BUFFER pszDest);
