/*
 * MP Core Addition Generic Implemention
 *
 * Copyright (c) 2014-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include <mp/add.h>
#include <mp/digit.h>

char mp_add_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len,
	       int c)
{
	size_t i;

	for (i = 0; i < len; ++i)
		c = mp_digit_adc (r + i, x[i], y[i], c);

	return c;
}

char mp_add_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	int c;

	c = mp_digit_add (r, x[0], y);

	for (i = 1; i < len; ++i)
		c = mp_digit_adc (r + i, x[i], 0, c);

	return c;
}

char mp_add (digit_t *r, const digit_t *x, size_t xlen,
			 const digit_t *y, size_t ylen, int c)
{
	size_t i;

	for (i = 0; i < ylen; ++i)
		c = mp_digit_adc (r + i, x[i], y[i], c);

	for (; i < xlen; ++i)
		c = mp_digit_adc (r + i, x[i], 0, c);

	return c;
}

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

char mp_neg (digit_t *r, const digit_t *x, size_t len)
{
	size_t i;
	int c = 0;

	for (i = 0; i < len; ++i)
		c = mp_digit_sbb (r + i, 0, x[i], c);

	return c;
}

int mp_cmp_n (const digit_t *x, const digit_t *y, size_t len)
{
	for (; len > 0; --len) {
		if (x[len - 1] > y[len - 1])
			return 1;

		if (x[len - 1] < y[len - 1])
			return -1;
	}

	return 0;
}
