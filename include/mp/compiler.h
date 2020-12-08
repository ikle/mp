/*
 * MP Compiler specific definitions
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_COMPILER_H
#define MP_COMPILER_H  1

#ifdef __clang__
#include <mp/compiler/clang.h>
#endif

#ifdef _MSC_VER
#include <mp/compiler/msvc.h>
#endif

#ifdef __INTEL_COMPILER
#include <mp/compiler/icc.h>
#endif

#ifdef __GNUC__
#include <mp/compiler/gcc.h>
#endif

#endif  /* MP_COMPILER_H */
