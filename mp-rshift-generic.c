/*
 * MP Core Right Shift: Generic Implementation
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/digit.h>
#include <mp/shift.h>

/*
 * Function mp_rshift divides (x, len) by 2^n, stores result bitwise ored
 * with input carry at high digit into (r, len), and returns the remainder
 * value (shifted-out bits). The n must be less than MP_DIGIT_BITS.
 */
digit_t mp_rshift (digit_t *r, const digit_t *x, size_t len, digit_t c, int n)
{
	size_t i;

	for (i = len; i > 0; )
		--i, c = mp_digit_rshift (r + i, x[i], c, n);

	return c;
}
