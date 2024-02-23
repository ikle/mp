/*
 * Montgomery Multiplication Tests
 *
 * Copyright (c) 2014-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <string.h>

#include <mp/conv.h>
#include <mp/core.h>
#include <mp/mont-mul.h>
#include <mp/unit.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)  (sizeof (a) / sizeof ((a)[0]))
#endif

static void mp_show (const char *prefix, const digit_t *x, size_t len)
{
	char s[len * 16 + 1];

	mp_save_hex (s, sizeof (s), x, len);
	printf ("%s%s\n", prefix, s);
}

static int do_mu_test (digit_t x)
{
	digit_t y, z;

	y = mp_mont_mu (x);
	z = x * (0 - y);

	if (z == 1)
		return 1;

	fprintf (stderr, "E: %lx * -%lx (mod B) = %lx != 1\n", x, y, z);
	return 0;
}

static const digit_t mu_sample[] = {
	  0x03,   0x07,   0x0b,   0x0f,   0x13,   0x17,   0x1b,   0x1f,
	  0x33,   0x37,   0x3b,   0x3f,   0x73,   0x77,   0x7b,   0x7f,
	 0x103,  0x207,  0x30b,  0x40f,  0x513,  0x617,  0x71b,  0x81f,
	 0x933,  0xa37,  0xb3b,  0xc3f,  0xd73,  0xe77,  0xf7b, 0x107f,
	0x1103, 0x2207, 0x630b, 0x940f, 0xc513, 0xd617, 0xe71b, 0xf81f,
#if MP_DIGIT_BITS == 32
	 0x10933l,  0x30a37l,   0x71b3bl,   0xf5c3fl,  0x134d73l,  0x387e77l,
	0x610f7bl, 0xeb107fl, 0x1594bc1l, 0x93b4811l, 0xa6bc345l, 0xe72d84dl,
#elif MP_DIGIT_BITS == 64
	0x38a26b273ll, 0x538a26b273ll, 0x4838a26b273ll, 0xa4838a26b273ll,
	0x3a4838a26b273ll, 0xd3a4838a26b273ll, 0xfd3a4838a26b273ll,
	0xefd3a4838a26b273ll, 0xffffffffffffffffll,
#endif
};

static int do_mu_tests (void)
{
	size_t i;

	printf ("mu tests:\n");

	for (i = 0; i < ARRAY_SIZE (mu_sample); ++i)
		if (!do_mu_test (mu_sample[i]))
			break;

	printf ("\tpassed %zu tests\n", i);

	if (i == ARRAY_SIZE (mu_sample))
		return 1;

	printf ("\tfailed test #%zu\n", i);
	return 0;
}

static const char *const pull_sample[] = {
	/* M = SHA256('') */
	"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
	/* M = SHA('') */
	"da39a3ee5e6b4b0d3255bfef95601890afd80709",
	/* M = MD5('') + 1 */
	"d41d8cd98f00b204e9800998ecf8427f",
	/* M = MD5('') >> 4 */
	"d41d8cd98f00b204e9800998ecf8427",
};

static int do_pull_test (const char *M)
{
	digit_t m[8], mu, x[8], r[8];
	size_t len = mp_load_hex (m, ARRAY_SIZE (m), M);
	int ok;

	printf ("pull test:\n");
	mp_show ("\tM  = ", m, len);

	mu = mp_mont_mu (m[0]);
	mp_neg (x, m, len);			/* X = base^len mod M	*/
	mp_mont_pull_n (r, x, m, len, mu);	/* R = X * X^-1 mod M	*/

	ok = mp_normalize (r, len) == 1 && r[0] == 1;
	printf ("\t%s\n", ok ? "passed" : "failed");
	return ok;
}

static int do_pull_tests (void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE (pull_sample); ++i)
		if (!do_pull_test (pull_sample[i]))
			return 0;

	return 1;
}

/*
 * M = SHA256(''), A = SHA256('A')
 */
static const char M[] =
	"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
static const char A[] =
	"559aead08264d5795d3909718cdd05abd49572e84fe55590eef31a88a08fdffd";

static int make_test (void)
{
	digit_t m[8], mu, ro[8], a[8], am[8], r[8];
	size_t len = mp_load_hex (m, ARRAY_SIZE (m), M);

	mp_show ("M  = ", m, len);

	mu = mp_mont_mu (m[0]);
	mp_mont_ro (ro, m, len);
	mp_show ("ro = ", ro, len);

	mp_load_hex (a, ARRAY_SIZE (a), A);  /* use mp_zext in generic case */
	mp_show ("A  = ", a, len);

	mp_mont_push_n (am, a, ro, m, len, mu);
	mp_show ("Am = ", am, len);

	mp_mont_pull_n (r, am, m, len, mu);
	mp_show ("A  = ", r, len);

	if (mp_cmp_n (a, r, len) == 0)
		return 1;

	fprintf (stderr, "E: Full test failed, result does not match\n");
	return 0;
}

struct pow_sample {
	const char *M, *A, *B, *P;
};

static const struct pow_sample pow_sample[] = {
	{
		/*
		 * M = SHA256(''), A = SHA256('A'), B = SHA256('B'),
		 * P = (A * A^B) mod M
		 */
		"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
		"559aead08264d5795d3909718cdd05abd49572e84fe55590eef31a88a08fdffd",
		"df7e70e5021544f4834bbee64a9e3789febc4be81470df629cad6ddb03320a5c",
		"1ecb066f4c9072b8cd7977e2011519b1b550ff2d0d48fc689b548ec579b1a958"
	},
	{
		/*
		 * M = SHA(''), A = SHA('A'), B = SHA('B'),
		 * P = (A * A^B) mod M
		 */
		"da39a3ee5e6b4b0d3255bfef95601890afd80709",
		"6dcd4ce23d88e2ee9568ba546c007c63d9131c1b",
		"ae4f281df5a5d0ff3cad6371f76d5c29b6d953ec",
		"b4d18c8b96093f7432c9f33ce456c9a37d871085"
	},
	{
		/*
		 * M = MD5('') + 1, A = MD5('A'), B = MD5('B'),
		 * P = (A * A^B) mod M
		 */
		"d41d8cd98f00b204e9800998ecf8427f",
		"7fc56270e7a70fa81a5935b72eacbe29",
		"9d5ed678fe57bcca610140957afab571",
		"654c914145d02e984c3ceed12bb64e2b"
	},
	{
		/*
		 * M = MD5('') >> 4, A = MD5('A') >> 4, B = MD5('B') >> 4,
		 * P = (A * A^B) mod M
		 */
		"d41d8cd98f00b204e9800998ecf8427",
		"7fc56270e7a70fa81a5935b72eacbe2",
		"9d5ed678fe57bcca610140957afab57",
		"1fc541f4d8b93eb976d17f7e236ff0"
	},
};

static int do_pow_test (const struct pow_sample *o)
{
	digit_t m[8], mu, ro[8], a[8], am[8], b[8], rm[8], r[8], p[8];
	size_t len = mp_load_hex (m, ARRAY_SIZE (m), o->M);
	int ok;

	printf ("pow test:\n");

	mp_show ("\tM  = ", m, len);

	mu = mp_mont_mu (m[0]);
	mp_mont_ro_gen (ro, m, len);

	mp_load_hex (a, ARRAY_SIZE (a), o->A);  /* use mp_zext in generic case */
	mp_show ("\tA  = ", a, len);

	mp_mont_push_n (am, a, ro, m, len, mu);

	mp_load_hex (b, ARRAY_SIZE (b), o->B);  /* use mp_zext in generic case */
	mp_show ("\tB  = ", b, len);

	mp_copy (rm, am, len);
	mp_mont_pow_n (rm, am, b, m, len, mu);
	mp_mont_pull_n (r, rm, m, len, mu);
	mp_show ("\tR  = ", r, len);

	mp_load_hex (p, ARRAY_SIZE (p), o->P);  /* use mp_zext in generic case */

	ok = mp_cmp_n (r, p, len) == 0;
	printf ("\t%s\n", ok ? "passed" : "failed");
	return ok;
}

static int do_pow_tests (void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE (pow_sample); ++i)
		if (!do_pow_test (pow_sample + i))
			return 0;

	return 1;
}

int main (int argc, char *argv[])
{
	return	do_mu_tests () && do_pull_tests () &&
		make_test () && do_pow_tests () ? 0 : 1;
}
