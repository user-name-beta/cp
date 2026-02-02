// cpassert.h
//
// Copyright (c) 2025 user-name-beta. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.
//
// assert.h and static_assert (if compiler does not support static_assert natively).

#ifndef _CP_CPASSERT_H_
#define _CP_CPASSERT_H_

#include <assert.h>

#ifndef __cplusplus
#ifndef static_assert
#define static_assert(cond, msg) extern char _cp_static_assertion_symbol[(cond) ? 1 : -1]
#endif /* static_assert */
#endif /* __cplusplus */

#endif /* _CP_CPASSERT_H_ */