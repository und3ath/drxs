#pragma once
#include "notify.h"

#define MAX_NT6_PspCreateProcessNotifyRoutine	64
#define MAX_NT5_PspCreateProcessNotifyRoutine	8

ULONG * PspCreateProcessNotifyRoutineCount;
ULONG * PspCreateProcessNotifyRoutineExCount;
PVOID * PspCreateProcessNotifyRoutine;


//NTSTATUS DbgPrintListNotifyProcess();
//NTSTATUS kListNotifyProcesses(LPWSTR pszDest, size_t cbDest, LPWSTR *ppszDestEnd, size_t *pcbRemaining);

//NTSTATUS ListProcessNotification(LPWSTR outBuffer);


NTSTATUS getPspCreateProcessNotifyRoutine();
NTSTATUS kListNotifyProcesses(PDRX_BUFFER pszDest);
NTSTATUS kUnregisterNotifyProcess(SIZE_T  szBufferIn, PVOID BufferIn, PDRX_BUFFER outBuffer);