/*
 * MP Core Types
 *
 * Copyright (c) 2014-2018 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_TYPES_H
#define MP_PYPES_H  1

#include <limits.h>
#include <stdint.h>

/* NOTE: mp_dword type is used by generic multiplication only */

#ifndef MP_WORD_BITS

#if ULONG_MAX > 0xfffffffful && defined(__SIZEOF_INT128__)

typedef uint64_t mp_word;
typedef unsigned __int128 mp_dword;

#define MP_WORD_SIZE  8
#define MP_WORD_BITS  64

#elif defined(_M_AMD64) || defined(_M_IA64)

typedef uint64_t mp_word;

#define MP_WORD_SIZE  8
#define MP_WORD_BITS  64

#else

typedef uint32_t mp_word;
typedef uint64_t mp_dword;

#define MP_WORD_SIZE  4
#define MP_WORD_BITS  32

#endif
#endif  /* MP_WORD_BITS */

#endif  /* MP_TYPES_H */
