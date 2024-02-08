/*
 * MP Core Addition: Summation with a Single Digit, Generic Implemention
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/digit.h>

char mp_add_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	int c;

	c = mp_digit_add (r, x[0], y);

	for (i = 1; i < len; ++i)
		c = mp_digit_adc (r + i, x[i], 0, c);

	return c;
}
