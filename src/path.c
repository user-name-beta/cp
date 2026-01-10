// path.c
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Handle file paths.

#include "cptypes.h"
#include "path.h"
#include "safe_string.h"

#ifndef _WIN32
__attribute__((unused)) // Unix systems have no volume concept.
#endif
static inline char
get_volume(const char *path) {
#ifndef _WIN32
    return '\0';
#else
    if(strlen(path) >= 2 && path[1] == ':') {
        return path[0];
    }
    return '\0';
#endif
}

static inline void
remove_end_sep(char *path) {
    size_t len = strlen(path);
    if(len > 0 && CP_IS_PATH_SEP(path[len-1])) {
        path[len-1] = '\0';
    }
}

bool
CPath_IsAbsolute(const char *path) {
#ifdef _WIN32
    return get_volume(path) != '\0';
#else
    if(strlen(path) > 0 && path[0] == '/') {
        return true;
    }
    return false;
#endif
}

int
CPath_Getcwd(char *cwd) {
    if(cwd == NULL)return -1;
#ifdef _WIN32
    DWORD len = GetCurrentDirectoryA(CP_MAX_PATH, cwd);
    if(len == 0 || len >= CP_MAX_PATH) {
        /* len does not include null terminator */
        return -1;
    }
#else
    if(getcwd(cwd, CP_MAX_PATH) == NULL) {
        return -1;
    }
#endif
    return 0;
}

int
CPath_Setcwd(const char *path) {
    if(path == NULL)return -1;
#ifdef _WIN32
    if(SetCurrentDirectoryA(path) == 0) {
        return -1;
    }
#else
    if(chdir(path) == -1) {
        return -1;
    }
#endif
    return 0;
}

int
CPath_Join(char *dst, const char *src1, const char *src2) {
    if(dst == NULL || src1 == NULL || src2 == NULL)return -1;
    if(CPath_IsAbsolute(src2)) {
        return -1;
    }
    if(strcpy_safe(dst, src1, CP_MAX_PATH) != 0)return -1;
    remove_end_sep(dst);
    if(strcat_safe(dst, CP_PATH_SEP, CP_MAX_PATH) != 0)return -1;
    if(strcat_safe(dst, src2, CP_MAX_PATH) != 0)return -1;
    remove_end_sep(dst);
    return 0;
}

int
CPath_JoinInPlace(char *dst, const char *src) {
    if(dst == NULL || src == NULL)return -1;
    if(CPath_IsAbsolute(src)) {
        return -1;
    }
    remove_end_sep(dst);
    if(strcat_safe(dst, CP_PATH_SEP, CP_MAX_PATH) != 0)return -1;
    if(strcat_safe(dst, src, CP_MAX_PATH) != 0)return -1;
    remove_end_sep(dst);
    return 0;
}

int
CPath_Filename(char *dst, const char *src) {
    if(dst == NULL || src == NULL)return -1;
    size_t len = strlen(src);
    for(ssize_t i = (ssize_t)len - 1; i >= 0; i--) {
        if(CP_IS_PATH_SEP(src[i])) {
            if(strcpy_safe(dst, src + i + 1, CP_MAX_PATH) != 0)return -1;
            return 0;
        }
    }
    if(strcpy_safe(dst, src, CP_MAX_PATH) != 0)return -1;
    return 0;
}

int
CPath_Dirname(char *dst, const char *src) {
    if(dst == NULL || src == NULL)return -1;
    size_t len = strlen(src);
    for(ssize_t i = len - 1; i >= 0; i--) {
        if(CP_IS_PATH_SEP(src[i])) {
            if(i == 0) {
                dst[0] = src[0];
                dst[1] = '\0';
            } else {
                if(strncpy_safe(dst, src, i, CP_MAX_PATH) != 0)return -1;
                dst[i] = '\0';
            }
            return 0;
        }
    }
    dst[0] = '\0';
    return 0;
}