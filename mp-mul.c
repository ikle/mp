/*
 * MP Core Multiplication
 *
 * Copyright (c) 2018-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/mul.h>

/*
 * Constraint for all mp_mul: xlen >= ylen > 0
 */
void mp_mul_sb (digit_t *r, const digit_t *x, size_t xlen,
			    const digit_t *y, size_t ylen)
{
	size_t i;

	r[xlen] = mp_mul_1 (r, x, xlen, y[0]);

	for (i = 1; i < ylen; ++i) {
		r[i + xlen] = mp_addmul_1 (r + i, x, xlen, y[i], 0);
	}
}

#define MP_KARATSUBA_CUTOFF  20

/*
 * Constraint: xlen >= ylen > 4 to prevent overflow
 */
static
void mp_mul_kara (digit_t *r, const digit_t *x, size_t xlen,
			      const digit_t *y, size_t ylen)
{
	const size_t blen = ylen / 2, alen = xlen - blen;
	const size_t dlen = ylen / 2, clen = ylen - dlen;

	const digit_t *a = x + blen, *b = x, *c = y + dlen, *d = y;

	digit_t *ac = r + blen + dlen, *bd = r;

	mp_mul (bd, b, blen, d, dlen);
	mp_mul (ac, a, alen, c, clen);

	{
		/*
		 * do not move it up to prevent O(n log n) memory usage where
		 * O(n) is enough
		 */
		digit_t apb[alen + 1], cpd[clen + 1], m[alen + clen + 2];

		apb[alen] = mp_add (apb, a, alen, b, blen, 0);
		cpd[clen] = mp_add (cpd, c, clen, d, dlen, 0);

		mp_mul (m, apb, alen + 1, cpd, clen + 1);

		/* ignore carry, it evaluates to zero always */
		mp_sub (m, m, alen + clen + 1, ac, alen + clen, 0);
		mp_sub (m, m, alen + clen + 1, bd, blen + dlen, 0);

		/* Note that the most significant digit of m is always zero */
		mp_add (r + dlen, r + dlen, xlen + clen, m, alen + clen + 1, 0);
	}
}

void mp_mul (digit_t *r, const digit_t *x, size_t xlen,
			 const digit_t *y, size_t ylen)
{
	if (ylen < MP_KARATSUBA_CUTOFF)
		mp_mul_sb (r, x, xlen, y, ylen);
	else
		mp_mul_kara (r, x, xlen, y, ylen);
}
