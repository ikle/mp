/*
 * MP Core Multiplication Generic Implemention
 *
 * Copyright (c) 2014-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include <mp/mul.h>
#include <mp/digit.h>

static inline digit_t mp_zero (digit_t *r, size_t len)
{
	memset (r, 0, len * sizeof (*r));
	return 0;
}

static inline digit_t mp_copy (digit_t *r, const digit_t *x, size_t len)
{
	memcpy (r, x, len * sizeof (*r));
	return 0;
}

digit_t mp_mul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	digit_t c;

#ifdef MP_INSECURE
	if (y <= 1)
		return y < 1 ? mp_zero (r, len) : mp_copy (r, x, len);
#endif

	for (i = 0, c = 0; i < len; ++i)
		mp_digit_fma (&c, r + i, x[i], y, c);

	return c;
}

digit_t mp_addmul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y,
		     int c)
{
	size_t i;
	digit_t r1 = c, r0;

#ifdef MP_INSECURE
	if (y <= 1)
		return y < 1 ? 0 : mp_add_n (r, r, x, len, c);
#endif

	for (i = 0; i < len; ++i) {
		mp_digit_fma (&r1, &r0, x[i], y, r1);
		r1 += mp_digit_add (r + i, r[i], r0);
	}

	return r1;
}

digit_t mp_submul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y,
		     int c)
{
	size_t i;
	digit_t r1 = c, r0;

#ifdef MP_INSECURE
	if (y <= 1)
		return y < 1 ? 0 : mp_sub_n (r, r, x, len, c);
#endif

	for (i = 0; i < len; ++i) {
		mp_digit_fma (&r1, &r0, x[i], y, r1);
		r1 += mp_digit_sub (r + i, r[i], r0);
	}

	return r1;
}
