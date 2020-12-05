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

char mp_add_n (mp_word *r, const mp_word *x, const mp_word *y, size_t count);
char mp_add_1 (mp_word *r, const mp_word *x, size_t count, mp_word y);
char mp_add (mp_word *r, const mp_word *x, size_t xlen,
			 const mp_word *y, size_t ylen);

char mp_sub_n (mp_word *r, const mp_word *x, const mp_word *y, size_t count);
char mp_sub_1 (mp_word *r, const mp_word *x, size_t count, mp_word y);
char mp_sub (mp_word *r, const mp_word *x, size_t xlen,
			 const mp_word *y, size_t ylen);

char mp_neg (mp_word *r, const mp_word *x, size_t count);

mp_word mp_mul_1 (mp_word *r, const mp_word *x, size_t count, mp_word y);
mp_word mp_addmul_1 (mp_word *r, const mp_word *x, size_t count, mp_word y);

/* xlen >= ylen > 0 */
mp_word mp_mul (mp_word *r, const mp_word *x, size_t xlen,
			    const mp_word *y, size_t ylen);

#endif  /* MP_CORE_H */
