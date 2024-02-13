/*
 * MP Core Conversion from Hex String
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include <mp/conv.h>

#define MP_DIGIT_NIBS  (MP_DIGIT_BITS / 4)

size_t mp_load_hex (digit_t *x, size_t avail, const char *n)
{
	const size_t count = strlen (n);
	const size_t len = (count + (MP_DIGIT_NIBS - 1)) / MP_DIGIT_NIBS;
	size_t i, nib;
	char a;
	digit_t h;

	if (len > avail)
		return len;

	for (i = 0; i < count; ++i) {
		a = n[count - 1 - i];
		h = a >= 'a' ? a - 'a'+10 : a >= 'A' ? a - 'A'+10 : a - '0';

		if ((nib = i % MP_DIGIT_NIBS) == 0)
			x[i / MP_DIGIT_NIBS] = h;
		else
			x[i / MP_DIGIT_NIBS] |= h << (nib * 4);
	}

	return len;
}
