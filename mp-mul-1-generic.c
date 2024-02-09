/*
 * MP Core Multiplication: Generic Implemention
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/digit.h>
#include <mp/mul.h>

digit_t mp_mul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	digit_t c;

	for (i = 0, c = 0; i < len; ++i)
		mp_digit_fma (&c, r + i, x[i], y, c);

	return c;
}

digit_t mp_addmul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y,
		     int c)
{
	size_t i;
	digit_t r1 = c, r0;

	for (i = 0; i < len; ++i) {
		mp_digit_fma (&r1, &r0, x[i], y, r1);
		r1 += mp_digit_add (r + i, r[i], r0);
	}

	return r1;
}

digit_t mp_submul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y,
		     int c)
{
	size_t i;
	digit_t r1 = c, r0;

	for (i = 0; i < len; ++i) {
		mp_digit_fma (&r1, &r0, x[i], y, r1);
		r1 += mp_digit_sub (r + i, r[i], r0);
	}

	return r1;
}
