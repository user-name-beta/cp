// Test/platform/mmap.c
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Test src/platform/mmap.c

#include <platform/mmap.h>

#include <stdio.h>

int
main() {
    int ret;
    CPMemoryMapping map1;
    CPMemoryMapping map2;
    ret = CPMemoryMapping_Create(&map1, NULL, 4096, 0, CP_MMAP_PROT_READ, CP_MMAP_FLAG_SHARED);
    if (ret != 0) {
        printf("Failed to create memory mapping 1\n");
        return -1;
    }
    (char *)map1.addr[0] = 'A';
    (char *)map1.addr[1] = 'B';
    (char *)map1.addr[2] = 'C';
    ret = CPMemoryMapping_Protect(&map1, 0, 4096, CP_MMAP_PROT_READ | CP_MMAP_PROT_WRITE);
    if (ret != 0) {
        printf("Failed to protect memory mapping 1\n");
        return -1;
    }
    ret = CPMemoryMapping_Destroy(&map1);
    if (ret != 0) {
        printf("Failed to destroy memory mapping 1\n");
        return -1;
    }
    FILE *file = fopen("test_mmap.txt", "wb+");
    if (file == NULL) {
        printf("Failed to open file\n");
        return -1;
    }
    ret = CPMemoryMapping_Create(&map2, file, 4096, 0, CP_MMAP_PROT_READ | CP_MMAP_PROT_WRITE, CP_MMAP_FLAG_PRIVATE);
    if (ret != 0) {
        printf("Failed to create memory mapping 2\n");
        return -1;
    }
    (char *)map2.addr[0] = 'D';
    (char *)map2.addr[1] = 'E';
    (char *)map2.addr[2] = 'F';
    ret = CPMemoryMapping_Protect(&map2, 0, 4096, 0);
    if (ret != 0) {
        printf("Failed to protect memory mapping 2\n");
        return -1;
    }
    ret = CPMemoryMapping_Destroy(&map2);
    if (ret != 0) {
        printf("Failed to destroy memory mapping 2\n");
        return -1;
    }
    fclose(file);
    return 0;
}