/*
 * MP Core Tests
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>

#include <mp/add.h>
#include <mp/conv.h>
#include <mp/shift.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)  (sizeof (a) / sizeof ((a)[0]))
#endif

static void mp_show (const char *prefix, const digit_t *x, size_t len)
{
	char s[len * 16 + 1];

	mp_save_hex (s, sizeof (s), x, len);
	printf ("%s%s\n", prefix, s);
}

struct lshift_sample {
	const char *X; int n; const char *R;
};

static const struct lshift_sample lshift_sample[] = {
	{
		"d41d8cd98f00b204e9800998ecf8427f", 4,
		"41d8cd98f00b204e9800998ecf8427f0"
	},
	{
		"d41d8cd98f00b204e9800998ecf8427f", 8,
		"1d8cd98f00b204e9800998ecf8427f00"
	},
	{
		"d41d8cd98f00b204e9800998ecf8427f", 12,
		"d8cd98f00b204e9800998ecf8427f000"
	},
	{
		"d41d8cd98f00b204e9800998ecf8427f", 16,
		"8cd98f00b204e9800998ecf8427f0000"
	},
};

static int do_lshift_test (const struct lshift_sample *o)
{
	digit_t x[8], r[8], res[8];
	size_t len = mp_load_hex (x, ARRAY_SIZE (x), o->X);
	int ok;

	printf ("lshift test:\n");

	mp_show ("\tX  = ", x, len);
	printf ("\tn  = %d\n", o->n);

	mp_load_hex (r, ARRAY_SIZE (r), o->R);  /* use mp_zext in generic case */
	mp_show ("\tR  = ", r, len);

	mp_lshift (res, x, len, 0, o->n);
	mp_show ("\tR' = ", res, len);

	ok = mp_cmp_n (r, res, len) == 0;
	printf ("\t%s\n", ok ? "passed" : "failed");
	return ok;
}

static int do_lshift_tests (void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE (lshift_sample); ++i)
		if (!do_lshift_test (lshift_sample + i))
			return 0;

	return 1;
}

struct rshift_sample {
	const char *X; int n; const char *R;
};

static const struct rshift_sample rshift_sample[] = {
	{
		"d41d8cd98f00b204e9800998ecf8427f", 4,
		"0d41d8cd98f00b204e9800998ecf8427"
	},
	{
		"d41d8cd98f00b204e9800998ecf8427f", 8,
		"00d41d8cd98f00b204e9800998ecf842"
	},
	{
		"d41d8cd98f00b204e9800998ecf8427f", 12,
		"000d41d8cd98f00b204e9800998ecf84"
	},
	{
		"d41d8cd98f00b204e9800998ecf8427f", 16,
		"0000d41d8cd98f00b204e9800998ecf8"
	},
};

static int do_rshift_test (const struct rshift_sample *o)
{
	digit_t x[8], r[8], res[8];
	size_t len = mp_load_hex (x, ARRAY_SIZE (x), o->X);
	int ok;

	printf ("rshift test:\n");

	mp_show ("\tX  = ", x, len);
	printf ("\tn  = %d\n", o->n);

	mp_load_hex (r, ARRAY_SIZE (r), o->R);  /* use mp_zext in generic case */
	mp_show ("\tR  = ", r, len);

	mp_rshift (res, x, len, 0, o->n);
	mp_show ("\tR' = ", res, len);

	ok = mp_cmp_n (r, res, len) == 0;
	printf ("\t%s\n", ok ? "passed" : "failed");
	return ok;
}

static int do_rshift_tests (void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE (rshift_sample); ++i)
		if (!do_rshift_test (rshift_sample + i))
			return 0;

	return 1;
}

int main (int argc, char *argv[])
{
	return do_lshift_tests () && do_rshift_tests () ? 0 : 1;
}
