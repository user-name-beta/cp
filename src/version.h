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

#define CP_VERSION_TYPE 0 // CP_VERSION_PRERELEASE
#define CP_VERSION_MAJOR 1
#define CP_VERSION_MINOR 0
#define CP_VERSION_MICRO 0
#define CP_VERSION_HEX 0x00100000L

#define CP_VERSION_PRERELEASE 0
#define CP_VERSION_RELEASE 1

CP_API_DATA(const char *) CP_VersionString;
CP_API_DATA(const int) CP_VersionType;
CP_API_DATA(const int) CP_VersionMajor;
CP_API_DATA(const int) CP_VersionMinor;
CP_API_DATA(const int) CP_VersionMicro;
CP_API_DATA(const long) CP_VersionHex;

#endif // _CP_VERSION_H_