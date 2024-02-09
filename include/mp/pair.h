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

#endif  /* MP_PAIR_H */
