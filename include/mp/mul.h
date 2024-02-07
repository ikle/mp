/*
 * MP Core Multiplication
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_MUL_H
#define MP_MUL_H  1

#include <mp/types.h>

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

#endif  /* MP_MUL_H */
