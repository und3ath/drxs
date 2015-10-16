#pragma once 

#include "z_types.h"

#include <aux_klib.h>

NTSTATUS kModulesList(LPWSTR pszDest, size_t cbDest, LPWSTR *ppszDestEnd, size_t *pcbRemaining);
NTSTATUS getModuleFromAddr(ULONG_PTR addr, PDRX_BUFFER pszDest);
//NTSTATUS getModuleFromAddr(ULONG_PTR theAddr, LPWSTR pszDest, size_t cbDest, LPWSTR *ppszDestEnd, size_t *pcbRemaining);
NTSTATUS PrintCallBackInfos(ULONG_PTR addr);