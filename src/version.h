/*
 * version.h - version information.
 * Copyright (C) 2026 Huang Jiangyao. All rights reserved.
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

#ifndef _CP_VERSION_H_
#define _CP_VERSION_H_

#ifdef _CP_VERSION_E_
#define CP_EXPORT_SYMBOL
#endif
#include "exports.h"

#define CP_VERSION_STRING "p1.0.0"

#define CP_VERSION_TYPE 0xA0 // CP_VERSION_PRERELEASE
#define CP_VERSION_MAJOR 1
#define CP_VERSION_MINOR 0
#define CP_VERSION_MICRO 0
#define CP_VERSION_HEX 0xF0100000L

#define CP_VERSION_PRERELEASE 0xA0
#define CP_VERSION_RELEASE 0xFF

#define CP_BYTECODE_MAGIC_NUMBER_SIZE 4
#define CP_BYTECODE_MAGIC_NUMBER "\x63\x70\x6d\x80"
#define CP_BYTECODE_VERSION_MAJOR 0x00000000L
#define CP_BYTECODE_VERSION_MINOR 0x00000000L

#ifdef __cplusplus
extern "C" {
#endif

CP_API_DATA(const char *) CP_VersionString;
CP_API_DATA(const int) CP_VersionType;
CP_API_DATA(const int) CP_VersionMajor;
CP_API_DATA(const int) CP_VersionMinor;
CP_API_DATA(const int) CP_VersionMicro;
CP_API_DATA(const long) CP_VersionHex;
CP_API_DATA(const char *) CP_BytecodeMagicNumber;
CP_API_DATA(const long) CP_BytecodeVersionMajor;
CP_API_DATA(const long) CP_BytecodeVersionMinor;

#ifdef __cplusplus
}
#endif

#endif /* _CP_VERSION_H_ */
