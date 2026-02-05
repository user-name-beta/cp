// cpoffset.c
//
// Copyright (c) 2025-3000 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// cpoffset_t-related functions.

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#include "cpoffset.h"
#include "module.h"
#include <cptypes.h>

int
CPOffset_VerifyByteMode(CPModule *module)
{
    return (module->byte_mode == 0 || module->byte_mode == 1) ? 1 : 0;
}

ssize_t
CPOffset_ToSizeT(CPModule *module, cpoffset_t offset)
{
#if SIZE_MAX == UINT64_MAX
    if(module->byte_mode == 0) { // 32-bit mode
        return (ssize_t)(offset.o32);
    } else if(module->byte_mode == 1) {  // 64-bit mode
        return (ssize_t)(offset.o64);
    } else {
        return -2;
    }
#elif SIZE_MAX == UINT32_MAX
    if(module->byte_mode == 0) {  // 32-bit mode
        return (ssize_t)(offset.o32);
    } else if(module->byte_mode == 1) {  // 64-bit mode
        if(offset.o64 > UINT32_MAX) {
            return -1;
        }
        return (ssize_t)(offset.o64);
    } else {
        return -2;
    }
#else
    #error "Unsupported size_t size"
#endif
}

int
CPOffset_FromSize_T(CPModule *module, size_t value, cpoffset_t *offset)
{
#if SIZE_MAX == UINT64_MAX
    if(module->byte_mode == 0) {  // 32-bit mode
        if(value > UINT32_MAX) {
            return -1;
        }
        offset->o32 = (uint32_t)value;
        return 0;
    } else if(module->byte_mode == 1) {  // 64-bit mode
        offset->o64 = (uint64_t)value;
        return 0;
    } else {
        return -2;
    }
#elif SIZE_MAX == UINT32_MAX
    if(module->byte_mode == 0) {  // 32-bit mode
        offset->o32 = (uint32_t)value;
        return 0;
    } else if(module->byte_mode == 1) {  // 64-bit mode
        offset->o64 = (uint64_t)value;
        return 0;
    } else {
        return -2;
    }
#else
    #error "Unsupported size_t size"
#endif
}

int
CPOffset_ReadFromMemory(CPModule *module, const void *buffer, cpoffset_t *offset)
{
    if(module->byte_mode == 0) {  // 32-bit mode
        if((uintptr_t)buffer + sizeof(uint32_t) > (uintptr_t)module->data + module->size) {
            return -1;
        }
        offset->o32 = *((const uint32_t *)buffer);
        return 0;
    } else if(module->byte_mode == 1) {  // 64-bit mode
        if((uintptr_t)buffer + sizeof(uint64_t) > (uintptr_t)module->data + module->size) {
            return -1;
        }
        offset->o64 = *((const uint64_t *)buffer);
        return 0;
    } else {
        return -2;
    }
}

int
CPOffset_WriteToMemory(CPModule *module, cpoffset_t offset, void *buffer)
{
    if(module->byte_mode == 0) {  // 32-bit mode
        if((uintptr_t)buffer + sizeof(uint32_t) > (uintptr_t)module->data + module->size) {
            return -1;
        }
        *((uint32_t *)buffer) = offset.o32;
        return 0;
    } else if(module->byte_mode == 1) {  // 64-bit mode
        if((uintptr_t)buffer + sizeof(uint64_t) > (uintptr_t)module->data + module->size) {
            return -1;
        }
        *((uint64_t *)buffer) = offset.o64;
        return 0;
    } else {
        return -2;
    }
}

size_t
CPOffset_GetSizeByByteMode(CPModule *module)
{
    if(module->byte_mode == 0) {  // 32-bit mode
        return sizeof(uint32_t);
    } else if(module->byte_mode == 1) {  // 64-bit mode
        return sizeof(uint64_t);
    } else {
        return (size_t)-1;
    }
}