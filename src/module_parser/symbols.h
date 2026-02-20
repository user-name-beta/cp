// symbols.h
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// The set of all the symbol headers

#ifndef _CP_MODULE_PARSER__SYMBOLS_H_
#define _CP_MODULE_PARSER__SYMBOLS_H_

#include "symbols/attr.h"
#include "symbols/class.h"
#include "symbols/func.h"

#define CP_SYMBOLS_TYPE_ATTRIBUTE 1
#define CP_SYMBOLS_TYPE_CLASS 2
#define CP_SYMBOLS_TYPE_FUNC 3

typedef struct
{
    uint32_t symbol_type;
    union
    {
        CPModuleAttributeSymbol attr;
        CPModuleClassSymbol class_;
        CPModuleFuncSymbol func;
    };
} CPModuleSymbol;

#endif /* _CP_MODULE_PARSER__SYMBOLS_H_ */
