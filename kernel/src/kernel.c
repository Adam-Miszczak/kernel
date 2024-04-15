#include "gdt.h"

#include "efimem.h"
#include "memory.h"
#include "stdio.h"

typedef struct _bootinfo {
	Framebuffer* framebuffer;
	PSF1_FONT* font;
	unsigned int status;
	EFI_MEMORY_DESCRIPTOR* mmap;
	uint64_t mmapsize;
	uint64_t mmapdescsize;
}BOOTINFO;

void _start(BOOTINFO* bootinfo) {
	uint64_t mmapentries = bootinfo->mmapsize / bootinfo->mmapdescsize;
	uint64_t availablemem = 0;
	uint64_t availablepages = 0;

	GDT_DESCRIPTOR gdt_desc = {sizeof(GDT) - 1, (uint64_t)&default_gdt};

	terminal.framebuffer = bootinfo->framebuffer;
	terminal.font = bootinfo->font;
	
//	asm("cli");
	load_gdt(&gdt_desc);

//	clearbuffer(COLOR_BLUE_DARKER);

	setcolor(COLOR_WHITE);
	printf("Kernel\n\r");
	printf("Exit status: %d\r\n", bootinfo->status);

	/*printf("Memory Size: %d\n\r", getmemsize(bootinfo->mmap, mmapentries, bootinfo->mmapdescsize));

	for (int i = 0; i < mmapentries; i++) {
		setcolor(COLOR_WHITE);
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)bootinfo->mmap + (i * bootinfo->mmapdescsize));
		
		if (desc->type == EFI_CONVENTIONAL_MEMORY) { 
			printf("%s ", EFI_MEMORY_TYPE_STRINGS[desc->type]);
			setcolor(COLOR_MAGENTA);
			printf("%d KB (Addr: 0x%x) (Pages: %d) \n\r", desc->numpages * 4096 / 1024, desc->virtaddr, desc->numpages);
			availablemem += desc->numpages * 4096 / 1024;
			availablepages += desc->numpages;
		}
	}

	printf("Available Memory: %d MB (%d pages)\n\r", availablemem / 1024, availablepages);
*/
	return;	
}
