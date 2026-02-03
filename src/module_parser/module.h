// module_parser.h
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// A header file including other headers in this directory.

#ifndef _CP_MODULE_PARSER__MODULE_H_
#define _CP_MODULE_PARSER__MODULE_H_

#include <stdint.h>

typedef struct
{
    uint8_t byte_mode;
    size_t size;
} CPModule;

#endif /* _CP_MODULE_PARSER__MODULE_H_ */