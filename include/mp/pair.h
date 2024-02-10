/*
 * Multiple-Precission Digit Pair Primitives
 *
 * Copyright (c) 2018-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_PAIR_H
#define MP_PAIR_H  1

#include <mp/digit.h>

/*
 * Function mp_pair_add adds [y1, y0] to [x1, x0], stores result into
 * [r1, r0], and returns the carry value.
 */
#ifndef mp_pair_add
static inline char mp_pair_add (digit_t *r1, digit_t *r0,
				digit_t x1, digit_t x0, digit_t y1, digit_t y0)
{
	char c = mp_digit_add (r0, x0, y0);

	return mp_digit_adc (r1, x1, y1, c);
}
#endif

/*
 * Function mp_pair_sub substracts [y1, y0] from [x1, x0], stores result
 * into [r1, r0], and returns the borrow value.
 */
#ifndef mp_pair_sub
static inline char mp_pair_sub (digit_t *r1, digit_t *r0,
				digit_t x1, digit_t x0, digit_t y1, digit_t y0)
{
	char c = mp_digit_sub (r0, x0, y0);

	return mp_digit_sbb (r1, x1, y1, c);
}
#endif

/*
 * Function mp_pair_invert calculates the value of B^3 / (d + 1) - B.
 * The divisor d = [d1, d0] must be fully normalized, i.e. the most
 * significant bit of d1 is set.
 *
 * In the expression above, B = 2^n, where n = MP_DIGIT_BITS.
 */
digit_t mp_pair_invert (digit_t d1, digit_t d0);

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
 * The r value should be precalculated with mp_pair_invert. In the
 * expression above, B = 2^n, where n = MP_DIGIT_BITS.
 */
static inline digit_t mp_pair_divapprox (digit_t n1, digit_t n0, digit_t r)
{
	digit_t q1, q0, p1, p0;

	mp_digit_mul (&q1, &q0, n1, r);
	mp_digit_mul (&p1, &p0, n0, r);

	q1 += mp_digit_add (&q0, q0, p1);	/* Q = N r / B */

	mp_pair_add (&q1, &q0, q1, q0, n1, n0);
	return q1;
}

#endif  /* MP_PAIR_H */
