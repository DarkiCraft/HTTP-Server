#include "common.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

const size_t PORT = 8080;
const size_t MAX_PENDING_CONNECTIONS = 10;
const size_t BUFFER_SIZE = 1024;

volatile sig_atomic_t is_server_running = 0;

size_t my_strnlen(const char* s, size_t maxlen) {
	if (s == NULL) {
		return 0;
	}

	const char* p = (const char*)memchr(s, '\0', maxlen);
	return p ? (size_t)(p - s) : maxlen;
}

void* my_memset(void* dest, int value, size_t n) {
	if (dest == NULL) {
		return NULL;
	}

	unsigned char* d = dest;
	volatile unsigned char* vd = d;

	while (n--) {
		*vd++ = (unsigned char)value;
	}

	return dest;
}

void* my_memcpy(void* dest, const void* src, size_t n) {
	if (dest == NULL || src == NULL) {
		return NULL;
	}

	if (n == 0) {
		return dest;
	}

	unsigned char* d = dest;
	const unsigned char* s = src;

	volatile unsigned char* vd = d;
	volatile const unsigned char* vs = s;

	if (vd > vs && vd < vs + n) {
		vd += n;
		vs += n;
		while (n--) {
			*(--vd) = *(--vs);
		}
	} else {
		while (n--) {
			*vd++ = *vs++;
		}
	}

	return dest;
}

char* my_strdup(const char* s) {
	if (s == NULL) {
		return NULL;
	}

	size_t len = strlen(s) + 1;
	char* dup = malloc(len);
	if (dup == NULL) {
		return NULL;
	}

	my_memcpy(dup, s, len);

	return dup;
}

char* my_strndup(const char* s, size_t n) {
	size_t len = my_strnlen(s, n);
	char* new_str = malloc(len + 1);
	if (new_str == NULL) {
		return NULL;
	}
	my_memcpy(new_str, s, len);
	new_str[len] = '\0';
	return new_str;
}

// src/common.c