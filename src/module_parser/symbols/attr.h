// attr.h
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Define the attribute symbol.

#ifndef _CP_MODULE_PARSER__SYMBOLS__ATTR_H_
#define _CP_MODULE_PARSER__SYMBOLS__ATTR_H_

#include "../stringtab.h"
#include "../cpoffset.h"
#include <objects/types.h>

typedef struct
{
    CPStringIndex class_name;
    CPStringIndex name;
    CPTypeDescriptor type;
    cpoffset_t value_offset;
} CPModuleAttributeSymbol;

#endif /* _CP_MODULE_PARSER__SYMBOLS__ATTR_H_ */
