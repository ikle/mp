/*
 * MP Core Division: Digit Pair Inversion
 *
 * Copyright (c) 2018-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/pair.h>

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
