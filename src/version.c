/*
 * version.c - version information of the program.
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
#define _CP_VERSION_E_

#include "config.h"
#include "version.h"

CP_API_DATA(const char *) CP_VersionString = CP_VERSION_STRING;
CP_API_DATA(const int) CP_VersionType = CP_VERSION_TYPE;
CP_API_DATA(const int) CP_VersionMajor = CP_VERSION_MAJOR;
CP_API_DATA(const int) CP_VersionMinor = CP_VERSION_MINOR;
CP_API_DATA(const int) CP_VersionMicro = CP_VERSION_MICRO;
CP_API_DATA(const long) CP_VersionHex = CP_VERSION_HEX;
CP_API_DATA(const char *) CP_BytecodeMagicNumber = CP_BYTECODE_MAGIC_NUMBER;
CP_API_DATA(const long) CP_BytecodeVersionMajor = CP_BYTECODE_VERSION_MAJOR;
CP_API_DATA(const long) CP_BytecodeVersionMinor = CP_BYTECODE_VERSION_MINOR;
