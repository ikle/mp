/*
 * MP Core Modular Arithmetics: Montgomery Multiplication
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_MONT_MUL_H
#define MP_MONT_MUL_H  1

#include <mp/mod.h>

/*
 * Constraints for all Montgomery multiplication methods: M is odd.
 *
 * Function mp_mont_mu computes the Montgomery constant mu = -M ^ -1 mod B,
 * where m0 is a low dogot of M, B = 2^w and w = MP_DIGIT_BITS. Subsequently,
 * the precalculated value of mu is used for all Montgomery multiplication
 * operations.
 *
 * Function mp_mont_ro computes ro = R^2 mod M, where M = (m, len), and
 * R = B ^ len. The precomputed value of ro is then used to convert a residue
 * module M from its regular to its Montgomery representation. Constraint:
 * the most significant bit of M is set.
 *
 * Function mp_mont_ro_gen does the same thing as function mp_mont_ro, but
 * with fewer restrictions. Constraint: The most significant digit of M is
 * not zero.
 *
 * Function mp_mont_push_n converts a residue (x, len) modulo (m, len)
 * from its regular to its Montgomery representation, and stores result
 * into (r, len). Constraint: X < M.
 *
 * Function mp_mont_pull_n converts a residue (x, len) modulo (m, len)
 * from its Montgomery to its regular representation, and stores result
 * into (r, len).
 *
 * Function mp_mont_mul_n multiplies (x, len) by (y, len) module (m, len),
 * where all X, Y and R are in Montgomery representation, and stores result
 * into (r, len). Constraints: X < M, Y < M.
 *
 * Function mp_mont_pow_n computes R * (X^Y) modulo M, where R = (r, len),
 * X = (x, len), Y = (y, len) and M = (m, len), and stores result into R.
 * Constraints: R and X operands are in Montgomery representation.
 *
 * Function mp_mont_pow_n_sec does the same thing as function mp_mont_pow_n,
 * but the number of operations depends only on the length of the numbers
 * (len) to prevent timing and Flush+Reload side-channel attacks.
 */
digit_t mp_mont_mu  (digit_t m0);
void mp_mont_ro     (digit_t *r, const digit_t *m, size_t len);
void mp_mont_ro_gen (digit_t *r, const digit_t *m, size_t len);

void mp_mont_mul_n  (digit_t *r, const digit_t *x, const digit_t *y,
		     const digit_t *m, size_t len, digit_t mu);
void mp_mont_pull_n (digit_t *r, const digit_t *x,
		     const digit_t *m, size_t len, digit_t mu);
static inline
void mp_mont_push_n (digit_t *r, const digit_t *x, const digit_t *ro,
		     const digit_t *m, size_t len, digit_t mu)
{
	mp_mont_mul_n (r, x, ro, m, len, mu);
}

void mp_mont_pow_n     (digit_t *r, const digit_t *x, const digit_t *y,
			const digit_t *m, size_t len, digit_t mu);
void mp_mont_pow_n_sec (digit_t *r, const digit_t *x, const digit_t *y,
			const digit_t *m, size_t len, digit_t mu);

#endif  /* MP_MONT_MUL_H */
