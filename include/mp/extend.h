/*
 * MP Core Zero and Sign Extension
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_EXTEND_H
#define MP_EXTEND_H  1

#include <mp/unit.h>

#define MP_DIGIT_SIGN_MASK  ((digit_t) 1 << (MP_DIGIT_BITS - 1))

/*
 * Function mp_is_neg returns nonzero if x is negative in two's complement,
 * zero otherwise. Constraint: len > 0.
 */
static inline mp_is_neg (const digit_t *x, size_t len)
{
	return (x[len - 1] & MP_DIGIT_SIGN_MASK) != 0;
}

/*
 * Function mp_zext extends (x, xlen) with zeros and stores the result
 * into (r, rlen). Constraint: rlen >= xlen.
 */
static inline
void mp_zext (digit_t *r, size_t rlen, const digit_t *x, size_t xlen)
{
	mp_copy (r, x, xlen);
	mp_zero (r + xlen, rlen - xlen);
}

/*
 * Function mp_sext performs the signed two's complement extension of
 * (x, xlen) and stores the result into (r, rlen).
 * Constraint: rlen >= xlen > 0.
 */
static inline
void mp_sext (digit_t *r, size_t rlen, const digit_t *x, size_t xlen)
{
	const int fill = mp_is_neg (x, xlen) ? ~0 : 0;

	mp_copy (r, x, xlen);
	mp_fill (r + xlen, fill, rlen - xlen);
}

#endif  /* MP_EXTEND_H */
