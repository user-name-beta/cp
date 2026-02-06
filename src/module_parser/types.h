// types.h
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Define the type.

#ifndef _CP_MODULE_PARSER__TYPES_H_
#define _CP_MODULE_PARSER__TYPES_H_

#include "stringtab.h"

typedef uint32_t cpbuiltin_type_t;

#define CP_BUILTIN_TYPE_NONE 0

typedef struct
{
    cpbuiltin_type_t builtin_type; // 0 for non-builtin types
    CPStringIndex module_name; // where the type is defined
    CPStringIndex class_name; // the name of the type
} CPTypeDescriptor;

#endif /* _CP_MODULE_PARSER__TYPES_H_ */