#pragma once

#include "z_types.h"

#define TOKEN_FROZEN_MASK		0x00008000
#define PROTECTED_PROCESS_MASK	0x00000800


NTSTATUS sysProcessToken(SIZE_T szBufferIn, PVOID bufferIn, PDRX_BUFFER outBuffer);


NTSTATUS sysUnprotectProcess(SIZE_T szBufferIn, PVOID bufferIn, PDRX_BUFFER outBuffer);


NTSTATUS sysProcessTokenDuplicator(SIZE_T szBufferIn, PVOID bufferIn, PDRX_BUFFER outBuffer, HANDLE hSrcToken, PEPROCESS pToProcess);