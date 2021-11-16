/*
 * MP Core Generic Implemention
 *
 * Copyright (c) 2014-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include <mp/core.h>
#include <mp/digit.h>

digit_t mp_div_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	digit_t rem;

	for (i = len, rem = 0; i > 0; --i)
		mp_digit_div (r + i - 1, &rem, rem, x[i - 1], y);

	return rem;
}
