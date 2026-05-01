/*
 * mmap.c - cross-platform memory mapping.
 * Copyright (C) 2026 Huang Jiangyao. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
