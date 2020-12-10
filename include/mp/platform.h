/*
 * MP Platform-specific definitions
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_PLATFORM_H
#define MP_PLATFORM_H  1

#include <mp/types.h>
#include <mp/compiler.h>
#include <mp/primitive.h>

/*
 * Core functions
 */

static inline
digit_t mp_word_mul_add (digit_t *r, digit_t x, digit_t y, digit_t a)
{
#ifdef MP_UFMA
	return MP_UFMA (r, x, y, a);
#else
	digit_t l, h;

	mp_digit_mul (&h, &l, x, y);

	return h + mp_digit_add (r, l, a);
#endif
}

#endif  /* MP_PLATFORM_H */
