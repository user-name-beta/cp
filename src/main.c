// main.c
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Implementation of the entry point function CP_Main() for the program.
#define _CP_MAIN_E_

#include "parsearg.h"
#include "cptypes.h"
#include "path.h"
#include "report_error.h"
#include "version.h"

#include "main.h"

static bool main_running = false;

static inline int
get_exe_path(char *dst) {
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
        if(strcpy_s(dst, CP_MAX_PATH, info.dli_fname) != 0)return -1;
    } else {
        return -1;
    }
#endif
    return 0;
}

static inline int
print_file(const char *directory, const char *rel_path) {
    if(directory == NULL || rel_path == NULL) {
        return -1;
    }
    char full_path[CP_MAX_PATH];
    CPath_Join(full_path, directory, rel_path);
    FILE *f = fopen(full_path, "r");
    if(f == NULL) {
        return -1;
    }
    char line[1024];
    while(fgets(line, sizeof(line), f) != NULL) {
        printf("%s", line);
    }
    fclose(f);
    return 0;
}

static inline void
print_copyright(void) {
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

static inline void
print_version(void) {
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
    print_copyright();
}

static inline int
get_home_path(char *dst, const char *exe) {
    char *home = getenv("CPLOCALHOME");
    if(home != NULL) {
        if(strcpy_s(dst, CP_MAX_PATH, home) != 0)return -1;
        return 0;
    }
    if(exe == NULL)return -1;
    if(CPath_Dirname(dst, exe) < 0)return -1;
    return 0;
}

CP_API_FUNC(int)
CP_Main(int argc, char **argv) {
    assert(!main_running && "CP_Main() cannot be called recursively.");
    main_running = true;
    int rv = 1;
    char *exe;
    char *home;
    char exe_name[CP_MAX_PATH];
    char buf1[CP_MAX_PATH];
    if(get_exe_path(buf1) < 0) {
        cp_report_error(NULL, "Cannot get executable path.");
        exe = NULL;
    } else exe = buf1;
    char buf2[CP_MAX_PATH];
    if(get_home_path(buf2, exe) < 0) {
        cp_report_error(exe_name, "Cannot get home directory.");
        home = NULL;
    } else home = buf2;
    if(CPath_Filename(exe_name, exe) < 0)goto error;
    /* Initialize the argument parser. */
    cp_argc = argc - 1;
    cp_argv = argv + 1;
    const char *cmd = CP_ParseOneArg();
    if(cmd == NULL) {
        print_version();
    } else
    if(strcmp(cmd, "copyright") == 0) {
        print_copyright();
    } else
    if(strcmp(cmd, "license") == 0) {
        if(print_file(home, "LICENSE") != 0)goto error;
    } else
    if(strcmp(cmd, "version") == 0) {
        print_version();
    } else {
        cp_report_fatal(exe_name, "Unknown command: %s", cmd);
        goto error;
    }
    rv = 0;
error:
    main_running = false;
    return rv;
}