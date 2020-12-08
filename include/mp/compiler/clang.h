/*
 * Clang specific definitions
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef __clang__
#error "This file must be included for Clang compiler only"
#endif

#ifndef MP_COMPILER_CLANG_H
#define MP_COMPILER_CLANG_H  1

#include <mp/types.h>

#ifndef MP_ADDC
#if __has_builtin (__builtin_addcl)

#if MP_DIGIT_BITS == (sizeof (int) * CHAR_BIT)
#define MP_CLANG_ADDC	__builtin_addc
#define MP_CLANG_SUBB	__builtin_subc
#elif MP_DIGIT_BITS == (sizeof (long) * CHAR_BIT)
#define MP_CLANG_ADDC	__builtin_addcl
#define MP_CLANG_SUBB	__builtin_subcl
#else
#define MP_CLANG_ADDC	__builtin_addcll
#define MP_CLANG_SUBB	__builtin_subcll
#endif

static inline digit_t mp_clang_addc (digit_t *r, int c, digit_t x, digit_t y)
{
	digit_t cout;

	*r = MP_CLANG_ADDC (x, y, c, &cout);
	return cout;
}

static inline digit_t mp_clang_subb (digit_t *r, int c, digit_t x, digit_t y)
{
	digit_t cout;

	*r = MP_CLANG_SUBB (x, y, c, &cout);
	return cout;
}

#define MP_ADDC(r, c, x, y)  mp_clang_addc ((r), (c), (x), (y))
#define MP_SUBB(r, c, x, y)  mp_clang_subb ((r), (c), (x), (y))

#elif MP_DIGIT_BITS == 64 && __has_builtin (__builtin_ia32_addcarry_u64)

#define MP_ADDC(r, c, x, y)  __builtin_ia32_addcarry_u64  ((c), (x), (y), (r))
#define MP_SUBB(r, c, x, y)  __builtin_ia32_subborrow_u64 ((c), (x), (y), (r))

#elif MP_DIGIT_BITS == 32 && __has_builtin (__builtin_ia32_addcarry_u32)

#define MP_ADDC(r, c, x, y)  __builtin_ia32_addcarry_u32  ((c), (x), (y), (r))
#define MP_SUBB(r, c, x, y)  __builtin_ia32_subborrow_u32 ((c), (x), (y), (r))

#endif
#endif  /* no MP_ADDC */

#if !defined (MP_HAVE_GCC_CHECKED) && __has_builtin (__builtin_add_overflow)
#define MP_HAVE_GCC_CHECKED
#endif

#ifndef MP_HAVE_ADDCARRY
#if (MP_DIGIT_BITS == 64 && __has_builtin (_addcarry_u64)) || \
    (MP_DIGIT_BITS == 32 && __has_builtin (_addcarry_u32))
#define MP_HAVE_ADDCARRY
#endif
#endif

#endif  /* MP_COMPILER_CLANG_H */
