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
cp_report_error_v(const char *fmt, va_list args)
{
    if(cp_exename[0] != '\0') {
        fprintf(stderr, "%s: ", cp_exename);
    }
    vfprintf(stderr, fmt, args);
}

void
cp_report_fatal_v(const char *fmt, va_list args)
{
    cp_report_error_v(fmt, args);
    exit(1);
}

void
cp_report_error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    cp_report_error_v(fmt, args);
    va_end(args);
}

void
cp_report_fatal(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    cp_report_fatal_v(fmt, args);
    va_end(args);
}