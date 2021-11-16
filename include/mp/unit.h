/*
 * MP Core Unit
 *
 * Copyright (c) 2014-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_UNIT_H
#define MP_UNIT_H  1

#include <string.h>

#include <mp/types.h>

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

#endif  /* MP_UNIT_H */
