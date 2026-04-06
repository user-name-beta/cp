/*
 * path.h - handle file paths.
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

#ifndef _CP_PATH_H_
#define _CP_PATH_H_

#ifdef _WIN32
#include <windows.h>
#define CP_MAX_PATH MAX_PATH
#define CP_PATH_SEP "\\"
#define CP_PATH_SEP2 "/"
#define CP_IS_PATH_SEP(c) ((c) == '\\' || (c) == '/')
#else /* _WIN32 */
#include <limits.h>
#define CP_MAX_PATH PATH_MAX
#define CP_PATH_SEP "/"
#define CP_PATH_SEP2 "/"
#define CP_IS_PATH_SEP(c) ((c) == '/')
#endif /* _WIN32 */

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool CPath_IsAbsolute(const char *path);
int CPath_Getcwd(char *cwd);
int CPath_Join(char *dst, const char *src1, const char *src2);
int CPath_JoinInPlace(char *dst, const char *src);
int CPath_Filename(char *dst, const char *src);
int CPath_Dirname(char *dst, const char *src);

#ifdef __cplusplus
}
#endif

#endif /* _CP_PATH_H_ */
