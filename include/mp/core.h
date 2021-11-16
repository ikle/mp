/*
 * MP Core
 *
 * Copyright (c) 2014-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_CORE_H
#define MP_CORE_H  1

#include <stddef.h>

#include <mp/add.h>

/*
 * Function mp_lshift multiplies (x, len) by (2 ^ count), stores result into
 * (r, len + extra), and returns the carry value. The count must be less
 * than MP_DIGIT_BITS.
 *
 * Function mp_rshift divides (x, len) by (2 ^ count) and stores result
 * into (r, len). The most significant bits will be cleared.
 */
digit_t mp_lshift (digit_t *r, const digit_t *x, size_t len, size_t count);
void    mp_rshift (digit_t *r, const digit_t *x, size_t len, size_t count);

/*
 * Function mp_mul_1 multiplies (x, len) by y, stores result into (r, len),
 * and returns the carry value.
 *
 * Function mp_addmul_1 multiplies (x, len) by y, adds result into (r, len),
 * and returns the carry value.
 *
 * Function mp_submul_1 multiplies (x, len) by y, substracts result from
 * (r, len), and returns the borrow value.
 *
 * Function mp_mul multiplies (x, xlen) by (y, ylen), stores result into
 * (r, xlen + ylen). Constrains: xlen >= ylen > 0.
 *
 * Function mp_mul_sb does the same as mp_mul, only using school book
 * algorithm exclusively. Exported for tests only.
 */
digit_t mp_mul_1    (digit_t *r, const digit_t *x, size_t len, digit_t y);
digit_t mp_addmul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y,
		     int c);
digit_t mp_submul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y,
		     int c);

void    mp_mul      (digit_t *r, const digit_t *x, size_t xlen,
				 const digit_t *y, size_t ylen);
void    mp_mul_sb   (digit_t *r, const digit_t *x, size_t xlen,
				 const digit_t *y, size_t ylen);
/*
 * Function mp_div_1 divides (x, len) by y, stores result into (r, len),
 * and returns the remainder value.
 *
 * Function mp_div divides (n, nlen) by (d, dlen), stores quotinent into
 * (q, nlen - dlen + 1) and remainder into (r, nlen), and returns the size
 * of remainder. Constrains: n and d are normalized and nlen >= dlen > 0.
 * Note that the remainder is not normalized.
 */
digit_t mp_div_1 (digit_t *r, const digit_t *x, size_t len, digit_t y);

size_t mp_div (digit_t *q, digit_t *r, const digit_t *n, size_t nlen,
				       const digit_t *d, size_t dlen);

#endif  /* MP_CORE_H */
