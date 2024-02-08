/*
 * MP Core Addition: Negation Generic Implemention
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/digit.h>

char mp_neg (digit_t *r, const digit_t *x, size_t len)
{
	size_t i;
	int c = 0;

	for (i = 0; i < len; ++i)
		c = mp_digit_sbb (r + i, 0, x[i], c);

	return c;
}
