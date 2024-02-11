/*
 * MP Core Addition
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_ADD_H
#define MP_ADD_H  1

#include <mp/types.h>

/*
 * Function mp_add_n adds (y, len) to (x, len), stores result into (r, len),
 * and returns the carry value.
 *
 * Function mp_add_1 adds y to (x, len), stores result into (r, len), and
 * returns the carry value.
 *
 * Function mp_add adds (y, ylen) to (x, xlen), stores result into (r, xlen),
 * and returns the carry value. Constraint: xlen >= ylen.
 */
char mp_add_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len,
	       int c);
char mp_add_1 (digit_t *r, const digit_t *x, size_t len, digit_t y);
char mp_add   (digit_t *r, const digit_t *x, size_t xlen,
			   const digit_t *y, size_t ylen, int c);
/*
 * Function mp_sub_n substracts (y, len) from (x, len), stores result into
 * (r, len), and returns the borrow value.
 *
 * Function mp_sub_1 substracts y from (x, len), stores result into (r, len),
 * and returns the borrow value.
 *
 * Function mp_sub substracts (y, ylen) from (x, xlen), stores result into
 * (r, len), and returns the borrow value. Constraint: xlen >= ylen.
 *
 * Function mp_neg negates (x, len), stores result into (r, len), and returns
 * the borrow value.
 *
 * Function mp_cmp_n compares (x, len) and (y, len), and returns an integer
 * less than, equal to, or greater than zero if x is found, respectively,
 * to be less than, to equal, or be greater than y.
 */
char mp_sub_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len,
	       int c);
char mp_sub_1 (digit_t *r, const digit_t *x, size_t count, digit_t y);
char mp_sub   (digit_t *r, const digit_t *x, size_t xlen,
			   const digit_t *y, size_t ylen, int c);
char mp_neg   (digit_t *r, const digit_t *x, size_t len);

int  mp_cmp_n (const digit_t *x, const digit_t *y, size_t len);

/*
 * Function mp_double doubles (x, len), stores retult into (r, len), and
 * returns the carry value.
 */
#ifndef mp_double
static inline char mp_double (digit_t *r, const digit_t *x, size_t len, int c)
{
	return mp_add_n (r, x, x, len, c);
}
#endif

#endif  /* MP_ADD_H */
