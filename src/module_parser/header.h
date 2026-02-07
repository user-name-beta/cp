// header.h
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Parse the header of a cpm file.

#ifndef _CP_MODULE_PARSER__HEADER_H_
#define _CP_MODULE_PARSER__HEADER_H_

#include <stdint.h>
#include <stddef.h>
#include <cpassert.h>

#include "cpoffset.h"

#ifdef _MSV_VER
__pragma(pack(push, 1))
#endif

typedef struct
#ifdef __GNUC__
__attribute__((packed))
#endif
{
    char magic[4];          // Magic number to identify cpm files, offset 0
    uint32_t major;         // Major version number, offset 4
    uint32_t minor;         // Minor version number, offset 8
    uint8_t byte_mode;      // bytecode identifying 64-bit or 32-bit mode, now always 32. Offset 12
    uint8_t seg_counts;     // Number of segmentations, offset 13
    char _pending1[2];      // Padding for alignment, offset 14
    cpoffset_t seg_table[]; // Offsets to each segment, offset 16.
} CPModuleHeader;

#ifdef _MSV_VER
__pragma(pack(pop))
#endif

static_assert(offsetof(CPModuleHeader, magic) == 0, "Offset of magic is incorrect");
static_assert(offsetof(CPModuleHeader, major) == 4, "Offset of major is incorrect");
static_assert(offsetof(CPModuleHeader, minor) == 8, "Offset of minor is incorrect");
static_assert(offsetof(CPModuleHeader, byte_mode) == 12, "Offset of byte_mode is incorrect");
static_assert(offsetof(CPModuleHeader, seg_counts) == 13, "Offset of seg_counts is incorrect");
static_assert(offsetof(CPModuleHeader, _pending1) == 14, "Offset of _pending1 is incorrect");
static_assert(offsetof(CPModuleHeader, seg_table) == 16, "Offset of seg_table is incorrect");
static_assert(sizeof(CPModuleHeader) == 16, "Size of CPModuleHeader is incorrect");

#ifdef __cplusplus
extern "C" {
#endif

int CPModule_ParseHeader(CPModule *module, CPModuleHeader *out_header);
void *CPModule_GetSegment(CPModule *module, CPModuleHeader *header, uint8_t index);

#ifdef __cplusplus
}
#endif

#endif /* _CP_MODULE_PARSER__HEADER_H_ */