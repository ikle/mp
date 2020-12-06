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

char mp_add_n (digit_t *r, const digit_t *x, const digit_t *y, size_t count);
char mp_add_1 (digit_t *r, const digit_t *x, size_t count, digit_t y);
char mp_add (digit_t *r, const digit_t *x, size_t xlen,
			 const digit_t *y, size_t ylen);

char mp_sub_n (digit_t *r, const digit_t *x, const digit_t *y, size_t count);
char mp_sub_1 (digit_t *r, const digit_t *x, size_t count, digit_t y);
char mp_sub (digit_t *r, const digit_t *x, size_t xlen,
			 const digit_t *y, size_t ylen);

char mp_neg (digit_t *r, const digit_t *x, size_t count);

digit_t mp_mul_1 (digit_t *r, const digit_t *x, size_t count, digit_t y);
digit_t mp_addmul_1 (digit_t *r, const digit_t *x, size_t count, digit_t y);

/* xlen >= ylen > 0 */
digit_t mp_mul (digit_t *r, const digit_t *x, size_t xlen,
			    const digit_t *y, size_t ylen);

#endif  /* MP_CORE_H */
