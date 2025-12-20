// main.c
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license. 
// See LICENSE file in the project root for full license information.
//
// A placeholder file for the main function.

#include <stdio.h>
#ifdef _WIN32
__declspec(dllexport)
#else
__attribute__((visibility("default")))
#endif
int
CP_Main(int argc, char* argv[]) {
    printf("Hello, world!\n");
    return 0;
}