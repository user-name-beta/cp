/*
 * report_error.h - error reporting functions.
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

#ifndef _CP_REPORT_ERROR_H_
#define _CP_REPORT_ERROR_H_

#include <stdarg.h>

#include "path.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char cp_exename[CP_MAX_PATH];

void cp_report_fatal(const char *fmt, ...);
void cp_report_error(const char *fmt, ...);
void cp_report_fatal_v(const char *fmt, va_list args);
void cp_report_error_v(const char *fmt, va_list args);

#ifdef __cplusplus
}
#endif

#endif /* _CP_REPORT_ERROR_H_ */
