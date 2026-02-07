// safe_string.c
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Safe string functions for C programming language.

#include <stdio.h> // for snprintf()
#include <string.h> // for strlen()
#include <stddef.h> // for size_t

int
strcat_safe(char *dst, const char *src, size_t dst_size)
{
    size_t dst_len = strlen(dst);
    size_t src_len = strlen(src);
    if(dst_len + src_len + 1 > dst_size) {
        return -1;
    }
    int r = snprintf(dst + dst_len, dst_size - dst_len, "%s", src);
    if(r < 0 || r >= (int)(dst_size - dst_len)) {
        return -1;
    }
    return 0;
}

int
strcpy_safe(char *dst, const char *src, size_t dst_size)
{
    size_t src_len = strlen(src);
    if(src_len + 1 > dst_size) {
        return -1;
    }
    int r = snprintf(dst, dst_size, "%s", src);
    if(r < 0 || r >= (int)dst_size) {
        return -1;
    }
    return 0;
}

int
strncat_safe(char *dst, const char *src, size_t dst_size, size_t n)
{
    size_t dst_len = strlen(dst);
    size_t src_len = strlen(src);
    if(n > src_len) {
        n = src_len;
    }
    if(dst_len + n + 1 > dst_size) {
        return -1;
    }
    int r = snprintf(dst + dst_len, dst_size - dst_len, "%.*s", (int)n, src);
    if(r < 0 || r >= (int)(dst_size - dst_len)) {
        return -1;
    }
    return 0;
}

int
strncpy_safe(char *dst, const char *src, size_t dst_size, size_t n)
{
    size_t src_len = strlen(src);
    if(n > src_len) {
        n = src_len;
    }
    if(n + 1 > dst_size) {
        return -1;
    }
    int r = snprintf(dst, dst_size, "%.*s", (int)n, src);
    if(r < 0 || r >= (int)dst_size) {
        return -1;
    }
    return 0;
}
