/*
 * MP Speed Tests
 *
 * Copyright (c) 2018-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <math.h>
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

static inline void mp_show (const char *prefix, const digit_t *o, size_t len)
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
 * Gauge addition operation
 */

static clock_t gauge_add (size_t len, size_t count)
{
	digit_t *a, *b, *s;
	clock_t t;

	if ((a = mp_alloc (len))     == NULL)	goto no_a;
	if ((b = mp_alloc (len))     == NULL)	goto no_b;
	if ((s = mp_alloc (len + 1)) == NULL)	goto no_s;

	mp_random (a, len);
	mp_random (b, len);

	for (t = clock (); count > 0; --count)
		s[len] = mp_add_n (s, a, b, len, 0);

	t = clock () - t;

	mp_free (s);
	mp_free (b);
	mp_free (a);
	return t;

no_s:	mp_free (b);
no_b:	mp_free (a);
no_a:	perror ("gauge add");
	return (clock_t) -1;
}

/*
 * Gauge multiplication operation
 */

static clock_t gauge_mul (size_t len, size_t count)
{
	digit_t *a, *b, *m;
	clock_t t;

	if ((a = mp_alloc (len))     == NULL)	goto no_a;
	if ((b = mp_alloc (len))     == NULL)	goto no_b;
	if ((m = mp_alloc (len * 2)) == NULL)	goto no_m;

	mp_random (a, len);
	mp_random (b, len);

	for (t = clock (); count > 0; --count)
		mp_mul (m, a, len, b, len);

	t = clock () - t;

	mp_free (m);
	mp_free (b);
	mp_free (a);
	return t;

no_m:	mp_free (b);
no_b:	mp_free (a);
no_a:	perror ("gauge mul");
	return (clock_t) -1;
}

/*
 * Gauge division operation
 */

static clock_t gauge_div (size_t len, size_t count)
{
	digit_t *n, *d, *q, *r;
	size_t nlen = 2 * len, dlen = len, qlen = len + 1, rlen = nlen;
	clock_t t;

	if ((n  = mp_alloc (nlen)) == NULL)	goto no_n;
	if ((d  = mp_alloc (dlen)) == NULL)	goto no_d;
	if ((q  = mp_alloc (qlen)) == NULL)	goto no_q;
	if ((r  = mp_alloc (rlen)) == NULL)	goto no_r;

	mp_random (n, nlen);
	mp_random (d, dlen);

	/* b must be normalized */
	d[len - 1] |= (digit_t) 1 << (MP_DIGIT_BITS - 1);

	for (t = clock (); count > 0; --count)
		mp_div (q, r, n, nlen, d, dlen);

	t = clock () - t;

	mp_free (r);
	mp_free (q);
	mp_free (d);
	mp_free (n);
	return t;

no_r:	mp_free (q);
no_q:	mp_free (d);
no_d:	mp_free (n);
no_n:	return (clock_t) -1;
}

/*
 * Speed test helpers
 */

static int clock_cmp (const void *A, const void *B)
{
	const clock_t *a = A, *b = B;

	return *a - *b;
}

void average (clock_t *data, size_t n, size_t count, double *value, double *s)
{
	size_t i, lo = n / 10, hi = (n * 9 + 9) / 10;
	double v, ss, d;
	const double r = (1e9 / CLOCKS_PER_SEC) / count;

	qsort (data, n, sizeof (data[0]), clock_cmp);

	for (v = 0, i = lo; i < hi; ++i)
		v += data[i] * r;

	v /= (hi - lo);

	for (ss = 0, i = lo; i < hi; ++i) {
		d = data[i] *r - v;
		ss += d * d;
	}

	ss /= (hi - lo);

	*value = v;
	*s = sqrt (ss);
}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)  (sizeof (a) / sizeof ((a)[0]))
#endif

typedef clock_t gauge_fn (size_t len, size_t count);

void gauge (size_t len, size_t count, gauge_fn *f,
	    const char *root, const char *name)
{
	FILE *to = NULL;
	char path[256];
	clock_t data[400];
	size_t i;
	double v, s;

	if (root != NULL) {
		snprintf (path, sizeof (path), "%s/%s.data", root, name);
		to = fopen (path, "a");
	}

	if (to == NULL)
		to = stdout;

	count = count > len ? count / len : 1;

	for (i = 0; i < ARRAY_SIZE (data); ++i)
		data[i] = f (len, count);

	average (data, ARRAY_SIZE (data), count, &v, &s);

	fprintf (to, "%zu\t%.2f\t%.2f\n", len, v, s);

	if (to != stdout)
		fclose (to);
}

/*
 * Top-level code
 */

#define MAX_LEN		100

int main (int argc, char *argv[])
{
	const char *root = NULL;
	time_t start = time (NULL);
	size_t len;

	srand ((unsigned) start);

	if (argc > 1)
		root = argv[1];

	printf ("Legend: number of digits, running time and standard "
		"deviation in nanoseconds\n");

	printf ("\nTest a + b\n\n");

	for (len = 1; len <= MAX_LEN; ++len)
		gauge (len, 10000, gauge_add, root, "add");

	printf ("\nTest a * b\n\n");

	for (len = 1; len <= MAX_LEN; ++len)
		gauge (len, 3000, gauge_mul, root, "mul");

	printf ("\nTest a / b\n\n");

	for (len = 1; len <= MAX_LEN; ++len)
		gauge (len, 2000, gauge_div, root, "div");

	return 0;
}
