/*
 * MP Core Multiplication
 *
 * Copyright (c) 2018 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include "mp-core.h"

static inline
mp_word mp_zero (mp_word *r, size_t count)
{
	memset (r, 0, count * MP_WORD_SIZE);
	return 0;
}

static inline
mp_word mp_copy (mp_word *r, const mp_word *x, size_t count)
{
	memcpy (r, x, count * MP_WORD_SIZE);
	return 0;
}

mp_word mp_mul (mp_word *r, const mp_word *x, size_t xlen,
			    const mp_word *y, size_t ylen)
{
	mp_word c, a = y[0];
	size_t i;

	if (ylen == 0)
		return 0;

#if 0
	c = a >  1 ? mp_mul_1 (r, x, xlen, a) :
	    a == 1 ? mp_copy (r, x, xlen) :
		     mp_zero (r, xlen);
#elif 0
	c = a <  1 ? mp_zero (r, xlen) :
	    a == 1 ? mp_copy (r, x, xlen) :
		     mp_mul_1 (r, x, xlen, a);
#else
	c = mp_mul_1 (r, x, xlen, a);
#endif

	r++[xlen] = c;

	for (i = 1; i < ylen; ++i) {
		a = y[i];

#if 0
		c = a >  1 ? mp_addmul_1 (r, x, xlen, a) :
		    a == 1 ? mp_add_n (r, r, x, xlen) :
			     0;
#elif 0
		c = a <  1 ? 0 :
		    a == 1 ? mp_add_n (r, r, x, xlen) :
			     mp_addmul_1 (r, x, xlen, a);
#else
		c = mp_addmul_1 (r, x, xlen, a);
#endif

		r++[xlen] = c;
	}

	return c;
}
