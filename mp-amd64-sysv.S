/*
 * MP Core AMD64 SysV ABI Implemention
 *
 * Copyright (c) 2014-2020 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
	.text

.macro head label
	.p2align 4
	\label:
.endm

.macro entry name
	.globl	\name
	head \name
.endm

/*
 * char mp_add_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len)
 */

entry mp_add_n

#define R	%rdi
#define X	%rsi
#define Y	%rdx
#define LEN	%rcx
#define I	%r8

	test	LEN, LEN
	jz	1f
	xor	I, I		/* and reset input carry */
head 2
	mov	(X, I, 8), %rax
	adc	(Y, I, 8), %rax
	mov	%rax, (R, I, 8)
	inc	I
	dec	LEN
	jnz	2b
1:	setc	%al
	ret

#undef R
#undef X
#undef Y
#undef LEN
#undef I

/*
 * char mp_add_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
 */

entry mp_add_1

#define R	%rdi
#define X	%rsi
#define LEN	%rdx
#define Y	%rcx
#define I	%r8

	xor	I, I
	add	(X), Y
	mov	Y, (R)
	inc	I
	dec	LEN
	jz	1f
head 2
	mov	(X, I, 8), %rax
	adc	$0, %rax
	mov	%rax, (R, I, 8)
	inc	I
	dec	LEN
	jnz	2b
1:	setc	%al
	ret

#undef R
#undef X
#undef LEN
#undef Y
#undef I

/*
 * char mp_add (digit_t *r, const digit_t *x, size_t xlen,
 *			    const digit_t *y, size_t ylen)
 *
 * NOTE: xlen ≥ ylen
 */

entry mp_add

#define R	%rdi
#define X	%rsi
#define XLEN	%rdx
#define Y	%rcx
#define YLEN	%r8
#define I	%r9

	test	XLEN, XLEN
	jz	1f
	sub	YLEN, XLEN
	xor	I, I		/* and reset input carry */
head 2
	mov	(X, I, 8), %rax
	adc	(Y, I, 8), %rax
	mov	%rax, (R, I, 8)
	inc	I
	dec	YLEN
	jnz	2b
	inc	XLEN		/* this magic required to save CF */
	jmp	3f
head 4
	mov	(X, I, 8), %rax
	adc	$0, %rax
	mov	%rax, (R, I, 8)
	inc	I
3:	dec	XLEN
	jnz	4b
1:	setc	%al
	ret

#undef R
#undef X
#undef XLEN
#undef Y
#undef YLEN
#undef I

/*
 * char mp_sub_n (digit_t *r, const digit_t *x, const digit_t *y, size_t len)
 */

entry mp_sub_n

#define R	%rdi
#define X	%rsi
#define Y	%rdx
#define LEN	%rcx
#define I	%r8

	test	LEN, LEN
	jz	1f
	xor	I, I		/* and reset input carry */
head 2
	mov	(X, I, 8), %rax
	sbb	(Y, I, 8), %rax
	mov	%rax, (R, I, 8)
	inc	I
	dec	LEN
	jnz	2b
1:	setc	%al
	ret

#undef R
#undef X
#undef Y
#undef LEN
#undef I

/*
 * char mp_sub_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
 */

entry mp_sub_1

#define R	%rdi
#define X	%rsi
#define LEN	%rdx
#define Y	%rcx
#define I	%r8

	xor	I, I
	mov     (X), %rax
	sub	Y, %rax
	mov	%rax, (R)
	inc	I
	dec	LEN
	jz	1f
head 2
	mov	(X, I, 8), %rax
	sbb	$0, %rax
	mov	%rax, (R, I, 8)
	inc	I
	dec	LEN
	jnz	2b
1:	setc	%al
	ret

#undef R
#undef X
#undef LEN
#undef Y
#undef I

/*
 * char mp_sub (digit_t *r, const digit_t *x, size_t xlen,
 *			    const digit_t *y, size_t ylen)
 *
 * NOTE: xlen ≥ ylen
 */

entry mp_sub

#define R	%rdi
#define X	%rsi
#define XLEN	%rdx
#define Y	%rcx
#define YLEN	%r8
#define I	%r9

	test	XLEN, XLEN
	jz	1f
	sub	YLEN, XLEN
	xor	I, I		/* and reset input carry */
head 2
	mov	(X, I, 8), %rax
	sbb	(Y, I, 8), %rax
	mov	%rax, (R, I, 8)
	inc	I
	dec	YLEN
	jnz	2b
	inc	XLEN		/* this magic required to save CF */
	jmp	3f
head 4
	mov	(X, I, 8), %rax
	sbb	$0, %rax
	mov	%rax, (R, I, 8)
	inc	I
3:	dec	XLEN
	jnz	4b
1:	setc	%al
	ret

#undef R
#undef X
#undef XLEN
#undef Y
#undef YLEN
#undef I

/*
 * char mp_neg (digit_t *r, const digit_t *x, size_t len)
 */

entry mp_neg

#define R	%rdi
#define X	%rsi
#define LEN	%rdx
#define I	%rcx
#define Z	%r8

	test	LEN, LEN
	jz	1f
	xor	I, I		/* and reset input carry */
	mov	I, Z
head 2
	mov	Z, %rax
	sbb	(X, I, 8), %rax
	mov	%rax, (R, I, 8)
	inc	I
	dec	LEN
	jnz	2b
1:	setc	%al
	ret

#undef R
#undef X
#undef LEN
#undef I

/*
 * int mp_cmp_n (const digit_t *x, const digit_t *y, size_t len)
 */

entry mp_cmp_n

#define X	%rdi
#define Y	%rsi
#define LEN	%rdx

	test	LEN, LEN
	jz	1f
head 4
	mov	-8 (X, LEN, 8), %rax
	cmp	%rax, -8 (Y, LEN, 8)
	ja	2f
	jb	3f
	dec	LEN
	jnz	4b
1:	xor	%eax, %eax
	ret
2:	mov	$1, %eax
	ret
3:	mov	$0xffffffff,%eax
	ret

#undef R
#undef X
#undef Y
#undef LEN
#undef I

/*
 * digit_t mp_mul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
 */

entry mp_mul_1

#define R	%rdi
#define X	%rsi
#define LEN	%r10
#define Y	%rcx
#define C	%r9
#define I	%r8

	xor	C, C
	xor	I, I
	test	%rdx, %rdx
	mov	%rdx, LEN	/* rdx mangled by multiplication, save it */
	jz	1f
head 2
	mov	(X, I, 8), %rax
	mul	Y
	add	C,  %rax
	adc	$0, %rdx
	mov	%rax, (R, I, 8)
	mov	%rdx, C
	inc	I
	dec	LEN
	jne	2b
1:	mov	C, %rax
	ret

#undef R
#undef X
#undef LEN
#undef Y
#undef C
#undef I

/*
 * digit_t mp_addmul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
 */

entry mp_addmul_1

#define R	%rdi
#define X	%rsi
#define LEN	%r10
#define Y	%rcx
#define C	%r9
#define I	%r8

	xor	C, C
	xor	I, I
	test	%rdx, %rdx
	mov	%rdx, LEN	/* rdx mangled by multiplication, save it */
	jz	1f
head 2
	mov	(X, I, 8), %rax
	mul	Y
	add	C,  %rax
	adc	$0, %rdx
	add	%rax, (R, I, 8)
	adc	$0, %rdx
	mov	%rdx, C
	inc	I
	dec	LEN
	jne	2b
1:	mov	C, %rax
	ret

#undef R
#undef X
#undef LEN
#undef Y
#undef C
#undef I

/*
 * digit_t mp_submul_1 (digit_t *r, const digit_t *x, size_t len, digit_t y)
 */

entry mp_submul_1

#define R	%rdi
#define X	%rsi
#define LEN	%r10
#define Y	%rcx
#define C	%r9
#define I	%r8

	xor	C, C
	xor	I, I
	test	%rdx, %rdx
	mov	%rdx, LEN	/* rdx mangled by multiplication, save it */
	jz	1f
head 2
	mov	(X, I, 8), %rax
	mul	Y
	add	C,  %rax
	adc	$0, %rdx
	sub	%rax, (R, I, 8)
	adc	$0, %rdx
	mov	%rdx, C
	inc	I
	dec	LEN
	jne	2b
1:	mov	C, %rax
	ret

#undef R
#undef X
#undef LEN
#undef Y
#undef C
#undef I
