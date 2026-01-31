// commandline.c
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Functions for simple commands like --help and --version.

#include "commandline.h"
#include "path.h"
#include "version.h"
#include "safe_string.h"
#include "report_error.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#endif

#include <stdio.h>
#include <stdlib.h>

int
CPCommandLine_GetExecutablePath(char *dst) {
#ifdef _WIN32
    size_t len = GetModuleFileNameA(NULL, dst, CP_MAX_PATH);
    if(len == 0 || len >= CP_MAX_PATH) {
        return -1;
    }
#elif defined(__linux__) || defined(__APPLE__)
    if(readlink("/proc/self/exe", dst, CP_MAX_PATH) == -1) {
        return -1;
    }
#else
    Dl_info info;
    if(dladdr((void *)CP_Main, &info)) {
        if(strcpy_safe(dst, info.dli_fname, CP_MAX_PATH) != 0)return -1;
    } else {
        return -1;
    }
#endif
    return 0;
}

int
CPCommandLine_PrintFile(const char *directory, const char *rel_path){
    if(directory == NULL || rel_path == NULL) {
        return -1;
    }
    char full_path[CP_MAX_PATH];
    CPath_Join(full_path, directory, rel_path);
    FILE *f = fopen(full_path, "r");
    if(f == NULL) {
        cp_report_error(NULL, "Cannot open file: %s", full_path);
        return -1;
    }
    char line[1024];
    while(fgets(line, sizeof(line), f) != NULL) {
        printf("%s", line);
    }
    fclose(f);
    return 0;
}

void
CPCommandLine_PrintCopyright(void) {
    printf("Copyright (c) 2025 user-name-beta. All rights reserved.\n");
    printf("Licensed under the MIT license.\n");
    printf("See LICENSE file in the software root for full license information.\n");
    printf("\n");
    printf("CP is free-software. It is distributed in the hope that it will be useful,\n");
    printf("but WITHOUT WARRANTY; without even the implied warranty of\n");
    printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
    printf("\n");
    printf("If you wants more information, please visit https://github.com/user-name-beta/cp\n");
}

void
CPCommandLine_PrintVersion(void) {
    printf("CP version %s\n", CP_VERSION_STRING);
    printf("Build date: %s %s\n", __DATE__, __TIME__);
#ifdef _MSC_VER
    printf("Compiler: Microsoft Visual Studio %d.%d\n", _MSC_VER / 100, _MSC_VER % 100);
#elif defined(__GNUC__)
    printf("Compiler: GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
    printf("Compiler: unknown\n");
#endif
    printf("\n");
    CPCommandLine_PrintCopyright();
}

int
CPCommandLine_GetHomeDirectory(char *dst, const char *exe) {
    char *home = getenv("CPLOCALHOME");
    if(home != NULL) {
        if(strcpy_safe(dst, home, CP_MAX_PATH) != 0)return -1;
        return 0;
    }
    if(exe == NULL)return -1;
    if(CPath_Dirname(dst, exe) < 0)return -1;
    return 0;
}

int
CPCommandLine_PrintLicense(const char *home) {
    return CPCommandLine_PrintFile(home, "LICENSE");
}

int
CPCommandLine_PrintHelp(const char *home, const char *helpfile) {
    return CPCommandLine_PrintFile(home, helpfile);
}