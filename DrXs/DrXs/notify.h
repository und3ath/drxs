#pragma once 

#include <ntifs.h>
#include "z_types.h"
#include "mod_memory.h"
#include "modules.h"
//#include "notify_process.h"
//#include "notify_thread.h"



typedef struct _DRX_CALLBACK
{
#ifdef _M_IX86
	PVOID unk0;
#endif
	PVOID * callback;
	LARGE_INTEGER * opt_cookie; // structure de feignant pour les process;threads;images aussi
} DRX_CALLBACK, *PDRX_CALLBACK;
