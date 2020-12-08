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

#ifndef MP_UMUL
static inline digit_t mp_msvc_umul (digit_t *r, digit_t x, digit_t y)
{
	digit_t cout;

	*r = _umul128 (x, y, &cout);
	return cout;
}

#define MP_UMUL(r, x, y)  mp_msc_umul ((r), (x), (y))
#endif

#ifndef MP_UDIV
static inline
digit_t mp_msvc_udiv (digit_t *r, digit_t x1, digit_t x0, digit_t y)
{
	digit_t quo, rem;

	*r = _udiv128 (x1, x0, y, &rem);
	return rem;
}

#define MP_UDIV(r, x1, xo, y)  mp_msc_udiv ((r), (x1), (x0), (y))
#endif

#endif  /* AMD64 or IA-64 */

#ifndef MP_ADDC

#include <intrin.h>

#define MP_HAVE_ADDCARRY

#endif  /* MP_ADDC */
#endif  /* MSC ≥ 18.0 (VS ≥ 2013) */

#endif  /* MP_COMPILER_MSVC_H */
