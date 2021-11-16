/*
 * MP Core Division
 *
 * Copyright (c) 2014-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_DIV_H
#define MP_DIV_H  1

#include <stddef.h>

#include <mp/types.h>

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

#endif  /* MP_DIV_H */
