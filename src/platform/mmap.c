// mmap.c
//
// Copyright (c) 2025-3000 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Memory mapping functions for cross-platform compatibility.

#define _POSIX_C_SOURCE 200112L

#include "mmap.h"

#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif

#ifdef _WIN32
typedef HANDLE file_t;
#else
typedef int file_t;
#endif

#ifdef _WIN32
typedef DWORD prot_t;
#else
typedef int prot_t;
#endif

#ifdef _WIN32
typedef DWORD flags_t;
#else
typedef int flags_t;
#endif

static inline file_t
convert_file_to_handle_or_fd(FILE *file)
{
#ifdef _WIN32
    if(file == NULL)return INVALID_HANDLE_VALUE;
    return (HANDLE) _get_osfhandle(_fileno(file));
#else
    if(file == NULL)return -1;
    return fileno(file);
#endif
}

static inline size_t
convert_size(file_t file, size_t size)
{
    if(size != (size_t)-1)  {
        return size;
    }
#ifdef _WIN32
    LARGE_INTEGER fileSize;
    if(!GetFileSizeEx(file, &fileSize)) {
        return (size_t)-1;
    }
    return (size_t)fileSize.QuadPart;
#else
    struct stat st;
    if(fstat(file, &st) != 0) {
        return (size_t)-1;
    }
    return (size_t)st.st_size;
#endif
}

static inline prot_t
convert_prot(int prot, int flags)
{
#ifdef _WIN32
    int r = prot & CP_MMAP_PROT_READ;
    int w = prot & CP_MMAP_PROT_WRITE;
    int x = prot & CP_MMAP_PROT_EXEC;
    int s = flags & CP_MMAP_FLAG_SHARED || (flags & CP_MMAP_FLAG_PRIVATE) == 0;
    if(!(r || w || x)) return PAGE_NOACCESS;
    if(r && !w && !x) return PAGE_READONLY;
    if(!r && w && !x) return s ? PAGE_READWRITE : PAGE_WRITECOPY;
    if(r && w && !x) return s ? PAGE_READWRITE : PAGE_WRITECOPY;
    if(!r && !w && x) return PAGE_EXECUTE;
    if(r && !w && x) return PAGE_EXECUTE_READ;
    if(!r && w && x) return s ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_WRITECOPY;
    if(r && w && x) return s ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_WRITECOPY;
    assert(0 && "Invalid combination of flags and prot");
    return 0; // To avoid compiler warning: not all control paths return a value
#else
    (void)flags; // unused
    int f = 0;
    if(prot & CP_MMAP_PROT_READ) {
        f |= PROT_READ;
    }
    if(prot & CP_MMAP_PROT_WRITE) {
        f |= PROT_WRITE;
    }
    if(prot & CP_MMAP_PROT_EXEC) {
        f |= PROT_EXEC;
    }
    return f;
#endif
}

static inline flags_t
convert_flags(file_t file, prot_t p, int flags)
{
#ifdef _WIN32
    (void)file; // unused
    (void)flags; // unused
    switch (p) {
        case PAGE_NOACCESS:
            return 0;
        case PAGE_READONLY:
            return FILE_MAP_READ;
        case PAGE_READWRITE:
            return FILE_MAP_ALL_ACCESS;
        case PAGE_WRITECOPY:
            return FILE_MAP_COPY;
        case PAGE_EXECUTE:
            return FILE_MAP_EXECUTE;
        case PAGE_EXECUTE_READ:
            return FILE_MAP_READ | FILE_MAP_EXECUTE;
        case PAGE_EXECUTE_READWRITE:
            return FILE_MAP_ALL_ACCESS | FILE_MAP_EXECUTE;
        case PAGE_EXECUTE_WRITECOPY:
            return FILE_MAP_COPY | FILE_MAP_EXECUTE;
        default:
            return FILE_MAP_READ;
    }
#else
    (void)p; // unused
    int mmap_flags = 0;
    if(flags & CP_MMAP_FLAG_SHARED) {
        mmap_flags |= MAP_SHARED;
    } else
    if(flags & CP_MMAP_FLAG_PRIVATE) {
        mmap_flags |= MAP_PRIVATE;
    }
    if(file == -1){
        mmap_flags |= MAP_ANONYMOUS;
    }
    return mmap_flags;
#endif
}

int 
CPMemoryMapping_Create(CPMemoryMapping *mapping, FILE *file, size_t size, size_t offset, int prot, int flags)
{
    file_t handle = convert_file_to_handle_or_fd(file);
    size = convert_size(handle, size);
    if(size == (size_t)-1) {
        return -1;
    }
    mapping->size = size;
    if(offset > size) {
        return -1;
    }
    prot_t p = convert_prot(prot, flags);
    flags_t f = convert_flags(handle, p, flags);
#ifdef _WIN32
  #ifdef _WIN64
    mapping->hMapping = CreateFileMappingA(handle, NULL, p, size >> 32, (DWORD)size, NULL);
  #else /* _WIN64 */
    mapping->hMapping = CreateFileMappingA(handle, NULL, p, 0, (DWORD)size, NULL);
  #endif /* _WIN64 */
    if(mapping->hMapping == NULL || mapping->hMapping == INVALID_HANDLE_VALUE) {
        return -1;
    }
  #ifdef _WIN64
    mapping->addr = MapViewOfFile(mapping->hMapping, f, offset >> 32, (DWORD)offset, size);
  #else /* _WIN64 */
    mapping->addr = MapViewOfFile(mapping->hMapping, f, 0, offset, size);
  #endif /* _WIN64 */
    if(mapping->addr == NULL) {
        CloseHandle(mapping->hMapping);
        return -1;
    }
    return 0;
#else /* _WIN32 */
    mapping->addr = mmap(NULL, size, p, f, handle, offset);
    if(mapping->addr == MAP_FAILED) {
        return -1;
    }
    return 0;
#endif
}

int CPMemoryMapping_Protect(CPMemoryMapping *mapping, size_t offset, size_t size, int prot)
{
    if(size == (size_t)-1) {
        size = mapping->size;
    }
#ifdef _WIN32
    DWORD dwOldProtect;
    if(!VirtualProtect((char *)mapping->addr + offset, size, convert_prot(prot, 0), &dwOldProtect)) {
        return -1;
    }
#else
    if(mprotect((char *)mapping->addr + offset, size, convert_prot(prot, 0)) != 0) {
        return -1;
    }
#endif
    return 0;
}

int CPMemoryMapping_Destroy(CPMemoryMapping *mapping)
{
#ifdef _WIN32
    UnmapViewOfFile(mapping->addr);
    CloseHandle(mapping->hMapping);
#else
    munmap(mapping->addr, mapping->size);
#endif
    return 0;
}