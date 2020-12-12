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
void mp_mul (digit_t *r, const digit_t *x, size_t xlen,
			 const digit_t *y, size_t ylen)
{
	size_t i;

	r[xlen] = mp_mul_1 (r, x, xlen, y[0]);

	for (i = 1; i < ylen; ++i) {
		r[i + xlen] = mp_addmul_1 (r + i, x, xlen, y[i]);
	}
}
