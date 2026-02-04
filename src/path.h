// path.h
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Handle file paths.

#ifndef _CP_PATH_H_
#define _CP_PATH_H_

#ifdef _WIN32
#include <windows.h>
#define CP_MAX_PATH MAX_PATH
#define CP_PATH_SEP "\\"
#define CP_PATH_SEP2 "/"
#define CP_IS_PATH_SEP(c) ((c) == '\\' || (c) == '/')
#else /* _WIN32 */
#define _POSIX_C_SOURCE 200809L
#include <limits.h>
#define CP_MAX_PATH PATH_MAX
#define CP_PATH_SEP "/"
#define CP_PATH_SEP2 "/"
#define CP_IS_PATH_SEP(c) ((c) == '/')
#endif /* _WIN32 */

#include <stdbool.h>

bool CPath_IsAbsolute(const char *path);
int CPath_Getcwd(char *cwd);
int CPath_Join(char *dst, const char *src1, const char *src2);
int CPath_JoinInPlace(char *dst, const char *src);
int CPath_Filename(char *dst, const char *src);
int CPath_Dirname(char *dst, const char *src);

#endif /* _CP_PATH_H_ */