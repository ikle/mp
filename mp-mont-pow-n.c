/*
 * MP Core Modular Exponentiation in Montgomery Form
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/mont-mul.h>
#include <mp/unit.h>

void mp_mont_pow_n (digit_t *r, const digit_t *x, const digit_t *y,
		    const digit_t *m, size_t len, digit_t mu)
{
	size_t i, j;
	digit_t d, X[len], t[len];

	mp_copy (X, x, len);

	for (i = 0; i < len; ++i)
		for (j = 0, d = y[i]; j < MP_DIGIT_BITS; ++j, d >>= 1) {
			if ((d & 1) != 0) {
				mp_mont_mul_n (t, r, X, m, len, mu);
				mp_copy (r, t, len);
			}

			mp_mont_mul_n (t, X, X, m, len, mu);
			mp_copy (X, t, len);
		}
}
