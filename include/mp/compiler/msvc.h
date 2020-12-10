/*
 * Microsoft Visual C Compiler specific definitions
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _MSC_VER
#error "This file must be included for Microsoft Visual C compiler only"
#endif

#ifndef MP_COMPILER_MSVC_H
#define MP_COMPILER_MSVC_H  1

#include <mp/types.h>

#if _MSC_VER >= 1800	/* VS 2013 or greater */

#if defined (_M_AMD64) || defined (_M_IA64)

#include <intrin.h>

#ifndef mp_digit_mul
static inline void mp_msvc_mul (digit_t *r1, digit_t *r0, digit_t x, digit_t y)
{
	*r0 = _umul128 (x, y, r1);
}

#define mp_digit_mul	mp_msvc_mul
#endif

#ifndef mp_digit_div
static inline
void mp_msvc_div (digit_t *q, digit_t *r, digit_t n1, digit_t n0, digit_t d)
{
	*q = _udiv128 (x1, x0, y, r);
}

#define mp_digit_div	mp_msvc_div
#endif

#endif  /* AMD64 or IA-64 */

#ifndef MP_HAVE_ADDCARRY

#include <intrin.h>

#define MP_HAVE_ADDCARRY

#endif  /* addcarry */
#endif  /* MSC ≥ 18.0 (VS ≥ 2013) */

#endif  /* MP_COMPILER_MSVC_H */
