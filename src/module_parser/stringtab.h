// stringtab.h
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Parse the string table of a cpm file.

#ifndef _CP_MODULE_PARSER__STRINGTAB_H_
#define _CP_MODULE_PARSER__STRINGTAB_H_

#include <stdint.h>
#include "cpoffset.h"
#include "header.h"

typedef uint32_t cpstringtabindex_t;

typedef struct
{
    cpstringtabindex_t string_count;
    cpoffset_t strings_offset[];
} CPModuleStringTable;

#ifdef __cplusplus
extern "C" {
#endif

int CPModule_ParseStringTable(CPModule *module, CPModuleHeader *header, CPModuleStringTable *out_string_table);
char *CPModule_GetStringFromStringTable(CPModule *module, CPModuleStringTable *string_table, cpstringtabindex_t index);

#ifdef __cplusplus
}
#endif

#endif /* _CP_MODULE_PARSER__STRINGTAB_H_ */