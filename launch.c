// main.c
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license. 
// See LICENSE file in the project root for full license information.
//
// A placeholder file for the program entry point.

#ifdef _WIN32
__declspec(dllimport)
#endif
int CP_Main(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    return CP_Main(argc, argv);
}