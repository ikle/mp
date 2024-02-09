/*
 * MP Core Multiplication: Generic Implemention
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/digit.h>
#include <mp/mul.h>

digit_t mp_mul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	digit_t c;

	for (i = 0, c = 0; i < len; ++i)
		mp_digit_fma (&c, r + i, x[i], y, c);

	return c;
}
