/*
 * commandline.c - helper functions for the program.
 * Copyright (C) 2025-2026 Huang Jiangyao. All rights reserved.
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

#include "config.h"
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
CPCommandLine_GetExecutablePath(char *dst)
{
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

void
CPCommandLine_PrintCopyright(void)
{
    printf("Copyright (c) 2025-2026 user-name-beta. All rights reserved.\n");
    printf("Licensed under the MIT license.\n");
    printf("See LICENSE file in the software root for full license information.\n");
    printf("\n");
    printf("If you want more information, please visit <https://github.com/user-name-beta/cp>\n");
}

void
CPCommandLine_PrintVersion(void)
{
    printf("CP version %s\n", CP_VERSION_STRING);
#if defined(__GNUC__)
    printf("Compiler: GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
    printf("Compiler: unknown\n");
#endif
    printf("\n");
    CPCommandLine_PrintCopyright();
}

int
CPCommandLine_GetHomeDirectory(char *dst, const char *exe)
{
    char *home = getenv("CPLOCALHOME");
    if(home != NULL) {
        if(strcpy_safe(dst, home, CP_MAX_PATH) != 0)return -1;
        return 0;
    }
    if(exe == NULL)return -1;
    if(CPath_Dirname(dst, exe) < 0)return -1;
    return 0;
}
