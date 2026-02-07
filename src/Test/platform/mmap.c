// Test/platform/mmap.c
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Test src/platform/mmap.c

#include <platform/mmap.h>

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#define err (int)GetLastError()
#else
#include <errno.h>
#define err errno
#endif

int
main()
{
    int ret;
    CPMemoryMapping map1;
    CPMemoryMapping map2;
    ret = CPMemoryMapping_Create(&map1, NULL, 4096, 0, CP_MMAP_PROT_WRITE, CP_MMAP_FLAG_SHARED);
    if (ret != 0) {
        printf("Failed to create memory mapping 1: %d\n", err);
        return -1;
    }
    ((char *)map1.addr)[0] = 'A';
    ((char *)map1.addr)[1] = 'B';
    ((char *)map1.addr)[2] = 'C';
    ret = CPMemoryMapping_Protect(&map1, 0, 4096, CP_MMAP_PROT_READ);
    if (ret != 0) {
        printf("Failed to protect memory mapping 1: %d\n", err);
        return -1;
    }
    ret = CPMemoryMapping_Destroy(&map1);
    if (ret != 0) {
        printf("Failed to destroy memory mapping 1: %d\n", err);
        return -1;
    }
    FILE *file = fopen("test_mmap.txt", "wb+");
    if (file == NULL) {
        printf("Failed to open file: %d\n", err);
        return -1;
    }
    if(fwrite("This is test data.\n", 1, 20, file) != 20) {
        printf("Failed to write to file: %d\n", err);
        fclose(file);
        return -1;
    }
    fflush(file);
    ret = CPMemoryMapping_Create(&map2, file, -1, 0, CP_MMAP_PROT_READ | CP_MMAP_PROT_WRITE, CP_MMAP_FLAG_PRIVATE);
    if (ret != 0) {
        printf("Failed to create memory mapping 2: %d\n", err);
        return -1;
    }
    ((char *)map2.addr)[0] = 'D';
    ((char *)map2.addr)[1] = 'E';
    ((char *)map2.addr)[2] = 'F';
    ret = CPMemoryMapping_Protect(&map2, 0, 4096, 0);
    if (ret != 0) {
        printf("Failed to protect memory mapping 2: %d\n", err);
        return -1;
    }
    ret = CPMemoryMapping_Destroy(&map2);
    if (ret != 0) {
        printf("Failed to destroy memory mapping 2: %d\n", err);
        return -1;
    }
    fclose(file);
    return 0;
}
