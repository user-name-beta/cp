/*
 * cptypes.h - common types and macros.
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

#ifndef _CP_CPTYPES_H_
#define _CP_CPTYPES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef _WIN32
#include <windows.h>
typedef SSIZE_T ssize_t;
#else
#include <unistd.h>
#endif

#ifdef __GNUC__
#define CP_ALWYAS_INLINE __attribute__((always_inline)) inline
#else
#define CP_ALWYAS_INLINE inline
#endif

#define CP_UNUSED(x) (void)(x)

#ifdef __GNUC__
#define CP_UNREACHABLE() __builtin_unreachable()
#else
#define CP_UNREACHABLE() do {
    assert(0 && "Unreachable code reached");
    abort();
} while (0)
#endif

#endif /* _CP_CPTYPES_H_ */
