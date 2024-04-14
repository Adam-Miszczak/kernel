#ifndef _STDIO_H
#define _STDIO_H

#include "stdarg.h"
#include "limits.h"
#include "memory.h"

//#define size_t unsigned long long
#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

#define COLOR_RED     0xffff0000
#define COLOR_GREEN   0xff00ff00
#define COLOR_BLUE    0xff0000ff
#define COLOR_YELLOW  0xffffff00
#define COLOR_MAGENTA 0xffff00ff
#define COLOR_CYAN    0xff00ffff
#define COLOR_WHITE   0xffffffff
#define COLOR_BLACK   0xff000000

#define COLOR_BLUE_DARKER 0xff000066

typedef struct _psf1_header {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;

typedef struct _psf1_font {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

typedef struct _framebuffer{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
} Framebuffer;

typedef struct _term {
	unsigned int cx;
	unsigned int cy;
	Framebuffer* framebuffer;
	PSF1_FONT* font;
	unsigned int color;
}TERMINAL;

int printf(const char* restrict format, ...);

TERMINAL terminal = {0, 0, 0, 0, 0xffffffff};

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void setcolor(unsigned int color) {
	terminal.color = color;
}

void clearbuffer(unsigned int color) {
	unsigned int* pix_ptr = (unsigned int*)terminal.framebuffer->BaseAddress;

	for (unsigned long y = 0; y < terminal.framebuffer->Height; y++) {
		for (unsigned long x = 0; x < terminal.framebuffer->Width; x++) {
			*(unsigned int*)(pix_ptr + x + (y * terminal.framebuffer->PixelsPerScanLine)) = color;
		}
	}
}

void putchar(TERMINAL tt, char ch) {
	unsigned int* pix_ptr = (unsigned int*)tt.framebuffer->BaseAddress;
	char* font_ptr = tt.font->glyphBuffer + (ch * tt.font->psf1_Header->charsize);

	for (unsigned long y = tt.cy; y < tt.cy + 16; y++) {
		for (unsigned long x = tt.cx; x < tt.cx + 8; x++) {
			if ((*font_ptr & (0b10000000 >> (x - tt.cx))) > 0) {
				*(unsigned int*)(pix_ptr + x + (y * tt.framebuffer->PixelsPerScanLine)) = tt.color;
			}
		}

		font_ptr++;
	}
}

void prntnum(unsigned long num, int base, char sign, char *outbuf) {

    int i = 12;
    int j = 0;

    do{
        outbuf[i] = "0123456789ABCDEF"[num % base];
        i--;
        num = num/base;
    }while( num > 0);
//dsddd
/*    if(sign != ' '){
        outbuf[0] = sign;
        ++j;
    }
*/
    while( ++i < 13){
       outbuf[j++] = outbuf[i];
    }

    outbuf[j] = 0;

}

char* itostr(char *dest, size_t size, int a, int base) {
  // Max text needs occur with itostr(dest, size, INT_MIN, 2)
  char buffer[sizeof a * CHAR_BIT + 1 + 1]; 
  static const char digits[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  if (base < 2 || base > 36) {
	 setcolor(COLOR_RED);
    printf("itostr: Invalid base\n\r");
    return NULL;
  }

  // Start filling from the end
  char* p = &buffer[sizeof buffer - 1];
  *p = '\0';

  // Work with negative `int`
  int an = a < 0 ? a : -a;  

  do {
    *(--p) = digits[-(an % base)];
    an /= base;
  } while (an);

  if (a < 0) {
    *(--p) = '-';
  }

  size_t size_used = &buffer[sizeof(buffer)] - p;
  if (size_used > size) {
	 setcolor(COLOR_RED);
    printf("itostr: Scant buffer su > s", size_used , size);
    return NULL;
  }

  memcpy(dest, p, size_used);
  return dest;
}

int print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++) {
		if(bytes[i] == '\n') {
			terminal.cy += 16;
		} else if (bytes[i] == '\r') {
			terminal.cx = 0;
		} else {
			putchar(terminal, bytes[i]);
			terminal.cx += 8;
		}
	}

	return 1;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;

		// prntnum works for both bases so reduce X and D to one if-statement
		if (*format == 'x') {
			format++;
			int c = (int) va_arg(parameters, int);
			char buf[256];
			prntnum(c, 16, 1, buf);
			size_t len = strlen(buf);
			if (!print(buf, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
 			format++;
			int c = (int) va_arg(parameters, int);
			char buf[256];
			itostr(buf, 256, c, 10);
			size_t len = strlen(buf);
			if (!print(buf, len))
				return -1;
			written += len;
		} else if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);
	return written;
}

#endif
