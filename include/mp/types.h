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

#ifndef MP_DIGIT_BITS

#if ULONG_MAX > 0xfffffffful && defined (__SIZEOF_INT128__)

typedef uint64_t digit_t;
typedef unsigned __int128 digit_pair_t;

#define MP_DIGIT_BITS  64
#define MP_HAVE_PAIR_TYPE

#elif UINT_FAST32_MAX > ULONG_MAX  /* LLP64 ABI? */

typedef uint_fast32_t digit_t;

#define MP_DIGIT_BITS  (CHAR_BIT * sizeof (digit_t))

#else

typedef uint_least32_t digit_t;
typedef uint_least64_t digit_pair_t;

#define MP_DIGIT_BITS  32
#define MP_HAVE_PAIR_TYPE

#endif
#endif  /* MP_DIGIT_BITS */

#endif  /* MP_TYPES_H */
