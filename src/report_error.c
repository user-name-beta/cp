// report_error.c
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Report error in command-line interface.

#include "path.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

char cp_exename[CP_MAX_PATH] = {0};

void
cp_report_fatal(const char *fmt, ...)
{
    if(cp_exename[0] != '\0') {
        fprintf(stderr, "%s: ", cp_exename);
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(1);
}

void
cp_report_error(const char *fmt, ...)
{
    if(cp_exename[0] != '\0') {
        fprintf(stderr, "%s: ", cp_exename);
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}
