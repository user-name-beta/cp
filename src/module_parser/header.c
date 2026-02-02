// header.c
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Parse the header of a cpm file.

#include "header.h"
#include "cpoffset.h"
#include <version.h>

int
CPModuleParser_ParseHeader(void *data, size_t size, CPModuleHeader *out_header)
{
    if(size < sizeof(CPModuleHeader)) {
        return -1;
    }
    *out_header = *(CPModuleHeader *)data;
    if(size < sizeof(CPModuleHeader) +
        CPOffset_GetSizeByByteMode(out_header->byte_mode) *
        out_header->seg_counts) {
        return -1;
    }
    //if(strncmp(out_header->magic, CP_BYTECODE_MAGIC_NUMBER, CP_BYTECODE_MAGIC_NUMBER_SIZE) != 0) {
    //    return -1;
    //}
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
    if(!CPOffset_IsValidByteMode(out_header->byte_mode)) {
        return -1;
    }
    return 0;
}

void *
CPModuleParser_GetSegment(CPModuleHeader *header, uint8_t index)
{
    if(header->seg_counts <= index) {
        return NULL;
    }
    size_t seg_offset_offset = index * CPOffset_GetSizeByByteMode(header->byte_mode);
    cpoffset_t seg_offset = *(cpoffset_t *)((char *)header->seg_table + seg_offset_offset);
    size_t seg_offset_size = CPOffset_ToSizeT(header->byte_mode, seg_offset);
    if(seg_offset_size == (size_t)-1) {
        return NULL;
    }
    return (void *)((char *)header + seg_offset_size);
}