// report_error.h
//
// Copyright (c) 2025-3000 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// Report error in command-line interface.

#ifndef _CP_REPORT_ERROR_H_
#define _CP_REPORT_ERROR_H_

void cp_report_fatal(const char *exename, const char *fmt, ...);
void cp_report_error(const char *exename, const char *fmt, ...);

#endif /* _CP_REPORT_ERROR_H_ */