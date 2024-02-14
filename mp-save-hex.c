/*
 * MP Core Conversion from Hex String
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/conv.h>
#include <mp/core.h>
#include <mp/digit.h>

#define MP_DIGIT_NIBS  (MP_DIGIT_BITS / 4)

static const char map[] = "0123456789abcdef";

size_t mp_save_hex (char *n, size_t avail, const digit_t *x, size_t len)
{
	size_t count, i;
	digit_t h;

	if ((len = mp_normalize (x, len)) == 0)
		count = 1;
	else
		count = len * MP_DIGIT_NIBS - mp_digit_clz (x[len-1]) / 4;

	if ((count + 1) > avail)
		return count + 1;

	if (len == 0)
		n[0] = '0';
	else
		for (i = 0; i < count; ++i) {
			h = x[i / MP_DIGIT_NIBS] >> ((i % MP_DIGIT_NIBS) * 4);
			n[count - 1 - i] = map[h & 15];
		}

	n[count] = '\0';
	return count + 1;
}
