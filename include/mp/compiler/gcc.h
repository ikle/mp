/*
 * GNU Compiler specific definitions
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef __GNUC__
#error "This file must be included for GNU or compatible compiler only"
#endif

#ifndef MP_COMPILER_GCC_H
#define MP_COMPILER_GCC_H  1

#include <mp/types.h>

#if !defined (MP_HAVE_GCC_CHECKED) && __GNUC__ >= 5
#define MP_HAVE_GCC_CHECKED
#endif  /* gcc >= 5 */

#if defined (__x86_64__) || defined (__i386__)

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)
#include <x86intrin.h>

#if !defined (MP_HAVE_ADDCARRY) && defined (_ADXINTRIN_H_INCLUDED)

/*
 * The function _subborrow_u{32,64} on gcc < 7.2 have wrong argument order,
 * use intrinsics directly.
 */
#if MP_DIGIT_BITS == 64

#define _addcarry_u64   __builtin_ia32_addcarryx_u64
#define _subborrow_u64  __builtin_ia32_sbb_u64
#define MP_HAVE_ADDCARRY

#elif MP_DIGIT_BITS == 32

#define _addcarry_u32   __builtin_ia32_addcarry_u32
#define _subborrow_u32  __builtin_ia32_sbb_u32
#define MP_HAVE_ADDCARRY

#endif  /* B = 64 or B = 32 */
#endif  /* ADX supported */
#endif  /* gcc >= 4.4 */

#endif  /* arch-dependent */

/*
 * GCC checked builtins
 */
#if !defined (mp_digit_add) && defined (MP_HAVE_GCC_CHECKED)

#define mp_digit_add(r, x, y)	__builtin_add_overflow ((x), (y), (r))
#define mp_digit_sub(r, x, y)	__builtin_sub_overflow ((x), (y), (r))

#endif  /* no add primitive and have checked */

/*
 * Family of addcarry builtins
 */
#if !defined (mp_digit_adc) && defined (MP_HAVE_ADDCARRY)

#if MP_DIGIT_BITS == 64

#define mp_digit_adc(r, x, y, c)  _addcarry_u64  ((c), (x), (y), (void *) (r))
#define mp_digit_sbb(r, x, y, c)  _subborrow_u64 ((c), (x), (y), (void *) (r))

#elif MP_DIGIT_BITS == 32

#define mp_digit_adc(r, x, y, c)  _addcarry_u32  ((c), (x), (y), (r))
#define mp_digit_sbb(r, x, y, c)  _subborrow_u32 ((c), (x), (y), (r))

#endif
#endif  /* no adc primitive and have addcarry */

#ifndef mp_digit_clz  /* && gcc > ? */
#if MP_DIGIT_ROOF == UINT_MAX
#define mp_digit_clz	__builtin_clz
#elif MP_DIGIT_ROOF == ULONG_MAX
#define mp_digit_clz	__builtin_clzl
#else
#define mp_digit_clz	__builtin_clzll
#endif
#endif  /* clz */

#ifndef mp_digit_ctz  /* && gcc > ? */
#if MP_DIGIT_ROOF == UINT_MAX
#define mp_digit_ctz	__builtin_ctz
#elif MP_DIGIT_ROOF == ULONG_MAX
#define mp_digit_ctz	__builtin_ctzl
#else
#define mp_digit_ctz	__builtin_ctzll
#endif
#endif  /* ctz */

#endif  /* MP_COMPILER_GCC_H */
