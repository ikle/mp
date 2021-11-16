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
#include <mp/mul.h>
#include <mp/div.h>
#include <mp/unit.h>

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

#endif  /* MP_CORE_H */
