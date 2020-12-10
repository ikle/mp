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

#if __has_builtin (__builtin_addcl)
#if MP_DIGIT_ROOF == UINT_MAX
#define MP_CLANG_ADDC	__builtin_addc
#define MP_CLANG_SUBB	__builtin_subc
#elif MP_DIGIT_ROOF == ULONG_MAX
#define MP_CLANG_ADDC	__builtin_addcl
#define MP_CLANG_SUBB	__builtin_subcl
#else
#define MP_CLANG_ADDC	__builtin_addcll
#define MP_CLANG_SUBB	__builtin_subcll
#endif

static inline char mp_clang_adc (digit_t *r, digit_t x, digit_t y, int c)
{
	digit_t cout;

	*r = MP_CLANG_ADDC (x, y, c, &cout);
	return cout;
}

static inline char mp_clang_sbb (digit_t *r, digit_t x, digit_t y, int c)
{
	digit_t cout;

	*r = MP_CLANG_SUBB (x, y, c, &cout);
	return cout;
}
#endif  /* __builtin_addc */

#ifndef mp_digit_adc
#if __has_builtin (__builtin_addcl)

#define mp_digit_adc	mp_clang_adc
#define mp_digit_sbb	mp_clang_sbb

#elif MP_DIGIT_BITS == 64 && __has_builtin (__builtin_ia32_addcarry_u64)

#define mp_digit_adc(r, x, y, c)  __builtin_ia32_addcarry_u64  ((c), (x), (y), (r))
#define mp_digit_sbb(r, x, y, c)  __builtin_ia32_subborrow_u64 ((c), (x), (y), (r))

#elif MP_DIGIT_BITS == 32 && __has_builtin (__builtin_ia32_addcarry_u32)

#define mp_digit_adc(r, x, y, c)  __builtin_ia32_addcarry_u32  ((c), (x), (y), (r))
#define mp_digit_sbb(r, x, y, c)  __builtin_ia32_subborrow_u32 ((c), (x), (y), (r))

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
