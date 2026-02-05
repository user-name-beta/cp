// parse.c
//
// Copyright (c) 2025-9999 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Parse the command line arguments.

#include <stdio.h>
#include <string.h>
#include "cptypes.h"

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
    for(int i = 0; i < cp_argc; i++) {
        if(strcmp(cp_argv[i], flag) == 0){
            pop(i);
            return 1;
        }
    }
    return 0;
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
                        return NULL;
                    }
                }
                pop(i);
                return value;
            } else if(arglen == len) { /* -option value */
                if(i + 1 == cp_argc) { /* last argument, value is missing*/
                    return NULL;
                }
                char *value = cp_argv[i+1];
                pop(i); /* remove option */
                pop(i); /* remove value */
                return value;
            } else { /* unreachable */
                CP_UNREACHABLE();
                return NULL;
            }
        }
    }
    return NULL;
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