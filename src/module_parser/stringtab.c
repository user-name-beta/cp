// stringtab.c
//
// Copyright (c) 2025-3000 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Parse the string table of a cpm file.

#include "header.h"
#include "stringtab.h"
#include "segmentnum.h"
#include "module.h"

#include <stdint.h>

static inline size_t
get_stringtab_size(CPModule *module, CPModuleStringTable *string_table)
{
    return sizeof(CPModuleStringTable) +
        CPOffset_GetSizeByByteMode(module) *
        string_table->string_count;
}

int
CPModule_ParseStringTable(CPModule *module, CPModuleHeader *header, CPModuleStringTable *out_string_table)
{
    void *seg = CPModule_GetSegment(module, header, CPMODULE_STRING_TABLE_INDEX);
    if(seg == NULL) {
        return -1;
    }
    if((uintptr_t)seg + sizeof(CPModuleStringTable) > (uintptr_t)module->data + module->size) {
        return -1;
    }
    *out_string_table = *(CPModuleStringTable *)seg;
    if((uintptr_t)seg + get_stringtab_size(module, out_string_table) > (uintptr_t)module->data + module->size) {
        return -1;
    }
    return 0;
}

char *
CPModule_GetStringFromStringTable(CPModule *module, CPModuleStringTable *string_table, cpstringtabindex_t index)
{
    if(index >= string_table->string_count) {
        return NULL;
    }
    size_t size = get_stringtab_size(module, string_table);
    size_t segoffset_offset = sizeof(CPModuleStringTable) +
        CPOffset_GetSizeByByteMode(module) * index;
    cpoffset_t seg_offset;
    if(CPOffset_ReadFromMemory(module, (char *)string_table + segoffset_offset, &seg_offset) != 0) {
        return NULL;
    }
    size_t seg_offset_size = CPOffset_ToSizeT(module, seg_offset);
    if(seg_offset_size == (size_t)-1) {
        return NULL;
    }
    return (char *)string_table + size + seg_offset_size;
}

