/*
 * MP Core Modular Arithmetics
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_MOD_H
#define MP_MOD_H  1

#include <mp/add.h>

/*
 * Function mp_mod_add_n adds Y = (y, len) to X = (x, len) modulo M =
 * (m, len), and stores result into (r, len). Constraints: X < M, y < M.
 *
 * Function mp_mod_add_n substracts Y = (y, len) from X = (x, len) modulo
 * M = (m, len), and stores result into (r, len). Constraints: X < M, y < M.
 */
static inline
void mp_mod_add_n (digit_t * r, const digit_t *x, const digit_t *y,
		   const digit_t *m, size_t len)
{
	if (mp_add_n (r, x, y, len, 0) != 0 || mp_cmp_n (r, m, len) >= 0)
		mp_sub_n (r, r, m, len, 0);
}

static inline
void mp_mod_sub_n (digit_t * r, const digit_t *x, const digit_t *y,
		   const digit_t *m, size_t len)
{
	if (mp_sub_n (r, x, y, len, 0) != 0)
		mp_add_n (r, r, m, len, 0);
}

#endif  /* MP_MOD_H */
