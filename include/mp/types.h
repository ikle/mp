/*
 * MP Core Types
 *
 * Copyright (c) 2014-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_TYPES_H
#define MP_PYPES_H  1

#include <limits.h>
#include <stdint.h>

/*
 * NOTE: The digit pair type is used by generic multiplication and
 * division only
 */

#ifndef MP_DIGIT_TYPE

#if ULONG_MAX > 0xfffffffful && defined (__SIZEOF_INT128__)

#define MP_DIGIT_BITS	64
#define MP_DIGIT_TYPE	uint_least64_t
#define MP_PAIR_TYPE	unsigned __int128

#elif UINT_FAST32_MAX > ULONG_MAX  /* LLP64 ABI? */

#define MP_DIGIT_BITS	(CHAR_BIT * sizeof (uint_fast32_t))
#define MP_DIGIT_TYPE	uint_fast32_t

#else

#define MP_DIGIT_BITS	32
#define MP_DIGIT_TYPE	uint_least32_t
#define MP_PAIR_TYPE	uint_least64_t

#endif
#endif  /* MP_DIGIT_TYPE */

/*
 * Core types to represent multiple-precission number digits
 */
typedef MP_DIGIT_TYPE	digit_t;

#ifdef MP_PAIR_TYPE
typedef MP_PAIR_TYPE	digit_pair_t;
#endif

#endif  /* MP_TYPES_H */
