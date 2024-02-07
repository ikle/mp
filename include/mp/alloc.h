/*
 * MP Core Allocation
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MP_ALLOC_H
#define MP_ALLOC_H  1

#include <stdlib.h>

#include <mp/types.h>

static inline digit_t *mp_alloc (size_t len)
{
	return malloc (sizeof (digit_t) * len);
}

static inline void mp_free (digit_t *o)
{
	free (o);
}

#endif  /* MP_ALLOC_H */
