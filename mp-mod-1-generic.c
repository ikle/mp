/*
 * MP Core Division: Generic Implemention
 *
 * Copyright (c) 2014-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/digit.h>
#include <mp/div.h>

digit_t mp_mod_1 (const digit_t *x, size_t len, digit_t y)
{
	size_t i;
	digit_t q, rem;

	for (i = len, rem = 0; i > 0; --i)
		mp_digit_div (&q, &rem, rem, x[i - 1], y);

	return rem;
}
