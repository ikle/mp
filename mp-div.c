/*
 * MP Core Division
 *
 * Copyright (c) 2018-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/digit.h>
#include <mp/div.h>
#include <mp/mul.h>
#include <mp/unit.h>

#ifndef mp_pair_add
static inline
char mp_pair_add (digit_t *r1, digit_t *r0,
		  digit_t x1, digit_t x0, digit_t y1, digit_t y0)
{
	char c = mp_digit_add (r0, x0, y0);

	return mp_digit_adc (r1, x1, y1, c);
}
#endif

#ifndef mp_pair_sub
static inline
char mp_pair_sub (digit_t *r1, digit_t *r0,
		  digit_t x1, digit_t x0, digit_t y1, digit_t y0)
{
	char c = mp_digit_sub (r0, x0, y0);

	return mp_digit_sbb (r1, x1, y1, c);
}
#endif

/*
 * Function mp_pair_invert calculates the value of B^3 / (d + 1) - B.
 * The divisor d must be normalized, i.e. the most significant bit of d
 * is set.
 */
digit_t mp_pair_invert (digit_t d1, digit_t d0)
{
	digit_t r2, r1, r0, p1, p0;

	if (d1 + 1 == 0) {
		if (d0 + 1 == 0)
			/* (B^2 - (d + 1)) * B / (d + 1), where (d + 1) = B^2 */
			return 0;

		/* (B^2 - (d + 1)) * B / (d1 + 1) = B^2 - (d + 1) */
		r2 = ~d1, r1 = ~d0;
	}
	else
		/* (B^2 - (d + 1)) * B / (d1 + 1) */
		mp_digit_div (&r2, &r1, ~d1, ~d0, d1 + 1);

	if (d0 + 1 == 0)
		return r2;  /* d + 1 = [d1 + 1, 0] */

	r0 = 0;

	/* r += r2 * (B - (d0 + 1)) */
	mp_digit_mul (&p1, &p0, r2, ~d0);
	r2 += mp_pair_add (&r1, &r0, r1, r0, p1, p0);

	p1 = d1 + mp_digit_add (&p0, d0, 1);	/* p = d + 1 */

	/*
	 * Do it until [r1, r0] < (d + 1). Note that d is normalized, so
	 * this can only happen once.
	 */
	if (!mp_pair_sub (&r1, &r0, r1, r0, p1, p0))
		++r2;

	return r2;
}

/*
 * Function mp_pair_divapprox calculates the value of q = N B / (d + 1):
 *
 * 1. r = B^3 / (d + 1) - B
 * 2. q = N B / (d + 1)
 *      = N (B + B^3 / (d + 1) - B) / B^2
 *      = N (B + r) / B^2
 *      = (N B + N r) / B^2
 *      = (N + N r / B) / B
 *
 * The r value should be precalculated with mp_pair_invert.
 */
static inline
digit_t mp_pair_divapprox (digit_t n1, digit_t n0, digit_t r)
{
	digit_t q1, q0, p1, p0;

	mp_digit_mul (&q1, &q0, n1, r);
	mp_digit_mul (&p1, &p0, n0, r);

	q1 += mp_digit_add (&q0, q0, p1);	/* Q = N r / B */

	mp_pair_add (&q1, &q0, q1, q0, n1, n0);
	return q1;
}

static inline
digit_t mp_div_reduce (digit_t *q, digit_t c, digit_t *r,
		       const digit_t *d, size_t dlen, digit_t inv)
{
	q[0] = mp_pair_divapprox (c, r[dlen - 1], inv);

	c -= mp_submul_1 (r, d, dlen, q[0], 0);

	if (c > 0 || mp_cmp_n (r, d, dlen) >= 0) {
		++q[0];
		mp_sub_n (r, r, d, dlen, 0);
	}

	return r[dlen - 1];
}

/*
 * Function mp_div divides (n, nlen) by (d, dlen), stores quotinent into
 * (q, nlen - dlen + 1) and remainder into (r, nlen), and returns the size
 * of remainder. Constrains: n and d are normalized and nlen >= dlen > 0.
 * Note that the remainder is not normalized.
 */
size_t mp_div (digit_t *q, digit_t *r, const digit_t *n, size_t nlen,
				       const digit_t *d, size_t dlen)
{
	digit_t c = 0, inv;
	size_t i, j;

	if (r != n)
		mp_copy (r, n, nlen);

	inv = mp_pair_invert (d[dlen - 1], dlen > 1 ? d[dlen - 2]: 0);

	for (
		i = nlen - 1, j = nlen - dlen;
		j > 0;
		--i, --j
	) {
		c = mp_div_reduce (q + j, c, r + j, d, dlen, inv);
		r[i] = 0;
	}

	r[i] = mp_div_reduce (q + 0, c, r + 0, d, dlen, inv);

	return i + 1;
}
