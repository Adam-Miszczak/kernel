#ifndef _EFI_MEM_H
#define _EFI_MEM_H

#include "stdint.h"

#define EFI_CONVENTIONAL_MEMORY 7

typedef struct _efi_mem_desc {
	uint32_t type;
	void* physaddr;
	void* virtaddr;
	uint64_t numpages;
	uint64_t attribs;
} EFI_MEMORY_DESCRIPTOR;

extern const char* EFI_MEMORY_TYPE_STRINGS[];

#endif
