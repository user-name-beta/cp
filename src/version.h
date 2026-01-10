// version.h
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license. 
// See LICENSE file in the project root for full license information.
//
// Provide with version information of the program.

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

#endif // _CP_VERSION_H_