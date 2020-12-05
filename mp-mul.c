/*
 * MP Core Multiplication
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/core.h>

/*
 * Constrains: xlen >= ylen > 0
 */
mp_word mp_mul (mp_word *r, const mp_word *x, size_t xlen,
			    const mp_word *y, size_t ylen)
{
	mp_word c;
	size_t i;

	c = mp_mul_1 (r, x, xlen, y[0]);

	for (i = 1; i < ylen; ++i) {
		r++[xlen] = c;

		c = mp_addmul_1 (r, x, xlen, y[i]);
	}

	return c;
}
