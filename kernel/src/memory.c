#include "memory.h"

uint64_t getmemsize(EFI_MEMORY_DESCRIPTOR* mmap, uint64_t mmapentries, uint64_t mmapdescsize) {
	static uint64_t memsizebytes = 0;
	if (memsizebytes > 0) return memsizebytes;
	
	for (int i = 0; i < mmapentries; i++) {
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mmap + (i * mmapdescsize));
		memsizebytes += desc->numpages * 4096;
	}

	return memsizebytes;
}