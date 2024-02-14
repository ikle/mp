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
#include <mp/mont-mul.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)  (sizeof (a) / sizeof ((a)[0]))
#endif

static int mu_test (digit_t x)
{
	digit_t y, z;

	y = mp_mont_mu (x);
	z = x * (0 - y);

	if (z == 1)
		return 1;

	fprintf (stderr, "E: %lx * -%lx (mod B) = %lx != 1\n", x, y, z);
	return 0;
}

static digit_t m0[] = {
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

static int mu_make_tests (void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE (m0); ++i)
		if (!mu_test (m0[i]))
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

static void mp_show (const char *prefix, const digit_t *x, size_t len)
{
	char s[len * 16 + 1];

	mp_save_hex (s, sizeof (s), x, len);
	printf ("%s%s\n", prefix, s);
}

static int make_test (void)
{
	digit_t m[8], mu, ro[8], a[8], am[8], r[8];
	size_t len = mp_load_hex (m, ARRAY_SIZE (m), M);

	mp_show ("M  = ", m, len);

	mu = mp_mont_mu (m[0]);
	mp_mont_ro (ro, m, len);
	mp_show ("ro = ", ro, len);

	mp_load_hex (a, ARRAY_SIZE (a), A);
	mp_show ("A  = ", a, len);

	mp_mont_push_n (am, a, ro, m, len, mu);
	mp_show ("Am = ", am, len);

	mp_mont_pull_n (r, am, m, len, mu);
	mp_show ("A  = ", r, len);

	if (mp_cmp_n (a, r, len) == 0)
		return 1;

	fprintf (stderr, "E: Full failed, result does not match\n");
	return 0;
}

int main (int argc, char *argv[])
{
	return mu_make_tests () && make_test () ? 0 : 1;
}
