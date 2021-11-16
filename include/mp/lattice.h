/*
 * MP Core Lattice Operations
 *
 * Copyright (c) 2007-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_LATTICE_H
#define MP_LATTICE_H  1

#include <stddef.h>

#include <mp/types.h>

/*
 * The function mp_join performs the lattice join operation, or,
 * in other words, computes the bitwise or.
 */
static inline
void mp_join (digit_t *r, const digit_t *x, const digit_t *y, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		r[i] = x[i] | y[i];
}

/*
 * The function mp_meet performs the lattice meet operation, or,
 * in other words, computes the bitwise and.
 */
static inline
void mp_meet (digit_t *r, const digit_t *x, const digit_t *y, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		r[i] = x[i] & y[i];
}

/*
 * The function mp_comp performs the lattice complement operation, or,
 * in other words, computes the bitwise inversion.
 */
static inline void mp_comp (digit_t *r, const digit_t *x, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		r[i] = ~x[i];
}

#endif  /* MP_LATTICE_H */
