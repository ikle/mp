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

int  mp_cmp_n (const digit_t *x, const digit_t *y, size_t len);

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
 */
digit_t mp_mul_1    (digit_t *r, const digit_t *x, size_t len, digit_t y);
digit_t mp_addmul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y);
digit_t mp_submul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y);
void    mp_mul      (digit_t *r, const digit_t *x, size_t xlen,
				 const digit_t *y, size_t ylen);
/*
 * Function mp_div_1 divides (x, len) by y, stores result into (r, len),
 * and returns the remaider value.
 *
 * Function mp_div divides (n, nlen) by (d, dlen), stores quotinent into
 * (q, nlen - dlen + 1) and remaider into (r, nlen), and returns the size of
 * remaider. Constrains: n and d are normalized and nlen >= dlen > 0.
 */
digit_t mp_div_1 (digit_t *r, const digit_t *x, size_t len, digit_t y);

size_t mp_div (digit_t *q, digit_t *r, const digit_t *n, size_t nlen,
				       const digit_t *d, size_t dlen);

#endif  /* MP_CORE_H */
