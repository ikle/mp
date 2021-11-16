/*
 * MP Core Multiplication
 *
 * Copyright (c) 2018-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/mul.h>

/*
 * Constrains for all mp_mul and mp_addmul : xlen >= ylen > 0
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

#define MP_KARATSUBA_CUTOFF  20

/*
 * Constrains: xlen >= ylen > 4 to prevent overflow
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
