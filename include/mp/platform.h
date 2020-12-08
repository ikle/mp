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

/*
 * Core functions
 */

static inline
char mp_word_add (digit_t *r, digit_t x, digit_t y)
{
#if defined (MP_ADD)
	return MP_ADD (r, x, y);
#elif defined (MP_ADDC)
	return MP_ADDC (r, 0, x, y);
#else  
	*r = x + y;
	return *r < x;
#endif
}

static inline
char mp_word_sub (digit_t *r, digit_t x, digit_t y)
{
#if defined (MP_SUB)
	return MP_SUB (r, x, y);
#elif defined (MP_SUBB)
	return MP_SUBB (r, 0, x, y);
#else  
	*r = x - y;
	return *r > x;
#endif
}

static inline
char mp_word_add_carry (digit_t *r, int c, digit_t x, digit_t y)
{
#if defined (MP_ADDC)
	return MP_ADDC (r, c, x, y);
#else  
	digit_t a;

	c = mp_word_add (&a, y, c);
	return c + mp_word_add (r,  x, a);
#endif
}

static inline
char mp_word_sub_borrow (digit_t *r, int c, digit_t x, digit_t y)
{
#if defined (MP_SUBB)
	return MP_SUBB (r, c, x, y);
#else
	digit_t a;

	c = mp_word_add (&a, y, c);
	return c + mp_word_sub (r,  x, a);
#endif
}

static inline
digit_t mp_word_mul (digit_t *r, digit_t x, digit_t y)
{
#ifdef MP_UMUL
	return MP_UMUL (r, x, y);
#elif defined (MP_PAIR_TYPE)
	digit_pair_t a = (digit_pair_t) x * y;

	*r = a;
	return a >> MP_DIGIT_BITS;
#else
#error "Platform does not define multiplication primitive and digit pair type"
#endif
}

static inline
digit_t mp_word_mul_add (digit_t *r, digit_t x, digit_t y, digit_t a)
{
#ifdef MP_UFMA
	return MP_UFMA (r, x, y, a);
#else
	digit_t l, h = mp_word_mul (&l, x, y);

	return h + mp_word_add (r, l, a);
#endif
}

static inline
digit_t mp_word_div (digit_t *r, digit_t x1, digit_t x0, digit_t y)
{
#ifdef MP_UMUL
	return MP_UDIV (r, x1, x0, y);
#elif defined (MP_PAIR_TYPE)
	digit_pair_t pair = ((digit_pair_t) x1 << MP_DIGIT_BITS) | x0;

	*r = pair / y;
	return pair % y;
#else
#error "Platform does not define division primitive and digit pair type"
#endif
}

#endif  /* MP_PLATFORM_H */
