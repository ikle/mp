/*
 * Multiple-Precission Digit Primitives
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef MP_DIGIT_H
#define MP_DIGIT_H  1

#include <mp/types.h>
#include <mp/compiler.h>

/*
 * Function mp_digit_add adds x and y, stores result into r and returns
 * the carry value.
 */
#ifndef mp_digit_add
static inline char mp_digit_add (digit_t *r, digit_t x, digit_t y)
{
#ifdef mp_digit_adc
	return mp_digit_adc (r, x, y, 0);
#else
	*r = x + y;
	return *r < x;
#endif
}
#endif

/*
 * Function mp_digit_sub substracts y from x, stores result into r and
 * returns the borrow value.
 */
#ifndef mp_digit_sub
static inline char mp_digit_sub (digit_t *r, digit_t x, digit_t y)
{
#ifdef mp_digit_sbb
	return mp_digit_sbb (r, x, y, 0);
#else
	*r = x - y;
	return *r > x;
#endif
}
#endif

/*
 * Function mp_digit_adc adds x and y with input carry c, stores result
 * into r and returns the output carry value.
 */
#ifndef mp_digit_adc
static inline char mp_digit_adc (digit_t *r, digit_t x, digit_t y, int c)
{
	digit_t a;

	c = mp_digit_add (&a, y, c);
	return c + mp_digit_add (r, x, a);
}
#endif

/*
 * Function mp_digit_sbb substracts y from x with input borrow c, stores
 * result into r and returns the output borrow value.
 */
#ifndef mp_digit_sbb
static inline char mp_digit_sbb (digit_t *r, digit_t x, digit_t y, int c)
{
	digit_t a;

	c = mp_digit_add (&a, y, c);
	return c + mp_digit_sub (r, x, a);
}
#endif

/*
 * If digit pair type defined then define constructor for it
 */
#ifdef MP_PAIR_TYPE
#define MP_PAIR_MAKE(hi, lo)	(((digit_pair_t) (hi) << MP_DIGIT_BITS) | (lo))
#endif

/*
 * Function mp_digit_mul multiplies x by y and stores result into pair
 * (r1, r0), where r1 * B + r0 = x * y.
 */
#ifndef mp_digit_mul
static inline void mp_digit_mul (digit_t *r1, digit_t *r0, digit_t x, digit_t y)
{
#ifdef MP_PAIR_TYPE
	digit_pair_t pair = (digit_pair_t) x * y;

	*r1 = pair >> MP_DIGIT_BITS;
	*r0 = pair;
#else
#error "Platform does not define multiplication primitive and digit pair type"
#endif
}
#endif

/*
 * Function mp_digit_div divides pair (n1 * B + n0) by d and stores quotient
 * into q and remainder into r.
 */
#ifndef mp_digit_div
static inline
void mp_digit_div (digit_t *q, digit_t *r, digit_t n1, digit_t n0, digit_t d)
{
#ifdef MP_PAIR_TYPE
	digit_pair_t pair = MP_PAIR_MAKE (n1, n0);

	*q = pair / d;
	*r = pair % d;
#else
#error "Platform does not define division primitive and digit pair type"
#endif
}
#endif

/*
 * Function mp_digit_fma multiplies x by y, adds a to the resulting product
 * and stores result into pair (r1, r0), where r1 * B + r0 = x * y + a.
 */
#ifndef mp_digit_fma
static inline
void mp_digit_fma (digit_t *r1, digit_t *r0, digit_t x, digit_t y, digit_t a)
{
	mp_digit_mul (r1, r0, x, y);
	*r1 += mp_digit_add (r0, *r0, a);
}
#endif

/*
 * Function mp_digit_clz counts leading zeroes in x and returns it as
 * a result of function. Note that the value of the function from 0 is
 * generally undefined.
 */
#ifndef mp_digit_clz
#define NEED_MP_CLZ_TABLE
extern unsigned char mp_clz_table[];

static inline int mp_digit_clz (digit_t x)
{
	int shift = 0;

	switch (MP_DIGIT_BITS) {
	case 64:
		shift += x < 0x100000000000000ULL;
		shift += x < 0x1000000000000ULL;
		shift += x < 0x10000000000ULL;
		shift += x < 0x100000000ULL;
	case 32:
		shift += x < 0x1000000UL;
		shift += x < 0x10000UL;
	case 16:
		shift += x < 0x100U;
	}

	shift *= 8;

	return shift + mp_clz_table[(x >> (MP_DIGIT_BITS - 8 - shift)) & 0xff];
}
#endif

/*
 * Function mp_digit_ctz counts trailing zeroes in x and returns it as
 * a result of function. Note that the value of the function from 0 is
 * generally undefined.
 */
#ifndef mp_digit_ctz
static inline int mp_digit_ctz (digit_t x)
{
	return MP_DIGIT_BITS - 1 - mp_digit_clz (x & -x);
}
#endif

#endif  /* MP_DIGIT_H */
