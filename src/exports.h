/*
 * exports.h - export and import macros for shared libraries.
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

#ifndef _CP_EXPORTS_H_
#define _CP_EXPORTS_H_

#ifdef _CP_STATIC_IMPORT_
#define CP_IMPORT
#else /* _CP_STATIC_IMPORT_ */
#ifdef _WIN32
#ifdef __MINGW32__
#define CP_IMPORT __attribute__((dllimport))
#else /* __MINGW32__ */
#define CP_IMPORT __declspec(dllimport)
#endif /* __MINGW32__ */
#else /* _WIN32 */
#define CP_IMPORT
#endif /* _WIN32 */
#endif /* _CP_STATIC_IMPORT_ */

#ifdef _WIN32
#ifdef __MINGW32__
#define CP_EXPORT __attribute__((dllexport))
#else /* __MINGW32__ */
#define CP_EXPORT __declspec(dllexport)
#endif /* __MINGW32__ */
#else /* _WIN32 */
#if defined(__GNUC__) && __GNUC__ >= 4 && __has_attribute(visibility)
#define CP_EXPORT __attribute__((visibility("default")))
#else /* __has_attribute(visibility) */
#define CP_EXPORT
#endif /* __has_attribute(visibility) */
#endif /* _WIN32 */

#define CP_EXPORT_FUNC(TYPE) CP_EXPORT TYPE
#define CP_IMPORT_FUNC(TYPE) CP_IMPORT TYPE
#define CP_EXPORT_DATA(TYPE) CP_EXPORT TYPE
#define CP_IMPORT_DATA(TYPE) CP_IMPORT extern TYPE

#endif /* _CP_EXPORTS_H_ */

#ifdef CP_API_FUNC
#undef CP_API_FUNC
#endif /* CP_API_FUNC */

#ifdef CP_API_DATA
#undef CP_API_DATA
#endif /* CP_API_DATA */

#ifdef CP_EXPORT_SYMBOL
#undef CP_EXPORT_SYMBOL
#define CP_API_FUNC(TYPE) CP_EXPORT_FUNC(TYPE)
#define CP_API_DATA(TYPE) CP_EXPORT_DATA(TYPE)
#else
#define CP_API_FUNC(TYPE) CP_IMPORT_FUNC(TYPE)
#define CP_API_DATA(TYPE) CP_IMPORT_DATA(TYPE)
#endif /* CP_EXPORT_SYMBOL */
