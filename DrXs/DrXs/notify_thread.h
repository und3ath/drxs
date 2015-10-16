#pragma once


#include "notify.h"

#define MAX_NT6_PspCreateThreadNotifyRoutine 64
#define MAX_NT5_PspCreateProcessNotifyRoutine 8


ULONG * PspCreateThreadNotifyRoutineCount;
PVOID * PspCreateThreadNotifyRoutine;

NTSTATUS getPspCreateThreadNotifyRoutine();
NTSTATUS kListNotifyThread(PDRX_BUFFER pszDest);
NTSTATUS kRemoveNotifyThread(SIZE_T pszInBuffer, PVOID InBuff, PDRX_BUFFER outBuffer);