/*
 * mmap.c - test memory mapping functionality.
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
