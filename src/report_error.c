// report_error.c
//
// Copyright (c) 2025-9999 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Report error in command-line interface.

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void
cp_report_fatal(const char *exename, const char *fmt, ...)
{
    if(exename != NULL) {
        fprintf(stderr, "%s: ", exename);
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(1);
}

void
cp_report_error(const char *exename, const char *fmt, ...)
{
    if(exename != NULL) {
        fprintf(stderr, "%s: ", exename);
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}