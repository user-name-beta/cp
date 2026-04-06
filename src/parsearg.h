/*
 * parse.h - command line argument parsing
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

#ifndef _CP_PARSE_H_
#define _CP_PARSE_H_

/* Always static link */

#ifdef __cplusplus
extern "C" {
#endif

extern int cp_argc;
extern char **cp_argv;

const char *CP_ParseOneArg(void);
int CP_ParseFlag(const char *flag);
int CP_ParseFlagEx(int flagc, const char * const *flags);
const char *CP_ParseOption(const char *option);
int CP_ParseOptionEx(const char *option, int valuec, const char **values);
int CP_ParseAssertNoMoreArgs(void);

#ifdef __cplusplus
}
#endif

#endif /* _CP_PARSE_H_ */
