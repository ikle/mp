/*
 * MP Platform-specific definitions
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_PLATFORM_H
#define MP_PLATFORM_H  1

#include "mp-types.h"

/*
 * GCC-specific checks
 */

#if defined(__GNUC__) && __GNUC__ >= 5

#define GCC_CHECKED

#endif  /* GCC ≥ 5.0 */

/*
 * Clang-specific checks
 */

#ifdef __clang__

#ifndef MP_ADDC

#if __has_builtin(__builtin_addcl)

#if (sizeof (int) * CHAR_BIT) == MP_WORD_BITS
#define MP_CLANG_ADDC(x, y, ci, co)	__builtin_addc ((x), (y), (ci), (co))
#define MP_CLANG_SUBB(x, y, ci, co)	__builtin_subc ((x), (y), (ci), (co))
#elif (sizeof (long) * CHAR_BIT) == MP_WORD_BITS
#define MP_CLANG_ADDC(x, y, ci, co)	__builtin_addcl ((x), (y), (ci), (co))
#define MP_CLANG_SUBB(x, y, ci, co)	__builtin_subcl ((x), (y), (ci), (co))
#else
#define MP_CLANG_ADDC(x, y, ci, co)	__builtin_addcll ((x), (y), (ci), (co))
#define MP_CLANG_SUBB(x, y, ci, co)	__builtin_subcll ((x), (y), (ci), (co))
#endif

static inline
mp_word mp_clang_addc (mp_word *r, int c, mp_word x, mp_word y)
{
	mp_word cout;

	*r = MP_CLANG_ADDC (x, y, c, &cout);
	return cout;
}

static inline
mp_word mp_clang_subb (mp_word *r, int c, mp_word x, mp_word y)
{
	mp_word cout;

	*r = MP_CLANG_SUBB (x, y, c, &cout);
	return cout;
}

#define MP_ADDC(r, c, x, y)  mp_clang_addc ((r), (c), (x), (y))
#define MP_SUBB(r, c, x, y)  mp_clang_subb ((r), (c), (x), (y))

#elif MP_WORD_BITS == 64 && __has_builtin(__builtin_ia32_addcarry_u64)

#define MP_ADDC(r, c, x, y)  __builtin_ia32_addcarry_u64  ((c), (x), (y), (r))
#define MP_SUBB(r, c, x, y)  __builtin_ia32_subborrow_u64 ((c), (x), (y), (r))

#elif MP_WORD_BITS == 32 && __has_builtin(__builtin_ia32_addcarry_u32)

#define MP_ADDC(r, c, x, y)  __builtin_ia32_addcarry_u32  ((c), (x), (y), (r))
#define MP_SUBB(r, c, x, y)  __builtin_ia32_subborrow_u32 ((c), (x), (y), (r))

#endif
#endif  /* MP_ADDC */

#if !defined (GCC_CHECKED) && __has_builtin(__builtin_add_overflow)

#define GCC_CHECKED

#endif
#endif  /* __clang__ */

/*
 * MSC-specific checks
 */

#if defined(_MSC_VER) && _MSC_VER >= 1800	/* VS 2013 or greater */

#if defined(_M_AMD64) || defined(_M_IA64)

#include <intrin.h>

static inline
mp_word mp_msc_umul (mp_word *r, mp_word x, mp_word y)
{
	mp_word cout;

	*r = _umul128 (x, y, &cout);
	return cout;
}

#define MP_UMUL(r, x, y)  mp_msc_umul ((r), (x), (y))

#endif  /* AMD64 or IA-64 */

#ifndef MP_ADDC

#include <intrin.h>

#define HAS_ADDCARRY

#endif  /* MP_ADDC */
#endif  /* MSC ≥ 18.0 (VS ≥ 2013) */

/*
 * ICC-spcific checks
 */

#if defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1300

#ifndef MP_ADDC

#include <immintrin.h>

#define HAS_ADDCARRY

#endif  /* MP_ADDC */
#endif  /* ICC ≥ 13.0 */

/*
 * GCC-specific checks
 */

#if defined(__GNUC__)
#if defined(__x86_64__) || defined (__i386__)

#include <x86intrin.h>

#ifdef _ADXINTRIN_H_INCLUDED

#define HAS_ADDCARRY

#endif  /* ADX */
#endif
#endif  /* GCC */

/*
 * GCC checked
 */

#if !defined(MP_ADD) && defined(GCC_CHECKED)

#define MP_ADD(r, x, y)  __builtin_add_overflow ((x), (y), (r))
#define MP_SUB(r, x, y)  __builtin_sub_overflow ((x), (y), (r))

#endif

/*
 * addcarry
 */

#if !defined(MP_ADDC) && defined(HAS_ADDCARRY)

#if MP_WORD_BITS == 64

#define MP_ADDC(r, c, x, y)  _addcarry_u64  ((c), (x), (y), (r))
#define MP_SUBB(r, c, x, y)  _subborrow_u64 ((c), (x), (y), (r))

#elif MP_WORD_BITS == 32

#define MP_ADDC(r, c, x, y)  _addcarry_u32  ((c), (x), (y), (r))
#define MP_SUBB(r, c, x, y)  _subborrow_u32 ((c), (x), (y), (r))

#endif
#endif  /* adcarry */

/*
 * Core functions
 */

static inline
char mp_word_add (mp_word *r, mp_word x, mp_word y)
{
#if defined(MP_ADD)   
	return MP_ADD (r, x, y);
#elif defined(MP_ADDC)
	return MP_ADDC (r, 0, x, y);
#else  
	*r = x + y;
	return *r < x;
#endif
}

static inline
char mp_word_sub (mp_word *r, mp_word x, mp_word y)
{
#if defined(MP_SUB)   
	return MP_SUB (r, x, y);
#elif defined(MP_SUBB)
	return MP_SUBB (r, 0, x, y);
#else  
	*r = x - y;
	return *r > x;
#endif
}

static inline
char mp_word_add_carry (mp_word *r, int c, mp_word x, mp_word y)
{
#if defined(MP_ADDC)
	return MP_ADDC (r, c, x, y);
#else  
	mp_word a;

	c = mp_word_add (&a, y, c);
	return c + mp_word_add (r,  x, a);
#endif
}

static inline
char mp_word_sub_borrow (mp_word *r, int c, mp_word x, mp_word y)
{
#if defined(MP_SUBB)
	return MP_SUBB (r, c, x, y);
#else
	mp_word a;

	c = mp_word_add (&a, y, c);
	return c + mp_word_sub (r,  x, a);
#endif
}

static inline
mp_word mp_word_mul (mp_word *r, mp_word x, mp_word y)
{
#ifdef MP_UMUL
	return MP_UMUL (r, x, y);
#else
	mp_dword a = (mp_dword) x * y;
	*r = a;
	return a >> MP_WORD_BITS;
#endif
}

static inline
mp_word mp_word_mul_add (mp_word *r, mp_word x, mp_word y, mp_word a)
{
	mp_word l, h = mp_word_mul (&l, x, y);

	return h + mp_word_add (r, l, a);
}

#endif  /* MP_PLATFORM_H */
