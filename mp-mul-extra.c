/*
 * MP Core Multiplication: Extra (unised) Functions
 *
 * Copyright (c) 2018-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/mul.h>

char mp_addmul_sb (digit_t *r, const digit_t *x, size_t xlen,
			       const digit_t *y, size_t ylen, int c)
{
	size_t i;
	digit_t rh;

	/*
	 * 1. mp_addmul_1 cannot oveflow: FF * F + FF + 1 = FF1
	 * 2. mp_add_1 can overflow only once
	 */
	rh = mp_addmul_1 (r + 0, x, xlen, y[0], c);
	c  = mp_add_1 (r + xlen + 0, r + xlen + 0, ylen - 0, rh);

	for (i = 1; i < ylen; ++i) {
		rh = mp_addmul_1 (r + i, x, xlen, y[i], 0);
		c |= mp_add_1 (r + xlen + i, r + xlen + i, ylen - i, rh);
	}

	return c;
}
