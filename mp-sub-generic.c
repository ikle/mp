/*
 * MP Core Addition: Substraction Generic Implemention
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/digit.h>

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
