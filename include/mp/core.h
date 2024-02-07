/*
 * MP Core
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_CORE_H
#define MP_CORE_H  1

#include <mp/add.h>
#include <mp/bit.h>
#include <mp/mul.h>
#include <mp/div.h>
#include <mp/shift.h>
#include <mp/unit.h>

/*
 * Function mp_normalize normalizes (x, len), and returns minimal possible
 * length value of x.
 */
static inline size_t mp_normalize (digit_t *x, size_t len)
{
	while (len > 0 && x[len - 1] == 0)
		--len;

	return len;
}

#endif  /* MP_CORE_H */
