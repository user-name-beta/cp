// cpoffset.h
//
// Copyright (c) 2025-3000 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// The definition of cpoffset_t used in cpm files and related functions.

#ifndef _CP_MODULE_PARSER__CPOFFSET_H_
#define _CP_MODULE_PARSER__CPOFFSET_H_

#include <cptypes.h>

#include <stdint.h>
#include "module.h"

typedef union
{
    uint32_t o32;
    uint64_t o64;
} cpoffset_t;

int CPOffset_VerifyByteMode(CPModule *module);
ssize_t CPOffset_ToSizeT(CPModule *module, cpoffset_t offset);
int CPOffset_FromSize_T(CPModule *module, size_t value, cpoffset_t *offset);
int CPOffset_WriteToMemory(CPModule *module, cpoffset_t offset, void *buffer);
int CPOffset_ReadFromMemory(CPModule *module, const void *buffer, cpoffset_t *offset);
size_t CPOffset_GetSizeByByteMode(CPModule *module);

#endif /* _CP_MODULE_PARSER__CPOFFSET_H_ */