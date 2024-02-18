/*
 * MP Core Shift
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_SHIFT_H
#define MP_SHIFT_H  1

#include <string.h>

#include <mp/types.h>

/*
 * Function mp_lshift multiplies (x, len) by 2^n, stores result bitwise
 * ored with input carry into (r, len), and returns the shift carry value
 * (shifted-out bits). The n must be less than MP_DIGIT_BITS.
 *
 * Function mp_rshift divides (x, len) by 2^n, stores result bitwise ored
 * with input carry at high digit into (r, len), and returns the remainder
 * value (shifted-out bits). The n must be less than MP_DIGIT_BITS.
 *
 * Function mp_rshift_word shifts one digit right, stores result into
 * (r, len) with input carry at high digit.
 */
digit_t mp_lshift (digit_t *r, const digit_t *x, size_t len, digit_t c, int n);
digit_t mp_rshift (digit_t *r, const digit_t *x, size_t len, digit_t c, int n);

static inline
void mp_rshift_word (digit_t *r, const digit_t *x, size_t len, digit_t c)
{
	memmove (r, x + 1, (len - 1) * sizeof (x[0]));
	r[len - 1] = c;
}

#endif  /* MP_SHIFT_H */
