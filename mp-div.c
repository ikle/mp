/*
 * MP Core Division
 *
 * Copyright (c) 2018-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mp/add.h>
#include <mp/div.h>
#include <mp/pair.h>
#include <mp/mul.h>
#include <mp/unit.h>

static inline
digit_t mp_div_reduce (digit_t *q, digit_t c, digit_t *r,
		       const digit_t *d, size_t dlen, digit_t inv)
{
	q[0] = mp_pair_divapprox (c, r[dlen - 1], inv);

	c -= mp_submul_1 (r, d, dlen, q[0], 0);

	if (c > 0 || mp_cmp_n (r, d, dlen) >= 0) {
		++q[0];
		mp_sub_n (r, r, d, dlen, 0);
	}

	return r[dlen - 1];
}

/*
 * Function mp_div divides (n, nlen) by (d, dlen), stores quotinent into
 * (q, nlen - dlen + 1), remainder into (r, nlen), and returns the size
 * of remainder. Constraints: n and d are normalized, the most significant
 * bit of d is set, and nlen >= dlen > 0. Note that the remainder is not
 * normalized.
 *
 * Tip: Normalize n and d, and then shift n and d left by clz(d) before
 * calling the mp_div function.
 */
size_t mp_div (digit_t *q, digit_t *r, const digit_t *n, size_t nlen,
				       const digit_t *d, size_t dlen)
{
	digit_t c = 0, inv;
	size_t i, j;

	if (r != n)
		mp_copy (r, n, nlen);

	inv = mp_pair_invert (d[dlen - 1], dlen > 1 ? d[dlen - 2]: 0);

	for (
		i = nlen - 1, j = nlen - dlen;
		j > 0;
		--i, --j
	) {
		c = mp_div_reduce (q + j, c, r + j, d, dlen, inv);
		r[i] = 0;
	}

	r[i] = mp_div_reduce (q + 0, c, r + 0, d, dlen, inv);

	return i + 1;
}
