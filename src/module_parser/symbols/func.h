// func.h
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Define the function symbol.

#ifndef _CP_MODULE_PARSER__SYMBOLS__FUNC_H_
#define _CP_MODULE_PARSER__SYMBOLS__FUNC_H_

#include "../stringtab.h"
#include "../types.h"

typedef uint32_t cpfunctype_t;

#define CP_FUNC_TYPE_METHOD 0

typedef struct
{
    CPStringIndex class_name;
    CPStringIndex func_name;
    cpfunctype_t func_type;
    CPTypeDescriptor return_type;
    uint32_t param_count;
    CPTypeDescriptor param_type[];
} CPModuleFuncSymbol;

#endif /* _CP_MODULE_PARSER__SYMBOLS__FUNC_H_ */
