// commandline.h
//
// Copyright (c) 2025-2026 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Functions for simple commands like --help and --version.

#ifndef _CP_COMMANDLINE_H_
#define _CP_COMMANDLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

int CPCommandLine_GetExecutablePath(char *dst);
int CPCommandLine_PrintFile(const char *directory, const char *rel_path);
void CPCommandLine_PrintCopyright(void);
void CPCommandLine_PrintVersion(void);
int CPCommandLine_GetHomeDirectory(char *dst, const char *exe);
int CPCommandLine_PrintLicense(const char *home);
int CPCommandLine_PrintHelp(const char *home, const char *helpfile);

#ifdef __cplusplus
}
#endif

#endif /* _CP_COMMANDLINE_H_ */
