#pragma once

#include <stdint.h>

typedef struct _gdt_desc {
	uint16_t limit;
	uint64_t offset;
} __attribute__((packed)) GDT_DESCRIPTOR;

typedef struct _gdt_entry {
	uint16_t limit0;
	uint16_t base0;
	uint8_t  base1;
	uint8_t  access_byte;
	uint8_t  limit1_flags;
	uint8_t  base2;
} __attribute__((packed)) GDT_ENTRY;

typedef struct _gdt {
	GDT_ENTRY null;
	GDT_ENTRY kernel_code;
	GDT_ENTRY kernel_data;
	GDT_ENTRY user_null;
	GDT_ENTRY user_code;
	GDT_ENTRY user_data;
} __attribute__((packed)) __attribute__((aligned(0x1000))) GDT;

__attribute__((aligned(0x1000)))
GDT default_gdt = {
	{0, 0, 0, 0x00, 0x00, 0}, 
	{0, 0, 0, 0x9a, 0xa0, 0},
	{0, 0, 0, 0x92, 0xa0, 0},
	{0, 0, 0, 0x00, 0x00, 0},
	{0, 0, 0, 0x9a, 0xa0, 0},
	{0, 0, 0, 0x92, 0xa0, 0},
};

extern void load_gdt(GDT_DESCRIPTOR* gdt_descriptor);