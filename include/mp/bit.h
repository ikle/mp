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

#define MP_BITWISE_ONE(name, op)					\
static inline digit_t mp_digit_##name (digit_t x)			\
{									\
	return op;							\
}									\
									\
static inline void mp_##name (digit_t *r, const digit_t *x, size_t len)	\
{									\
	size_t i;							\
									\
	for (i = 0; i < len; ++i)					\
		r[i] = mp_digit_##name (x[i]);				\
}

#define MP_BITWISE_TWO(name, op)					\
static inline digit_t mp_digit_##name (digit_t x, digit_t y)		\
{									\
	return op;							\
}									\
									\
static inline void mp_##name (digit_t *r, const digit_t *x,		\
			      const digit_t *y, size_t len)		\
{									\
	size_t i;							\
									\
	for (i = 0; i < len; ++i)					\
		r[i] = mp_digit_##name (x[i], y[i]);			\
}

/*
 * The function mp_join joins set of bits (x, len) and (y, len), and stores
 * result into (r, len). In other words, it computes the bitwise OR.
 *
 * See also: Bounded lattice (algebra), Boolean algebra
 */
MP_BITWISE_TWO (join, x | y)

/*
 * The function mp_meet meets set of bits (x, len) and (y, len), and stores
 * result into (r, len). In other words, it computes the bitwise AND.
 *
 * See also: Bounded lattice (algebra), Boolean algebra
 */
MP_BITWISE_TWO (meet, x & y)

/*
 * The function mp_comp complements set of bits (x, len), and stores result
 * into (r, len). In other words, it computes the bitwise NOT (inversion).
 *
 * See also: Complemented lattice (algebra), Boolean algebra
 */
MP_BITWISE_ONE (comp, ~x)

/*
 * The function mp_impl calculates the implication of set of bits (x, len)
 * and (y, len) -- Y implies X, and stores result into (r, len). In other
 * words, it computes the bitwise OR-NOT: R = ~X | Y.
 *
 * See also: Heyting algebra, Boolean algebra
 */
MP_BITWISE_TWO (impl, ~x | y)

/*
 * The function mp_diff calculates the difference of set of bits (x, len)
 * and (y, len), and stores result into (r, len). In other words, it
 * computes the bitwise AND-NOT: R = X & ~Y.
 *
 * See also: Set theory, Boolean algebra
 */
MP_BITWISE_TWO (diff, x & ~y)

/*
 * The function mp_xor calculates the symmetric difference of set of bits
 * (x, len) and (y, len), and stores result into (r, len). In other words,
 * it computes the bitwise XOR.
 *
 * See also: Boolean ring
 */
MP_BITWISE_TWO (xor, x ^ y)

#endif  /* MP_BIT_H */
