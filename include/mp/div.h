/*
 * MP Core Division
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_DIV_H
#define MP_DIV_H  1

#include <mp/types.h>

/*
 * Function mp_div_1 divides (x, len) by y, stores result into (r, len),
 * and returns the remainder value.
 *
 * Function mp_mod_1 divides (x, len) by y, and returns the remainder value.
 *
 * Function mp_div divides (n, nlen) by (d, dlen), stores quotinent into
 * (q, nlen - dlen + 1), remainder into (r, nlen), and returns the size
 * of remainder. Constraints: n and d are normalized, the most significant
 * bit of d is set, and nlen >= dlen > 0. Note that the remainder is not
 * normalized.
 *
 * Tip: Normalize n and d, and then shift n and d left by clz(d) before
 * calling the mp_div function.
*/
digit_t mp_div_1 (digit_t *r, const digit_t *x, size_t len, digit_t y);
digit_t mp_mod_1 (const digit_t *x, size_t len, digit_t y);

size_t mp_div (digit_t *q, digit_t *r, const digit_t *n, size_t nlen,
				       const digit_t *d, size_t dlen);

#endif  /* MP_DIV_H */
