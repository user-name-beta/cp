// safe_string.c
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Safe string functions for C programming language.

#ifndef _CP_SAFE_STRING_H_
#define _CP_SAFE_STRING_H_

#include <stddef.h>

int strcat_safe(char *dst, const char *src, size_t dst_size);
int strcpy_safe(char *dst, const char *src, size_t dst_size);
int strncat_safe(char *dst, const char *src, size_t dst_size, size_t n);
int strncpy_safe(char *dst, const char *src, size_t dst_size, size_t n);

#endif /* _CP_SAFE_STRING_H_ */