/**
 * The MIT License (MIT)
 * Copyright (c) 2018 Rudi Theunissen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including  without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef HAVE_PHP_DECIMAL_MATH_H
#define HAVE_PHP_DECIMAL_MATH_H

#include <php.h>
#include <mpdecimal.h>

/**
 * Rational helpers
 */
void php_decimal_rational_normalize(mpd_t *num, mpd_t *den);
void php_decimal_rational_simplify(mpd_t *num, mpd_t *den);
void php_decimal_rational_evaluate(mpd_t *res, const php_rational_t *obj, zend_long prec);
void php_decimal_rational_from_mpd(mpd_t *num, mpd_t *den, const mpd_t *mpd);
int  php_decimal_rational_parity(const php_rational_t *obj);

/**
 * Decimal binary ops
 */
typedef void (*php_decimal_binary_op_t)(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);

void php_decimal_add     (mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);
void php_decimal_sub     (mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);
void php_decimal_mul     (mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);
void php_decimal_div     (mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);
void php_decimal_pow     (mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);
void php_decimal_mod     (mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);
void php_decimal_rem     (mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);
void php_decimal_shiftl  (mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);
void php_decimal_shiftr  (mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec);

/**
 * Decimal unary ops
 */
typedef void (*php_decimal_unary_op_t) (mpd_t *res, const mpd_t *op1, zend_long prec);

void php_decimal_abs     (mpd_t *res, const mpd_t *op1, zend_long prec);
void php_decimal_neg     (mpd_t *res, const mpd_t *op1, zend_long prec);
void php_decimal_ln      (mpd_t *res, const mpd_t *op1, zend_long prec);
void php_decimal_log10   (mpd_t *res, const mpd_t *op1, zend_long prec);
void php_decimal_sqrt    (mpd_t *res, const mpd_t *op1, zend_long prec);
void php_decimal_exp     (mpd_t *res, const mpd_t *op1, zend_long prec);
void php_decimal_floor   (mpd_t *res, const mpd_t *op1, zend_long prec);
void php_decimal_ceil    (mpd_t *res, const mpd_t *op1, zend_long prec);
void php_decimal_trunc   (mpd_t *res, const mpd_t *op1, zend_long prec);

/**
 * Rational binary ops
 */
typedef void (*php_decimal_binary_rop_t)(mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);

void php_decimal_radd    (mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);
void php_decimal_rsub    (mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);
void php_decimal_rmul    (mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);
void php_decimal_rdiv    (mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);
void php_decimal_rmod    (mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);
void php_decimal_rrem    (mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);
void php_decimal_rpow    (mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);
void php_decimal_rshiftl (mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);
void php_decimal_rshiftr (mpd_t *rnum, mpd_t *rden, const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2);

/**
 * Rational unary ops
 */
typedef void (*php_decimal_unary_rop_t) (mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den);

void php_decimal_rabs    (mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den);
void php_decimal_rneg    (mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den);
void php_decimal_rfloor  (mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den);
void php_decimal_rceil   (mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den);
void php_decimal_rtrunc  (mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den);

#endif
