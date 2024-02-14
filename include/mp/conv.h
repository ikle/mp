/*
 * MP Core Conversions
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_CONV_H
#define MP_CONV_H  1

#include <mp/types.h>

/*
 * Function mp_load_hex loads a number from a string in hexadecimal notation
 * if there is enough space, and in any case returns the number of
 * significant digits.
 *
 * Function mp_save_hex stores the number into a string in hexadecimal
 * notation if there is enough space, and in any case returns the length
 * of the string, including the terminating NUL.
 */
size_t mp_load_hex (digit_t *x, size_t avail, const char *n);
size_t mp_save_hex (char *n, size_t avail, const digit_t *x, size_t len);

#endif  /* MP_SHIFT_H */

