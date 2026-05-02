/*
 * parsearg.c - command line argument parse.
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
#include <stdio.h>
#include <string.h>
#include "cptypes.h"
#include "report_error.h"
#include "safe_string.h"
#include "cpassert.h"

int cp_argc;
char **cp_argv;

static inline void
pop(int index)
{
    for(int i = index; i < cp_argc - 1; i++) {
        cp_argv[i] = cp_argv[i+1];
    }
    cp_argc--;
}

int
CP_ParseAssertNoMoreArgs(void)
{
    if(cp_argc > 0) {
        fprintf(stderr, "Error: extra arguments: %s\n", cp_argv[0]);
        return -1;
    }
    return 0;
}

const char *
CP_ParseOneArg(void)
{
    /* Parse next argument */
    if(cp_argc == 0) {
        return NULL;
    }
    char *arg = cp_argv[0];
    pop(0);
    return arg;
}

int
CP_ParseFlag(const char *flag)
{
    /* Parse --flag or -f */
    int found = 0;
    for(int i = 0; i < cp_argc; i++) {
        if(strcmp(cp_argv[i], flag) == 0){
            pop(i);
            i--;
            found = 1;
        }
    }
    return found;
}

int
CP_ParseFlagEx(int flagc, const char * const *flags)
{
    /* Parse exclusive multiple flags */
    int rv = -1;
    for(int i = 0; i < cp_argc; i++) {
        for(int j = 0; j < flagc; j++) {
            if(strcmp(cp_argv[i], flags[j]) == 0) {
                if(rv != -1) {
                    /* multiple flags found */
                    cp_report_error("Multiple exclusive flags found: '%s' and '%s'", flags[j], flags[rv]);
                    return -1;
                } else {
                    rv = j;
                    pop(i);
                    i--; /* adjust index after pop */
                }
            }
        }
    }
    return rv;
}

const char *
CP_ParseOption(const char *option)
{
    /* Parse -option value */
    const char *rv = NULL;
    size_t len = strlen(option);
    for(int i = 0; i < cp_argc; i++) {
        char *arg = cp_argv[i];
        if(strncmp(arg, option, len) == 0) {
            size_t arglen = strlen(arg);
            if(arglen > len) { /* -optionValue */
                char *value = arg + len;
                if(value[0] == '=') {
                    value++; /* skip '=' */
                    if(value[0] == '\0') { /* no value */
                        cp_report_error("%s= requires a value", option);
                        return NULL;
                    }
                }
                pop(i);
                i--;
                if(rv != NULL) {
                    cp_report_error("Multiple values of '%s' found", option);
                    return NULL;
                }
                rv = value;
            } else if(arglen == len) { /* -option value */
                if(i + 1 == cp_argc) { /* last argument, value is missing*/
                    cp_report_error("%s requires a value", option);
                    return NULL;
                }
                char *value = cp_argv[i+1];
                pop(i); /* remove option */
                pop(i); /* remove value */
                i -= 2;
                if(rv != NULL) {
                    cp_report_error("Multiple values of '%s' found", option);
                    return NULL;
                }
                rv = value;
            } else { /* unreachable */
                CP_UNREACHABLE();
                return NULL;
            }
        }
    }
    return rv;
}

int
CP_ParseOptionEx(const char *option, int valuec, const char **values)
{
    int count = 0;
    size_t len = strlen(option);
    for(int i = 0; i < cp_argc; i++) {
        char *arg = cp_argv[i];
        if(strncmp(arg, option, len) == 0) {
            size_t arglen = strlen(arg);
            if(arglen > len) { /* -optionValue */
                char *value = arg + len;
                if(value[0] == '=') {
                    value++; /* skip '=' */
                    if(value[0] == '\0') { /* no value */
                        cp_report_error("%s= requires a value", option);
                        return -1;
                    }
                }
                pop(i);
                if(count >= valuec) {
                    return -2;
                }
                values[count++] = value;
            } else if(arglen == len) { /* -option value */
                if(i + 1 == cp_argc) { /* last argument, value is missing*/
                    cp_report_error("%s requires a value");
                    return -1;
                }
                char *value = cp_argv[i+1];
                pop(i); /* remove option */
                pop(i); /* remove value */
                if(count >= valuec) {
                    return -2;
                }
                values[count++] = value;
            } else { /* unreachable */
                CP_UNREACHABLE();
                return -1;
            }
        }
    }
    return count;
}
