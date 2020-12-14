/*
 * MP Core Multiplication
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include <mp/core.h>

static inline void mp_zero (digit_t *x, size_t len)
{
	memset (x, 0, len * sizeof (x[0]));
}

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

static
char mp_addmul_sb (digit_t *r, const digit_t *x, size_t xlen,
			       const digit_t *y, size_t ylen, int c)
{
	size_t i;
	digit_t rh;

	for (i = 0; i < ylen; ++i) {
		rh = mp_addmul_1 (r + i, x, xlen, y[i], c);
		c = mp_add_1 (r + xlen + i, r + xlen + i, ylen - i, rh);
	}

	return c;
}

#define MP_KARATSUBA_CUTOFF  20

static
char mp_addmul_kara (digit_t *r, const digit_t *x, size_t xlen,
				 const digit_t *y, size_t ylen, int C)
{
	/*
	 * len must be less at least 4 to prevent infinite recursion
	 */
	if (ylen < MP_KARATSUBA_CUTOFF)
		return mp_addmul_sb (r, x, xlen, y, ylen, C);

	const size_t blen = ylen / 2, alen = xlen - blen;
	const size_t dlen = ylen / 2, clen = ylen - dlen;

	const digit_t *a = x + blen, *b = x, *c = y + dlen, *d = y;

	digit_t *ac = r + blen + dlen, *bd = r;
	digit_t apb[alen + 1], cpd[clen + 1], m[alen + clen + 2];

	C = mp_addmul_kara (bd, b, blen, d, dlen, C);
	C = mp_addmul_kara (ac, a, alen, c, clen, C);

	apb[alen] = mp_add (apb, a, alen, b, blen, 0);
	cpd[clen] = mp_add (cpd, c, clen, d, dlen, 0);

	mp_mul (m, apb, alen + 1, cpd, clen + 1);

	mp_sub (m, m, alen + clen + 2, ac, alen + clen, 0);
	mp_sub (m, m, alen + clen + 2, bd, blen + dlen, 0);

	/* Note that the most significant digit of m is always zero */
	return mp_add (r + dlen, r + dlen, xlen + clen, m, alen + clen + 1, 0) +
	       C;
}

static
void mp_mul_kara (digit_t *r, const digit_t *x, size_t xlen,
		  const digit_t *y, size_t ylen)
{
	mp_zero (r, xlen + ylen);
	mp_addmul_kara (r, x, xlen, y, ylen, 0);
}

void mp_mul (digit_t *r, const digit_t *x, size_t xlen,
			 const digit_t *y, size_t ylen)
{
	if (xlen < MP_KARATSUBA_CUTOFF)
		mp_mul_sb (r, x, xlen, y, ylen);
	else
		mp_mul_kara (r, x, xlen, y, ylen);
}

char mp_addmul (digit_t *r, const digit_t *x, size_t xlen,
			    const digit_t *y, size_t ylen, int c)
{
	if (xlen < MP_KARATSUBA_CUTOFF)
		return mp_addmul_sb (r, x, xlen, y, ylen, c);

	return mp_addmul_kara (r, x, xlen, y, ylen, 0);
}
