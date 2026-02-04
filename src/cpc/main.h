// main.h
//
// Copyright (c) 2025-2027 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Provide with the entry point function CP_Main() for the program.

#ifndef _CP_CPC__MAIN_H_
#define _CP_CPC__MAIN_H_

#ifdef _CP_CPC__MAIN_E_
#define CP_EXPORT_SYMBOL
#endif
#include <exports.h>

#ifdef __cplusplus
extern "C" {
#endif

CP_API_FUNC(int) CPMainProgramEntryPoint_CPC(int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif /* _CP_CPC__MAIN_H_ */