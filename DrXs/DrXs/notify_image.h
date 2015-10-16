#pragma once 

#include "notify.h"

#define MAX_NT_PspLoadImageNotifyRoutine 8

ULONG * PspLoadImageNotifyRoutineCount;
PVOID * PspLoadImageNotifyRoutine;

NTSTATUS getPspLoadImageNotifyRoutine();
NTSTATUS kListNotifyImages(PDRX_BUFFER pszDest);
NTSTATUS kRemoveNotifyImage(SIZE_T szBufferIn, PVOID BufferIn, PDRX_BUFFER outBuffer);