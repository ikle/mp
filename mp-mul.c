/*
 * MP Core Multiplication
 *
 * Copyright (c) 2018 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/core.h>

mp_word mp_mul (mp_word *r, const mp_word *x, size_t xlen,
			    const mp_word *y, size_t ylen)
{
	mp_word c, a = y[0];
	size_t i;

	if (ylen == 0)
		return 0;

	c = mp_mul_1 (r, x, xlen, a);

	r++[xlen] = c;

	for (i = 1; i < ylen; ++i) {
		a = y[i];

		c = mp_addmul_1 (r, x, xlen, a);

		r++[xlen] = c;
	}

	return c;
}
