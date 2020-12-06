/*
 * MP Core
 *
 * Copyright (c) 2014-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_CORE_H
#define MP_CORE_H  1

#include <stddef.h>

#include <mp/types.h>

/*
 * Function mp_add_n adds (y, len) to (x, len), stores result into (r, len),
 * and returns the carry value.
 *
 * Function mp_add_1 adds y to (x, len), stores result into (r, len), and
 * returns the carry value.
 *
 * Function mp_add adds (y, ylen) to (x, xlen), stores result into (r, xlen),
 * and returns the carry value.
 */
char mp_add_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len);
char mp_add_1 (digit_t *r, const digit_t *x, size_t len, digit_t y);
char mp_add   (digit_t *r, const digit_t *x, size_t xlen,
			   const digit_t *y, size_t ylen);
/*
 * Function mp_sub_n substracts (y, len) from (x, len), stores result into
 * (r, len), and returns the borrow value.
 *
 * Function mp_sub_1 substracts y from (x, len), stores result into (r, len),
 * and returns the borrow value.
 *
 * Function mp_sub substracts (y, ylen) from (x, xlen), stores result into
 * (r, len), and returns the borrow value.
 *
 * Function mp_neg negates (x, len), stores result into (r, len), and returns
 * the borrow value.
 */
char mp_sub_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len);
char mp_sub_1 (digit_t *r, const digit_t *x, size_t count, digit_t y);
char mp_sub   (digit_t *r, const digit_t *x, size_t xlen,
			   const digit_t *y, size_t ylen);
char mp_neg   (digit_t *r, const digit_t *x, size_t len);

/*
 * Function mp_mul_1 multiplies (x, len) by y, stores result into (r, len),
 * and returns the carry value.
 *
 * Function mp_addmul_1 multiplies (x, len) by y, adds result into (r, len),
 * and returns the carry value.
 *
 * Function mp_mul multiplies (x, xlen) by (y, ylen), stores result into
 * (r, xlen + ylen). Constrains: xlen >= ylen > 0.
 */
digit_t mp_mul_1    (digit_t *r, const digit_t *x, size_t len, digit_t y);
digit_t mp_addmul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y);
void    mp_mul      (digit_t *r, const digit_t *x, size_t xlen,
				 const digit_t *y, size_t ylen);
/*
 * Function mp_div_1 divides (x, len) by y, stores result into (r, len),
 * and returns the remaider value.
 */
digit_t mp_div_1 (digit_t *r, const digit_t *x, size_t len, digit_t y);

#endif  /* MP_CORE_H */
