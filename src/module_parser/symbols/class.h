// class.h
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Define the class symbol.

#ifndef _CP_MODULE_PARSER__SYMBOLS__CLASS_H_
#define _CP_MODULE_PARSER__SYMBOLS__CLASS_H_

#include "../stringtab.h"
#include "../cpoffset.h"
#include <stdint.h>

typedef struct
{
    CPStringIndex name;
    cpoffset_t object_size;
    uint32_t attr_start;
    uint32_t attr_end;
} CPModuleClassSymbol;

#endif /* _CP_MODULE_PARSER__SYMBOLS__CLASS_H_ */
