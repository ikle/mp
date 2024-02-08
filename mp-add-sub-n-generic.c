/*
 * MP Core Addition: Difference b/w Numbers of the Same Size, Generic Impl.
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
