/*
 * MP Core Tests
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mp/core.h>

static inline digit_t *mp_alloc (size_t len)
{
	return malloc (sizeof (digit_t) * len);
}

static inline void mp_free (digit_t *o)
{
	free (o);
}

static void mp_random (digit_t *o, size_t len)
{
	unsigned char *p;
	size_t i;

	for (p = (void *) o, i = 0; i < len * sizeof (*o); ++i)
		p[i] = rand ();
}

static digit_t *mp_alloc_random (size_t len)
{
	digit_t *o;

	if ((o = mp_alloc (len)) == NULL)
		return NULL;

	mp_random (o, len);
	return o;
}

static void mp_show (const char *prefix, const digit_t *o, size_t len)
{
	if (len == 0) {
		printf ("%s 0\n", prefix);
		return;
	}

	printf ("%s", prefix);

	for (; len > 0; --len)
		printf (" %016lX ", o[len - 1]);

	printf ("\n");
}

static int test_add (size_t len)
{
	digit_t *n, *a, *b;
	int ok;

	if ((n = mp_alloc (len + 1)) == NULL)		goto no_n;
	if ((a = mp_alloc_random (len)) == NULL)	goto no_a;
	if ((b = mp_alloc_random (len)) == NULL)	goto no_b;

	/*
	 * Test for a + b - b = a
	 */
	n[len]  = mp_add_n (n, a, b, len);
	n[len] -= mp_sub_n (n, n, b, len);

	if (!(ok = mp_cmp_n (a, n, len) == 0)) {
		printf ("add (%zu) failed:\n", len);
		mp_show ("\ta =", a, len);
		mp_show ("\tb =", b, len);
		mp_show ("\tn =", n, len);
	}

	mp_free (b);
	mp_free (a);
	mp_free (n);
	return ok;

no_b:	mp_free (a);
no_a:	mp_free (n);
no_n:	perror ("mp_alloc");
	return 0;
}

static int test_mul (size_t len)
{
	digit_t *a, *b, *c, *ns, *n, *m0, *m1, *m;
	int ok;

	if ((a  = mp_alloc_random (len)) == NULL)	goto no_a;
	if ((b  = mp_alloc_random (len)) == NULL)	goto no_b;
	if ((c  = mp_alloc_random (len)) == NULL)	goto no_c;

	if ((ns = mp_alloc (len + 1))     == NULL)	goto no_ns;
	if ((n  = mp_alloc (2 * len + 1)) == NULL)	goto no_n;
	if ((m0 = mp_alloc (2 * len))     == NULL)	goto no_m0;
	if ((m1 = mp_alloc (2 * len))     == NULL)	goto no_m1;
	if ((m  = mp_alloc (2 * len + 1)) == NULL)	goto no_m;

	/*
	 * Test for a(b + c) = ab + ac
	 */
	ns[len] = mp_add_n (ns, b, c, len);
	mp_mul (n, ns, len + 1, a, len);

	mp_mul (m0, a, len, b, len);
	mp_mul (m1, a, len, c, len);
	m[2 * len] = mp_add_n (m, m0, m1, 2 * len);

	if (!(ok = mp_cmp_n (n, m, 2 * len + 1) == 0)) {
		printf ("mul (%zu) failed:\n", len);

		mp_show ("\ta     =", a, len);
		mp_show ("\tb     =", b, len);
		mp_show ("\tc     =", c, len);

		mp_show ("\tb + c =", ns, len + 1);
		mp_show ("\tab    =", m0, 2 * len);
		mp_show ("\tac    =", m1, 2 * len);

		mp_show ("\tn     =", n, 2 * len + 1);
		mp_show ("\tm     =", m, 2 * len + 1);
	}

	mp_free (m);
	mp_free (m1);
	mp_free (m0);
	mp_free (n);
	mp_free (ns);
	mp_free (c);
	mp_free (b);
	mp_free (a);
	return ok;

no_m:	mp_free (m1);
no_m1:	mp_free (m0);
no_m0:	mp_free (n);
no_n:	mp_free (ns);
no_ns:	mp_free (c);
no_c:	mp_free (b);
no_b:	mp_free (a);
no_a:	perror ("mp_alloc");
	return 0;
}

static inline size_t mp_normalize (digit_t *x, size_t len)
{
	while (len > 0 && x[len - 1] == 0)
		--len;

	return len;
}

static int test_div (size_t len)
{
	digit_t *a, *b, *c, *m, *s, *q, *r;
	size_t alen = len + 2, mlen, slen, qlen, rlen;
	int ok;

	if ((a  = mp_alloc_random (alen)) == NULL)	goto no_a;
	if ((b  = mp_alloc_random (len))  == NULL)	goto no_b;
	if ((c  = mp_alloc_random (len))  == NULL)	goto no_c;

	/* b must be normalized */
	b[len - 1] |= (digit_t) 1 << (MP_DIGIT_BITS - 1);

	/* c must be less than b */
	if (mp_cmp_n (c, b, len) >= 0)
		mp_sub_n (c, c, b, len);

	mlen = alen + len;
	slen = mlen + 1;
	qlen = slen - len + 1;
	rlen = slen;

	if ((m = mp_alloc (mlen)) == NULL)	goto no_m;
	if ((s = mp_alloc (slen)) == NULL)	goto no_s;
	if ((q = mp_alloc (qlen)) == NULL)	goto no_q;
	if ((r = mp_alloc (rlen)) == NULL)	goto no_r;

	/*
	 * Test for (ab + c) / b = (a, c), where c < b
	 */
	mp_mul (m, a, alen, b, len);
	s[slen - 1] = mp_add (s, m, mlen, c, len);

	rlen = mp_div (q, r, s, slen, b, len);
	qlen = mp_normalize (q, qlen);

	ok = qlen == alen && mp_cmp_n (q, a, qlen) == 0 &&
	     rlen == len  && mp_cmp_n (r, c, len)  == 0;

	if (!ok) {
		printf ("div (%zu) failed:\n", len);

		mp_show ("\ta      =", a, alen);
		mp_show ("\tb      =", b, len);
		mp_show ("\tc      =", c, len);

		mp_show ("\tab     =", m, mlen);
		mp_show ("\tab + c =", s, slen);

		mp_show ("\tq      =", q, qlen);
		mp_show ("\tr      =", r, rlen);
	}

	mp_free (r);
	mp_free (q);
	mp_free (s);
	mp_free (m);
	mp_free (c);
	mp_free (b);
	mp_free (a);
	return ok;

no_r:	mp_free (q);
no_q:	mp_free (s);
no_s:	mp_free (m);
no_m:	mp_free (c);
no_c:	mp_free (b);
no_b:	mp_free (a);
no_a:	perror ("mp_alloc");
	return 0;
}

#define MAX_LEN		16

#define ADD_COUNT	10000
#define MUL_COUNT	10000
#define DIV_COUNT	10000

int main (int argc, char *argv[])
{
	size_t len, i;
	time_t start = time (NULL);
	int ok = 0;

	srand ((unsigned) start);

	for (len = 0; len < MAX_LEN; ++len)
		for (i = 0; i < ADD_COUNT; ++i)
			ok |= test_add (len);

	for (len = 0; len < MAX_LEN; ++len)
		for (i = 0; i < MUL_COUNT; ++i)
			ok |= test_mul (len);

	for (len = 1; len < MAX_LEN; ++len)
		for (i = 0; i < DIV_COUNT; ++i)
			ok |= test_div (len);

	return ok ? 0 : 1;
}
