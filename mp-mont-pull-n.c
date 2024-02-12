/*
 * MP Core Modular Arithmetics: Montgomery Multiplication
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/mont-mul.h>
#include <mp/mul.h>
#include <mp/unit.h>

void mp_mont_pull_n (digit_t *r, const digit_t *x,
		     const digit_t *m, size_t len, digit_t mu)
{
	size_t i;

	mp_copy (r, x, len); r[len] = 0;
	r[len] += mp_addmul_1 (r, m, len, mu * r[0], 0);
	memmove (r, r + 1, len * sizeof (r[0]));

	for (i = 1; i < len; ++i) {
		r[len] += mp_addmul_1 (r, m, len, mu * r[0], 0);
		memmove (r, r + 1, len * sizeof (r[0])); r[len] = 0;
	}

	if (mp_cmp_n (r, m, len) >= 0)
		mp_sub_n (r, r, m, len, 0);
}
