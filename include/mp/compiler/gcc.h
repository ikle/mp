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

#if __GNUC__ >= 4 && __GNUC_MINOR__ > 4
#include <x86intrin.h>

#ifdef _ADXINTRIN_H_INCLUDED
#define MP_HAVE_ADDCARRY
#endif  /* ADX supported */
#endif  /* gcc >= 4.4 */

#endif  /* arch-dependent */

/*
 * GCC checked builtins
 */
#if !defined (MP_ADD) && defined (MP_HAVE_GCC_CHECKED)

#define MP_ADD(r, x, y)  __builtin_add_overflow ((x), (y), (r))
#define MP_SUB(r, x, y)  __builtin_sub_overflow ((x), (y), (r))

#endif  /* no MP_ADD and have checked */

/*
 * Family of addcarry builtins
 */
#if !defined (MP_ADDC) && defined (MP_HAVE_ADDCARRY)

#if MP_DIGIT_BITS == 64

#define MP_ADDC(r, c, x, y)  _addcarry_u64  ((c), (x), (y), (void *) (r))
#define MP_SUBB(r, c, x, y)  _subborrow_u64 ((c), (x), (y), (void *) (r))

#elif MP_DIGIT_BITS == 32

#define MP_ADDC(r, c, x, y)  _addcarry_u32  ((c), (x), (y), (r))
#define MP_SUBB(r, c, x, y)  _subborrow_u32 ((c), (x), (y), (r))

#endif
#endif  /* no MP_ADDC and have addcarry */

#endif  /* MP_COMPILER_GCC_H */
