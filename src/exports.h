// exports.h
//
// Copyright (c) 2025-9999 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Macros for exporting and importing cp API functions.
// This header can be included by other headers more than once in one source file
// but including it explicitly multiple times is not allowed.
// NOTE: You must include this header file after including all other headers.

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