#include "modules.h"

NTSTATUS kModulesList(LPWSTR pszDest, size_t cbDest, LPWSTR *ppszDestEnd, size_t *pcbRemaining)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG i;
	ULONG modulesSize;
	AUX_MODULE_EXTENDED_INFO*  modules;
	ULONG  numberOfModules;

	*ppszDestEnd = pszDest;
	*pcbRemaining = cbDest;

	status = AuxKlibInitialize();
	if (NT_SUCCESS(status))
	{
		status = AuxKlibQueryModuleInformation(&modulesSize, sizeof(AUX_MODULE_EXTENDED_INFO), NULL);
		if (NT_SUCCESS(status))
		{
			if (modulesSize > 0)
			{
				numberOfModules = modulesSize / sizeof(AUX_MODULE_EXTENDED_INFO);
				modules = (AUX_MODULE_EXTENDED_INFO*)ExAllocatePoolWithTag(PagedPool, modulesSize, POOL_TAG);

				if (modules != NULL)
				{
					status = AuxKlibQueryModuleInformation(&modulesSize, sizeof(AUX_MODULE_EXTENDED_INFO), modules);
					if (NT_SUCCESS(status))
					{
						for (i = 0; i < numberOfModules; i++)
						{
							status = RtlStringCbPrintfExW(*ppszDestEnd, *pcbRemaining, ppszDestEnd, pcbRemaining, STRSAFE_NO_TRUNCATION, L"%p - %.8u [%S] %S\n", 
								modules[i].BasicInfo.ImageBase, 
								modules[i].ImageSize,
								modules[i].FullPathName + modules[i].FileNameOffset,
								modules[i].FullPathName
								);
						}
					}
					ExFreePoolWithTag(modules, POOL_TAG);
				}
			}
		}
	}

	return status;
}


NTSTATUS PrintCallBackInfos(ULONG_PTR addr)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG i;
	ULONG modulesSize;
	AUX_MODULE_EXTENDED_INFO* modules;
	ULONG numberOfModules;

	status = AuxKlibInitialize();
	if (NT_SUCCESS(status))
	{
		status = AuxKlibQueryModuleInformation(&modulesSize, sizeof(AUX_MODULE_EXTENDED_INFO), NULL);
		if (NT_SUCCESS(status))
		{
			if (modulesSize > 0)
			{
				numberOfModules = modulesSize / sizeof(AUX_MODULE_EXTENDED_INFO);
				modules = (AUX_MODULE_EXTENDED_INFO*)ExAllocatePoolWithTag(PagedPool, modulesSize, POOL_TAG);

				if (modules != NULL)
				{
					status = AuxKlibQueryModuleInformation(&modulesSize, sizeof(AUX_MODULE_EXTENDED_INFO), modules);
					if (NT_SUCCESS(status))
					{
						for (i = 0; i < numberOfModules; i++)
						{
							if (addr >= (ULONG_PTR)modules[i].BasicInfo.ImageBase && addr < ((ULONG_PTR)modules[i].BasicInfo.ImageBase + modules[i].ImageSize))
							{
								DbgPrint("CallbackAddress : %p\n", addr);
								DbgPrint("CallbackInfos : module - %s\n", modules[i].FullPathName);
							}

						}					
					}
					ExFreePoolWithTag(modules, POOL_TAG);
				}
			}
		}
	}

	return status;
}

// Get the module information from memory adress
NTSTATUS getModuleFromAddr(ULONG_PTR addr, PDRX_BUFFER pszDest)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG i;
	ULONG modulesSize;
	AUX_MODULE_EXTENDED_INFO*  modules;
	ULONG  numberOfModules;

	
	status = AuxKlibInitialize();
	if (NT_SUCCESS(status))
	{
		// Get the number of byte to allocate for the array
		status = AuxKlibQueryModuleInformation(&modulesSize, sizeof(AUX_MODULE_EXTENDED_INFO), NULL);
		if (NT_SUCCESS(status))
		{
			if (modulesSize > 0)
			{
				// number of modules
				numberOfModules = modulesSize / sizeof(AUX_MODULE_EXTENDED_INFO);
				// allocating module memory
				modules = (AUX_MODULE_EXTENDED_INFO*)ExAllocatePoolWithTag(PagedPool, modulesSize, POOL_TAG);

				if (modules != NULL)
				{
					// get all modules informations
					status = AuxKlibQueryModuleInformation(&modulesSize, sizeof(AUX_MODULE_EXTENDED_INFO), modules);
					if (NT_SUCCESS(status))
					{
						// loop trough all loaded module to find wich come from the given pointer
						for (i = 0; i < numberOfModules; i++)
						{
							status = STATUS_NOT_FOUND;
							// addr ptr must be bigger than the module imagebase (imao) | and | addr must be lower than base address + imagesize (imao not out-of-bound ) 
							// so the addr must be in the modules address space. 
							if (addr >= (ULONG_PTR)modules[i].BasicInfo.ImageBase && addr < ((ULONG_PTR)modules[i].BasicInfo.ImageBase + modules[i].ImageSize))
							{
								status = zprintf(pszDest, L"0x%p(*)[%S + 0x%08x]|",
									(PVOID)addr,
									modules[i].FullPathName + modules[i].FileNameOffset,
									addr - (ULONG_PTR)modules[i].BasicInfo.ImageBase);

								break;
							}
						}

						if (status == STATUS_NOT_FOUND)
						{
							status = zprintf(pszDest, L"0x%p [?]", addr);
							if (NT_SUCCESS(status)) status = STATUS_NOT_FOUND;
						}
					}
					ExFreePoolWithTag(modules, POOL_TAG);
				}
			}
		}
	}

	return status;
}


