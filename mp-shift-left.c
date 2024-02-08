/*
 * MP Core Left Shift
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/digit.h>
#include <mp/shift.h>

/*
 * Function mp_lshift multiplies (x, len) by 2^n, stores result bitwise
 * ored with input carry into (r, len), and returns the shift carry value
 * (shifted-out bits). The n must be less than MP_DIGIT_BITS.
 */
digit_t mp_lshift (digit_t *r, const digit_t *x, size_t len, digit_t c, int n)
{
	size_t i;

	for (i = 0; i < len; ++i)
		c = mp_digit_lshift (r + i, x[i], c, n);

	return c;
}
