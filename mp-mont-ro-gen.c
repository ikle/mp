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

void mp_mont_ro_gen (digit_t *r, const digit_t *m, size_t len)
{
	const size_t n = len * 2;
	const int shift = mp_digit_clz (m[len - 1]);
	digit_t R2[n + 1], ms[len];

	/* R^2 * 2^shift, where R = B^len */
	mp_zero (R2, n); R2[n] = (digit_t) 1 << shift;
	mp_lshift (ms, m, len, 0, shift);

	mp_mod (r, R2, n + 1, ms, len);
}
