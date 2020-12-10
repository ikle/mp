/*
 * Intel C Compiler specific definitions
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef __INTEL_COMPILER
#error "This file must be included for Intel C compiler only"
#endif

#ifndef MP_COMPILER_ICC_H
#define MP_COMPILER_ICC_H  1

#include <mp/types.h>

#if __INTEL_COMPILER >= 1300

#ifndef MP_HAVE_ADDCARRY

#include <immintrin.h>

#define MP_HAVE_ADDCARRY

#endif  /* MP_ADDC */
#endif  /* ICC ≥ 13.0 */

#endif  /* MP_COMPILER_ICC_H */
