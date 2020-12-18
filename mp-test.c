/*
 * MP Core Tests
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static inline size_t mp_normalize (digit_t *x, size_t len)
{
	while (len > 0 && x[len - 1] == 0)
		--len;

	return len;
}

static void mp_random (digit_t *o, size_t len)
{
	unsigned char *p;
	size_t i;

	for (p = (void *) o, i = 0; i < len * sizeof (*o); ++i)
		p[i] = rand ();
}

static void mp_show (const char *prefix, const digit_t *o, size_t len)
{
	if (len == 0) {
		printf ("%s 0\n", prefix);
		return;
	}

	printf ("%s", prefix);

	for (; len > 0; --len)
		printf (" %016lX", o[len - 1]);

	printf ("\n");
}

/*
 * Basic addition and substraction test
 */

struct test_add {
	digit_t *a, *b, *n;
	size_t len;
};

static int test_add_init (struct test_add *o, size_t len)
{
	if ((o->a = mp_alloc (len)) == NULL)		goto no_a;
	if ((o->b = mp_alloc (len)) == NULL)		goto no_b;
	if ((o->n = mp_alloc (len + 1)) == NULL)	goto no_n;

	o->len = len;
	return 1;

no_n:	mp_free (o->b);
no_b:	mp_free (o->a);
no_a:	perror ("test add");
	return 0;
}

static void test_add_fini (struct test_add *o)
{
	mp_free (o->n);
	mp_free (o->b);
	mp_free (o->a);
}

static void test_add_mix (struct test_add *o)
{
	mp_random (o->a, o->len);
	mp_random (o->b, o->len);
}

static int test_add (struct test_add *o)
{
	digit_t *a = o->a, *b = o->b, *n = o->n;
	size_t len = o->len;
	int ok;

	/*
	 * Test for a + b - b = a
	 */
	n[len]  = mp_add_n (n, a, b, len, 0);
	n[len] -= mp_sub_n (n, n, b, len, 0);

	if (!(ok = mp_cmp_n (a, n, len) == 0)) {
		printf ("add (%zu) failed:\n", len);
		mp_show ("\ta =", a, len);
		mp_show ("\tb =", b, len);
		mp_show ("\tn =", n, len);
	}

	return ok;
}

static int test_add_fuzzy (size_t len, size_t count)
{
	struct test_add o;
	int ok;

	if (!test_add_init (&o, len))
		return 0;

	for (ok = 1; count > 0; --count) {
		test_add_mix (&o);
		ok &= test_add (&o);
	}

	test_add_fini (&o);
	return ok;
}

/*
 * Basic multiplication with addition test
 */

struct test_mul {
	digit_t *a, *b, *c, *ns, *n, *m0, *m1, *m;
	size_t len;
};

static int test_mul_init (struct test_mul *o, size_t len)
{
	if ((o->a  = mp_alloc (len)) == NULL)		goto no_a;
	if ((o->b  = mp_alloc (len)) == NULL)		goto no_b;
	if ((o->c  = mp_alloc (len)) == NULL)		goto no_c;

	if ((o->ns = mp_alloc (len + 1))     == NULL)	goto no_ns;
	if ((o->n  = mp_alloc (2 * len + 1)) == NULL)	goto no_n;
	if ((o->m0 = mp_alloc (2 * len))     == NULL)	goto no_m0;
	if ((o->m1 = mp_alloc (2 * len))     == NULL)	goto no_m1;
	if ((o->m  = mp_alloc (2 * len + 1)) == NULL)	goto no_m;

	o->len = len;
	return 1;

no_m:	mp_free (o->m1);
no_m1:	mp_free (o->m0);
no_m0:	mp_free (o->n);
no_n:	mp_free (o->ns);
no_ns:	mp_free (o->c);
no_c:	mp_free (o->b);
no_b:	mp_free (o->a);
no_a:	perror ("test mul");
	return 0;
}

static void test_mul_fini (struct test_mul *o)
{
	mp_free (o->m);
	mp_free (o->m1);
	mp_free (o->m0);
	mp_free (o->n);
	mp_free (o->ns);
	mp_free (o->c);
	mp_free (o->b);
	mp_free (o->a);
}

static void test_mul_mix (struct test_mul *o)
{
	mp_random (o->a, o->len);
	mp_random (o->b, o->len);
	mp_random (o->c, o->len);
}

static int test_mul (struct test_mul *o)
{
	digit_t *a = o->a, *b = o->b, *c = o->c;
	digit_t *ns = o->ns, *n = o->n, *m0 = o->m0, *m1 = o->m1, *m = o->m;
	size_t len = o->len;
	int ok;

	/*
	 * Test for a(b + c) = ab + ac
	 */
	ns[len] = mp_add_n (ns, b, c, len, 0);
	mp_mul (n, ns, len + 1, a, len);

	mp_mul (m0, a, len, b, len);
	mp_mul (m1, a, len, c, len);
	m[2 * len] = mp_add_n (m, m0, m1, 2 * len, 0);

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

	return ok;
}

static int test_mul_fuzzy (size_t len, size_t count)
{
	struct test_mul o;
	int ok;

	if (!test_mul_init (&o, len))
		return 0;

	for (ok = 1; count > 0; --count) {
		test_mul_mix (&o);
		ok &= test_mul (&o);
	}

	test_mul_fini (&o);
	return ok;
}

/*
 * Basic division with multiplication amd addition test
 */

struct test_div {
	digit_t *a, *b, *c, *m, *s, *q, *r;
	size_t len;
};

static int test_div_init (struct test_div *o, size_t len)
{
	size_t alen = len + 2, mlen, slen, qlen, rlen;

	if ((o->a  = mp_alloc (alen)) == NULL)	goto no_a;
	if ((o->b  = mp_alloc (len))  == NULL)	goto no_b;
	if ((o->c  = mp_alloc (len))  == NULL)	goto no_c;

	mlen = alen + len;
	slen = mlen + 1;
	qlen = slen - len + 1;
	rlen = slen;

	if ((o->m = mp_alloc (mlen)) == NULL)	goto no_m;
	if ((o->s = mp_alloc (slen)) == NULL)	goto no_s;
	if ((o->q = mp_alloc (qlen)) == NULL)	goto no_q;
	if ((o->r = mp_alloc (rlen)) == NULL)	goto no_r;

	o->len = len;
	return 1;

no_r:	mp_free (o->q);
no_q:	mp_free (o->s);
no_s:	mp_free (o->m);
no_m:	mp_free (o->c);
no_c:	mp_free (o->b);
no_b:	mp_free (o->a);
no_a:	perror ("test div");
	return 0;
}

static void test_div_fini (struct test_div *o)
{
	mp_free (o->r);
	mp_free (o->q);
	mp_free (o->s);
	mp_free (o->m);
	mp_free (o->c);
	mp_free (o->b);
	mp_free (o->a);
}

static void test_div_mix (struct test_div *o)
{
	digit_t *a = o->a, *b = o->b, *c = o->c;
	size_t len = o->len, alen = o->len + 2;

	mp_random (a, alen);
	mp_random (b, len);
	mp_random (c, len);

	/* b must be normalized */
	b[len - 1] |= (digit_t) 1 << (MP_DIGIT_BITS - 1);

	/* c must be less than b */
	if (mp_cmp_n (c, b, len) >= 0)
		mp_sub_n (c, c, b, len, 0);
}

static int test_div (struct test_div *o)
{
	digit_t *a = o->a, *b = o->b, *c = o->c;
	digit_t *m = o->m, *s = o->s, *q = o->q, *r = o->r;
	size_t len = o->len, alen = len + 2, clen, mlen, slen, qlen, rlen;
	int ok;

	mlen = alen + len;
	slen = mlen + 1;
	qlen = slen - len + 1;
	rlen = slen;

	/*
	 * Test for (ab + c) / b = (a, c), where c < b
	 */
	mp_mul (m, a, alen, b, len);
	s[slen - 1] = mp_add (s, m, mlen, c, len, 0);

	rlen = mp_div (q, r, s, slen, b, len);
	qlen = mp_normalize (q, qlen);
	rlen = mp_normalize (r, rlen);

	alen = mp_normalize (a, alen);
	clen = mp_normalize (c, len);

	ok = qlen == alen && mp_cmp_n (q, a, qlen) == 0 &&
	     rlen == clen && mp_cmp_n (r, c, len)  == 0;

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

	return ok;
}

static int test_div_fuzzy (size_t len, size_t count)
{
	struct test_div o;
	int ok;

	if (!test_div_init (&o, len))
		return 0;

	for (ok = 1; count > 0; --count) {
		test_div_mix (&o);
		ok &= test_div (&o);
	}

	test_div_fini (&o);
	return ok;
}

/*
 * Top-level code
 */

#define MAX_LEN		32

#define ADD_COUNT	10000
#define MUL_COUNT	10000
#define DIV_COUNT	10000

int main (int argc, char *argv[])
{
	size_t len;
	time_t start = time (NULL);
	int ok = 1;

	srand ((unsigned) start);

	for (len = 0; len <= MAX_LEN; ++len)
		ok &= test_add_fuzzy (len, ADD_COUNT);

	for (len = 0; len <= MAX_LEN; ++len)
		ok &= test_mul_fuzzy (len, MUL_COUNT);

	for (len = 1; len <= MAX_LEN; ++len)
		ok &= test_div_fuzzy (len, DIV_COUNT);

	return ok ? 0 : 1;
}
