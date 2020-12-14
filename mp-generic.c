/*
 * MP Core Generic Implemention
 *
 * Copyright (c) 2014-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include <mp/core.h>
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

char mp_add_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len,
	       int c)
{
	size_t i;

	for (i = 0; i < len; ++i)
		c = mp_digit_adc (r + i, x[i], y[i], c);

	return c;
}

char mp_add_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	int c;

	c = mp_digit_add (r, x[0], y);

	for (i = 1; i < len; ++i)
		c = mp_digit_adc (r + i, x[i], 0, c);

	return c;
}

char mp_add (digit_t *r, const digit_t *x, size_t xlen,
			 const digit_t *y, size_t ylen, int c)
{
	size_t i;

	for (i = 0; i < ylen; ++i)
		c = mp_digit_adc (r + i, x[i], y[i], c);

	for (; i < xlen; ++i)
		c = mp_digit_adc (r + i, x[i], 0, c);

	return c;
}

char mp_sub_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len,
	       int c)
{
	size_t i;

	for (i = 0; i < len; ++i)
		c = mp_digit_sbb (r + i, x[i], y[i], c);

	return c;
}

char mp_sub_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	int c;

	c = mp_digit_sub (r, x[0], y);

	for (i = 1; i < len; ++i)
		c = mp_digit_sbb (r + i, x[i], 0, c);

	return c;
}

char mp_sub (digit_t *r, const digit_t *x, size_t xlen,
			 const digit_t *y, size_t ylen, int c)
{
	size_t i;

	for (i = 0; i < ylen; ++i)
		c = mp_digit_sbb (r + i, x[i], y[i], c);

	for (; i < xlen; ++i)
		c = mp_digit_sbb (r + i, x[i], 0, c);

	return c;
}

char mp_neg (digit_t *r, const digit_t *x, size_t len)
{
	size_t i;
	int c = 0;

	for (i = 0; i < len; ++i)
		c = mp_digit_sbb (r + i, 0, x[i], c);

	return c;
}

int mp_cmp_n (const digit_t *x, const digit_t *y, size_t len)
{
	for (; len > 0; --len) {
		if (x[len - 1] > y[len - 1])
			return 1;

		if (x[len - 1] < y[len - 1])
			return -1;
	}

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
		     digit_t c)
{
	size_t i;
	digit_t a;

#ifdef MP_INSECURE
	if (y <= 1)
		return y < 1 ? 0 : mp_add_n (r, r, x, len, c);
#endif

	for (i = 0; i < len; ++i) {
		mp_digit_fma (&c, &a, x[i], y, c);
		c += mp_digit_add (r + i, r[i], a);
	}

	return c;
}

digit_t mp_submul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y,
		     digit_t c)
{
	size_t i;
	digit_t a;

#ifdef MP_INSECURE
	if (y <= 1)
		return y < 1 ? 0 : mp_sub_n (r, r, x, len, c);
#endif

	for (i = 0; i < len; ++i) {
		mp_digit_fma (&c, &a, x[i], y, c);
		c += mp_digit_sub (r + i, r[i], a);
	}

	return c;
}

digit_t mp_div_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	digit_t rem;

	for (i = len, rem = 0; i > 0; --i)
		mp_digit_div (r + i - 1, &rem, rem, x[i - 1], y);

	return rem;
}
