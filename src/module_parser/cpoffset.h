// cpoffset.h
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// The definition of cpoffset_t used in cpm files and related functions.

#ifndef _CP_MODULE_PARSER__CPOFFSET_H_
#define _CP_MODULE_PARSER__CPOFFSET_H_

#include <cptypes.h>

#include <stdint.h>

typedef union
{
    uint32_t o32;
    uint64_t o64;
} cpoffset_t;

int CPOffset_IsValidByteMode(uint8_t byte_mode);
ssize_t CPOffset_ToSize_T(uint8_t byte_mode, cpoffset_t offset);
int CPOffset_FromSize_T(uint8_t byte_mode, size_t value, cpoffset_t *offset);
int CPOffset_WriteToMemory(uint8_t byte_mode, cpoffset_t offset, void *buffer, size_t buffer_size);
int CPOffset_ReadFromMemory(uint8_t byte_mode, const void *buffer, size_t buffer_size, cpoffset_t *offset);
size_t CPOffset_GetOffsetByCount(uint8_t byte_mode, size_t count);

#endif /* _CP_MODULE_PARSER__CPOFFSET_H_ */