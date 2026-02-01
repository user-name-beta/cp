// mmap.h
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Memory mapping functions for cross-platform compatibility.

#ifndef _CP_MMAP_H_
#define _CP_MMAP_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

#include <stdint.h>
#include <stdio.h>

typedef struct
{
    void *addr;
    size_t size;
#ifdef _WIN32
    HANDLE hMapping;
#endif
} CPMemoryMapping;

#define CP_MMAP_PROT_NONE 0b000
#define CP_MMAP_PROT_READ 0b001
#define CP_MMAP_PROT_WRITE 0b010
#define CP_MMAP_PROT_EXEC 0b100

#define CP_MMAP_FLAG_SHARED 0b01
#define CP_MMAP_FLAG_PRIVATE 0b10

int CPMemoryMapping_Create(CPMemoryMapping *mapping, FILE *file, size_t size, size_t offset, int prot, int flags);
int CPMemoryMapping_Protect(CPMemoryMapping *mapping, size_t offset, size_t size, int prot);
int CPMemoryMapping_Destroy(CPMemoryMapping *mapping);

#endif /* _CP_MMAP_H_ */