// main.c
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Implementation of the entry point function CP_Main() for the program.
#define _CP_CPC__MAIN_E_

#include <parsearg.h>
#include <path.h>
#include <report_error.h>
#include <commandline.h>

#include "main.h"

#include <stddef.h>

static int main_running = 0;

static char *exe = NULL;
static char *home = NULL;
static char buf1[CP_MAX_PATH];
static char buf2[CP_MAX_PATH];
static char exe_name[CP_MAX_PATH];

static int init_program_path(void)
{
    if(CPCommandLine_GetExecutablePath(buf1) == 0) {
        exe = buf1;
    } else {
        cp_report_error(NULL, "Cannot get executable path.");
        return -1;
    }
    if(CPCommandLine_GetHomeDirectory(buf2, exe) == 0) {
        home = buf2;
    } else {
        cp_report_error(exe_name, "Cannot get home directory.");
        return -1;
    }
    if(CPath_Filename(exe_name, exe) < 0) {
        cp_report_error(exe_name, "Cannot get executable name.");
        return -1;
    }
    return 0;
}

CP_API_FUNC(int)
CPMainProgramEntryPoint_CPC(int argc, char **argv)
{
    if(main_running) {
        cp_report_fatal("cpc", "Recursive call to main entry point.");
        return -1;
    }
    main_running = 1;
    int rv = 1;
    /* Initialize the argument parser. */
    cp_argc = argc - 1;
    cp_argv = argv + 1;
    if(init_program_path() < 0) {
        goto error;
    }
    if(CP_ParseFlag("--copyright") == 1) {
        CPCommandLine_PrintCopyright();goto end;
    }
    if(CP_ParseFlag("--version") == 1) {
        CPCommandLine_PrintVersion();goto end;
    }
    if(CP_ParseFlag("--license") == 1) {
        CPCommandLine_PrintLicense(home);goto end;
    }
    if(CP_ParseFlag("--help") == 1) {
        CPCommandLine_PrintHelp(home, "cpc-help.txt");goto end;
    }
    CPCommandLine_PrintHelp(home, "cpc-help.txt");
    goto error;
end:
    if(CP_ParseAssertNoMoreArgs() < 0) {
        CPCommandLine_PrintHelp(home, "cpc-help.txt");
        goto error;
    }
    rv = 0;
error:
    main_running = 0;
    return rv;
}
