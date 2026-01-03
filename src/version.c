// version.h
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license. 
// See LICENSE file in the project root for full license information.
//
// Provide with version information of the program at runtime.
#define _CP_VERSION_E_

#include "version.h"

CP_API_DATA(const char *) CP_VersionString = CP_VERSION_STRING;
CP_API_DATA(const int) CP_VersionType = CP_VERSION_TYPE;
CP_API_DATA(const int) CP_VersionMajor = CP_VERSION_MAJOR;
CP_API_DATA(const int) CP_VersionMinor = CP_VERSION_MINOR;
CP_API_DATA(const int) CP_VersionMicro = CP_VERSION_MICRO;
CP_API_DATA(const long) CP_VersionHex = CP_VERSION_HEX;