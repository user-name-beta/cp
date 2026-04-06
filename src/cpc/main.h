/*
 * main.h - entry point of the cpc program
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
