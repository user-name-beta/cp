// cptypes.h
//
// Copyright (c) 2025-9999 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Provide with necessary C standard and other custom 
// types and macros and functions.

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
#elif defined(_MSC_VER)
#define CP_ALWYAS_INLINE __forceinline inline
#else
#define CP_ALWYAS_INLINE inline
#endif

#define CP_UNUSED(x) (void)(x)

#ifdef __GNUC__
#define CP_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define CP_UNREACHABLE() __assume(0)
#else
#define CP_UNREACHABLE() do {
    assert(0 && "Unreachable code reached");
    abort();
} while (0)
#endif

#endif /* _CP_CPTYPES_H_ */