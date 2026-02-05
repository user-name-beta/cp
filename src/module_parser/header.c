// header.c
//
// Copyright (c) 2025-3000 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Parse the header of a cpm file.

#include "header.h"
#include "cpoffset.h"
#include <version.h>

static size_t
get_header_size(CPModule *module, CPModuleHeader *header) {
    size_t cpoffset_size = CPOffset_GetSizeByByteMode(module);
    if(cpoffset_size == (size_t)-1) {
        return (size_t)-1;
    }
    return sizeof(CPModuleHeader) +
        cpoffset_size * header->seg_counts;
}

int
CPModule_ParseHeader(CPModule *module, CPModuleHeader *out_header)
{
    if(module->size < sizeof(CPModuleHeader)) {
        return -1;
    }
    *out_header = *(CPModuleHeader *)module->data;
    if(!CPOffset_VerifyByteMode(module)) {
        return -1;
    }
    size_t header_size = get_header_size(module, out_header);
    if(header_size == (size_t)-1) {
        return -1;
    }
    if(module->size < header_size) {
        return -1;
    }
    if(memcmp(out_header->magic, CP_BYTECODE_MAGIC_NUMBER, CP_BYTECODE_MAGIC_NUMBER_SIZE) != 0) {
        return -1;
    }
    if(out_header->major != CP_BYTECODE_VERSION_MAJOR // ||
//     out_header->minor < CP_BYTECODE_VERSION_MINOR) {
// CP_BYTECODE_VERSION_MINOR is zero, so it will cause 
// a compilation warning about comparison being always true.
// We should avoid this warning.
    ) {
        return -1;
    }
    module->byte_mode = out_header->byte_mode;
    return 0;
}

void *
CPModule_GetSegment(CPModule *module, CPModuleHeader *header, uint8_t index)
{
    if(header->seg_counts <= index) {
        return NULL;
    }
    size_t cpoffset_size = CPOffset_GetSizeByByteMode(module);
    if(cpoffset_size == (size_t)-1) {
        return NULL;
    }
    size_t seg_offset_offset = index * CPOffset_GetSizeByByteMode(module);
    cpoffset_t seg_offset = *(cpoffset_t *)((char *)header->seg_table + seg_offset_offset);
    size_t seg_offset_size = CPOffset_ToSizeT(module, seg_offset);
    if(seg_offset_size == (size_t)-1) {
        return NULL;
    }
    return (void *)((char *)header + get_header_size(module, header) + seg_offset_size);
}