#ifndef _MEMORY_H
#define _MEMORY_H

#define UEFI_MMAP_SIZE 0x4000

#include "stdint.h"
#include "efimem.h"

typedef struct _mmap {
	uint64_t nbytes;
	uint8_t buffer[UEFI_MMAP_SIZE];
	uint64_t mapkey;
	uint64_t descsize;
	uint32_t desc_version;
} MEMORY_MAP;

uint64_t getmemsize(EFI_MEMORY_DESCRIPTOR* mmap, uint64_t mmapentries, uint64_t mmapdescsize);

void memzero(void* s, uint64_t n) {
    for (int i = 0; i < n; i++) {
		 ((uint8_t*)s)[i] = 0;
	}
}

void memcpy(void *dest, void *src, size_t n) {  
	uint8_t *pdest = (uint8_t*) dest;
    uint8_t *psrc = (uint8_t*) src;

	for (unsigned int i = 0; i < n; i++) {
   		pdest[i] = psrc[i];  
	}
} 

#endif
