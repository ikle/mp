/*
 * MP Core Addition: Comparision Generic Implemention
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/digit.h>

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
