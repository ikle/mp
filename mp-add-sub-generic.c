/*
 * MP Core Addition: Substraction Generic Implemention
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/digit.h>

char mp_sub_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len,
	       int c)
{
	size_t i;

	for (i = 0; i < len; ++i)
		c = mp_digit_sbb (r + i, x[i], y[i], c);

	return c;
}

char mp_sub_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	int c;

	c = mp_digit_sub (r, x[0], y);

	for (i = 1; i < len; ++i)
		c = mp_digit_sbb (r + i, x[i], 0, c);

	return c;
}

char mp_sub (digit_t *r, const digit_t *x, size_t xlen,
			 const digit_t *y, size_t ylen, int c)
{
	size_t i;

	for (i = 0; i < ylen; ++i)
		c = mp_digit_sbb (r + i, x[i], y[i], c);

	for (; i < xlen; ++i)
		c = mp_digit_sbb (r + i, x[i], 0, c);

	return c;
}
