/*
 * MP Core Generic Implemention
 *
 * Copyright (c) 2014-2018 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include <mp/core.h>
#include <mp/platform.h>

static inline mp_word mp_zero (mp_word *r, size_t count)
{
	memset (r, 0, count * sizeof (*r));
	return 0;
}

static inline mp_word mp_copy (mp_word *r, const mp_word *x, size_t count)
{
	memcpy (r, x, count * sizeof (*r));
	return 0;
}

char mp_add_n (mp_word *r, const mp_word *x, const mp_word *y, size_t count)
{
	size_t i;
	int c = 0;

	for (i = 0; i < count; ++i)
		c = mp_word_add_carry (r + i, c, x[i], y[i]);

	return c;
}

char mp_add_1 (mp_word *r, const mp_word *x, size_t count, mp_word y)
{
	size_t i;
	int c;

	c = mp_word_add (r, x[0], y);

	for (i = 1; i < count; ++i)
		c = mp_word_add_carry (r + i, c, x[i], 0);

	return c;
}

char mp_add (mp_word *r, const mp_word *x, size_t xlen,
			 const mp_word *y, size_t ylen)
{
	size_t i;
	int c = 0;

	for (i = 0; i < ylen; ++i)
		c = mp_word_add_carry (r + i, c, x[i], y[i]);

	for (i = 0; i < xlen; ++i)
		c = mp_word_add_carry (r + i, c, x[i], 0);

	return c;
}

char mp_sub_n (mp_word *r, const mp_word *x, const mp_word *y, size_t count)
{
	size_t i;
	int c = 0;

	for (i = 0; i < count; ++i)
		c = mp_word_sub_borrow (r + i, c, x[i], y[i]);

	return c;
}

char mp_sub_1 (mp_word *r, const mp_word *x, size_t count, mp_word y)
{
	size_t i;
	int c;

	c = mp_word_sub (r, x[0], y);

	for (i = 1; i < count; ++i)
		c = mp_word_sub_borrow (r + i, c, x[i], 0);

	return c;
}

char mp_sub (mp_word *r, const mp_word *x, size_t xlen,
			 const mp_word *y, size_t ylen)
{
	size_t i;
	int c = 0;

	for (i = 0; i < ylen; ++i)
		c = mp_word_sub_borrow (r + i, c, x[i], y[i]);

	for (i = 0; i < xlen; ++i)
		c = mp_word_sub_borrow (r + i, c, x[i], 0);

	return c;
}

char mp_neg (mp_word *r, const mp_word *x, size_t count)
{
	size_t i;
	int c = 0;

	for (i = 0; i < count; ++i)
		c = mp_word_sub_borrow (r + i, c, 0, x[i]);

	return c;
}

mp_word mp_mul_1 (mp_word *r, const mp_word *x, size_t count, mp_word y)
{
	size_t i;
	mp_word c;

#if 0
	if (y <= 1)
		return y < 1 ? mp_zero (r, count) : mp_copy (r, x, count);
#endif

	for (i = 0, c = 0; i < count; ++i)
		c = mp_word_mul_add (r + i, x[i], y, c);

	return c;
}

mp_word mp_addmul_1 (mp_word *r, const mp_word *x, size_t count, mp_word y)
{
	size_t i;
	mp_word c, a;

#if 0
	if (y <= 1)
		return y < 1 ? 0 : mp_add_n (r, r, x, count);
#endif

	for (i = 0, c = 0; i < count; ++i) {
		c  = mp_word_mul_add (&a, x[i], y, c);
		c += mp_word_add (r + i, r[i], a);
	}

	return c;
}
