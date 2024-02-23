/*
 * MP Core Modular Arithmetics: Montgomery Multiplication
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/digit.h>
#include <mp/div.h>
#include <mp/mont-mul.h>
#include <mp/shift.h>
#include <mp/unit.h>

void mp_mont_ro (digit_t *r, const digit_t *m, size_t len)
{
#if 1
	const size_t n = len * 2;
	digit_t R2[n + 1];

	mp_zero (R2, n); R2[n] = 1;  /* R^2, where R = B^len */

	mp_mod (r, R2, n + 1, m, len);
#else
	size_t i, j;

	mp_neg (r, m, len);

	for (i = 0; i < len; ++i)
		for (j = 0; j < MP_DIGIT_BITS; ++j)
			mp_mod_add_n (r, r, r, m, len);
#endif
}
