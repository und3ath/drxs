#pragma once 


#include <ntddk.h>
#include <ntstrsafe.h>


#define zprintf(DrxBuffer, Format, ...) (RtlStringCbPrintfExW(*(DrxBuffer)->Buffer, *(DrxBuffer)->szBuffer, (DrxBuffer)->Buffer, (DrxBuffer)->szBuffer, STRSAFE_NO_TRUNCATION, Format, __VA_ARGS__))

extern char * PsGetProcessImageFileName(PEPROCESS monProcess);
extern NTSYSAPI NTSTATUS NTAPI ZwSetInformationProcess(__in HANDLE ProcessHandle, __in PROCESSINFOCLASS ProcessInformationClass, __in_bcount(ProcessInformationLength) PVOID ProcessInformation, __in ULONG ProcessInformationLength);


typedef struct _DRX_BUFFER {
	size_t * szBuffer;
	PWSTR * Buffer;
} DRX_BUFFER, *PDRX_BUFFER;


#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))


#ifndef DRX_mask3bits
#define DRX_mask3bits(addr)	 (((ULONG_PTR) (addr)) & ~7)
#endif

#define POOL_TAG	'xrd'

#define INDEX_UNK	0
#define INDEX_XP	1
#define INDEX_2K3	2
#define INDEX_VISTA	3
#define INDEX_2K8	4
#define INDEX_7		5
#define INDEX_2K8R2	6
#define INDEX_8		7
#define MAX_OS_LEN	8

#ifdef _M_IX86
#define EX_FAST_REF_MASK	0x07
#else
#define EX_FAST_REF_MASK	0x0f
#endif


ULONG INDEX_OS;


typedef struct _PS_PROTECTION {
	UCHAR Type : 3;
	UCHAR Audit : 1;
	UCHAR Signer : 4;
} PS_PROTECTION, *PPS_PROTECTION;

typedef struct _DRX_PROCESS_SIGNATURE_PROTECTION {
	UCHAR SignatureLevel;
	UCHAR SectionSignatureLevel;
	PS_PROTECTION Protection;
} DRX_PROCESS_SIGNATURE_PROTECTION, *PDRX_PROCESS_SIGNATURE_PROTECTION;

typedef struct _DRXDRV_PROCESS_PROTECT_INFORMATION {
	ULONG processId;
	DRX_PROCESS_SIGNATURE_PROTECTION SignatureProtection;
} DRXDRV_PROCESS_PROTECT_INFORMATION, *PDRXDRV_PROCESS_PROTECT_INFORMATION;