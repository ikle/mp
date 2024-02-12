/*
 * MP Core Modular Arithmetics: Montgomery Multiplication
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/mont-mul.h>

static inline digit_t mp_digit_mod_exp2 (digit_t x, int n)
{
	const digit_t mask = ((digit_t) 1 << n) - 1;

	return x & mask;
}

digit_t mp_mont_mu (digit_t m0)
{
	digit_t x;
	int i;

	for (x = 1, i = 2; i < MP_DIGIT_BITS; ++i)
		if (mp_digit_mod_exp2 (m0 * x, i) != 1)
			x |= (digit_t) 1 << (i - 1);

	if ((digit_t) (m0 * x) != 1)
		x |= (digit_t) 1 << (i - 1);

	return 0 - x;
}
