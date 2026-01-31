// parse.h
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Parse the command line arguments.

#ifndef _CP_PARSE_H_
#define _CP_PARSE_H_

/* Always static link */

extern int cp_argc;
extern char **cp_argv;

const char *CP_ParseOneArg(void);
int CP_ParseFlag(const char *flag);
int CP_ParseFlagEx(int flagc, const char * const *flags);
const char *CP_ParseOption(const char *option);
int CP_ParseOptionEx(const char *option, int valuec, const char **values);
int CP_ParseAssertNoMoreArgs(void);

#endif /* _CP_PARSE_H_ */