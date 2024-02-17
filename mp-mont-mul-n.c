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

static inline
void mp_rshift_word (digit_t *r, const digit_t *x, size_t len, digit_t c)
{
	memmove (r, x + 1, (len - 1) * sizeof (x[0]));
	r[len - 1] = c;
}

void mp_mont_mul_n (digit_t *r, const digit_t *x, const digit_t *y,
		    const digit_t *m, size_t len, digit_t mu)
{
	digit_t rc;
	size_t i;

	rc  = mp_mul_1 (r, x, len, y[0]);
	rc += mp_addmul_1 (r, m, len, mu * r[0], 0);
	mp_rshift_word (r, r, len, rc);

	for (i = 1; i < len; ++i) {
		rc  = mp_addmul_1 (r, x, len, y[i], 0);
		rc += mp_addmul_1 (r, m, len, mu * r[0], 0);
		mp_rshift_word (r, r, len, rc);
	}

	if (mp_cmp_n (r, m, len) >= 0)
		mp_sub_n (r, r, m, len, 0);
}

/*
 * Montgomery Reduction
 */
void mp_mont_pull_n (digit_t *r, const digit_t *x,
		     const digit_t *m, size_t len, digit_t mu)
{
	digit_t rc;
	size_t i;

	mp_copy (r, x, len);
	rc = mp_addmul_1 (r, m, len, mu * r[0], 0);
	mp_rshift_word (r, r, len, rc);

	for (i = 1; i < len; ++i) {
		rc = mp_addmul_1 (r, m, len, mu * r[0], 0);
		mp_rshift_word (r, r, len, rc);
	}

	if (mp_cmp_n (r, m, len) >= 0)
		mp_sub_n (r, r, m, len, 0);
}
