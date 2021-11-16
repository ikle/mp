/*
 * MP Core Bit Operations
 *
 * Copyright (c) 2007-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_BIT_H
#define MP_BIT_H  1

#include <stddef.h>

#include <mp/types.h>

/*
 * The function mp_join joins set of bits (x, len) and (y, len), and stores
 * result into (r, len). In other words, it computes the bitwise OR.
 *
 * See also: Bounded lattice (algebra), Boolean algebra
 */
static inline
void mp_join (digit_t *r, const digit_t *x, const digit_t *y, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		r[i] = x[i] | y[i];
}

/*
 * The function mp_meet meets set of bits (x, len) and (y, len), and stores
 * result into (r, len). In other words, it computes the bitwise AND.
 *
 * See also: Bounded lattice (algebra), Boolean algebra
 */
static inline
void mp_meet (digit_t *r, const digit_t *x, const digit_t *y, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		r[i] = x[i] & y[i];
}

/*
 * The function mp_comp complements set of bits (x, len), and stores result
 * into (r, len). In other words, it computes the bitwise NOT (inversion).
 *
 * See also: Complemented lattice (algebra), Boolean algebra
 */
static inline void mp_comp (digit_t *r, const digit_t *x, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		r[i] = ~x[i];
}

/*
 * The function mp_impl calculates the implication of set of bits (x, len)
 * and (y, len) -- Y implies X, and stores result into (r, len). In other
 * words, it computes the bitwise OR-NOT: R = ~X | Y.
 *
 * See also: Heyting algebra, Boolean algebra
 */
static inline
void mp_impl (digit_t *r, const digit_t *x, const digit_t *y, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		r[i] = ~x[i] | y[i];
}

/*
 * The function mp_diff calculates the difference of set of bits (x, len)
 * and (y, len), and stores result into (r, len). In other words, it
 * computes the bitwise AND-NOT: R = X & ~Y.
 *
 * See also: Set theory, Boolean algebra
 */
static inline
void mp_diff (digit_t *r, const digit_t *x, const digit_t *y, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		r[i] = x[i] & ~y[i];
}

#endif  /* MP_BIT_H */
