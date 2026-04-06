/*
 * report_error.c - error reporting functions.
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
