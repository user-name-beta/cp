/*
 * safe_string.c - safe string manipulation functions.
 * Copyright (C) 2025-2026 Huang Jiangyao. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
