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
#include <php.h>
#include <mpdecimal.h>
#include "common.h"
#include "compare.h"
#include "context.h"
#include "convert.h"
#include "limits.h"
#include "math.h"
#include "parse.h"

/**
 * Attempt to compare op1 and op2. It is possible that the comparison is not
 * defined (when comparing to NAN), so we should return a special comparison
 * result that indicates an undefined result. Returning 1 here is no good
 * because operations like "greater than" would be true for NAN.
 */
static int php_decimal_compare_mpd_to_mpd(const mpd_t *op1, const mpd_t *op2)
{
    uint32_t status = 0;

    int result = mpd_qcmp(op1, op2, &status);

    if (UNEXPECTED(status & MPD_Invalid_operation)) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    }

    return result;
}

/**
 *
 */
static int php_decimal_compare_mpd_to_double(const mpd_t *op1, const double op2)
{
    int result;
    PHP_DECIMAL_TEMP_MPD(tmp);

    php_decimal_set_double(&tmp, op2);
    result = php_decimal_compare_mpd_to_mpd(op1, &tmp);

    mpd_del(&tmp);
    return result;
}

/**
 * Compares a decimal to a non-decimal zval.
 */
static int php_decimal_compare_mpd_to_scalar(const mpd_t *op1, const zval *op2)
{
    switch (Z_TYPE_P(op2)) {
        case IS_NULL:
        case IS_FALSE:
            return 1;

        case IS_TRUE:
            return 0;

        /* Allow comparing to float. */
        case IS_DOUBLE:
            return php_decimal_compare_mpd_to_double(op1, Z_DVAL_P(op2));

        /* Attempt to parse the value, then compare. */
        /* Return unknown on failure to avoid false "greater than" indicator. */
        default: {
            int result;
            PHP_DECIMAL_TEMP_MPD(tmp);

            if (php_decimal_parse_scalar_quiet(&tmp, op2) == FAILURE) {
                return PHP_DECIMAL_COMPARISON_FAILURE;
            }

            result = php_decimal_compare_mpd_to_mpd(op1, &tmp);

            mpd_del(&tmp);
            return result;
        }
    }
}

static inline int php_decimal_compare_precision(const php_decimal_t *op1, const php_decimal_t *op2)
{
    const zend_long p1 = php_decimal_get_prec(op1);
    const zend_long p2 = php_decimal_get_prec(op2);

    return p1 == p2 ? 0 : (p1 < p2 ? -1 : 1);
}

/**
 * Compares two decimals using value-only comparison, precision is ignored.
 */
static int php_decimal_compare_decimal_to_decimal(const php_decimal_t *op1, const php_decimal_t *op2)
{
    const mpd_t *a = PHP_DECIMAL_MPD(op1);
    const mpd_t *b = PHP_DECIMAL_MPD(op2);

    int result = php_decimal_compare_mpd_to_mpd(a, b);

    /* */
    if (result == 0) {
        return php_decimal_compare_precision(op1, op2);
    }

    return result;
}

static int php_decimal_compare_decimal_to_rational(const php_decimal_t *op1, const php_rational_t *op2)
{
    int result;

    PHP_DECIMAL_TEMP_MPD(tmp);

    uint32_t status = 0;

    const mpd_t *num = PHP_RATIONAL_NUM(op2);
    const mpd_t *den = PHP_RATIONAL_DEN(op2);

    /* */
    PHP_DECIMAL_WITH_PRECISION(PHP_DECIMAL_MPD(op1)->digits, {
        mpd_qmul(&tmp, PHP_DECIMAL_MPD(op1), den, SHARED_CONTEXT, &status);
    });

    result = php_decimal_compare_mpd_to_mpd(&tmp, num);

    mpd_del(&tmp);
    return result;
}

/**
 * Compares a decimal to a zval that could also be a decimal.
 */
int php_decimal_compare_decimal_to_zval(const php_decimal_t *op1, const zval *op2)
{
    if (Z_IS_DECIMAL_P(op2)) {
        return php_decimal_compare_decimal_to_decimal(op1, Z_DECIMAL_P(op2));
    }

    if (Z_IS_RATIONAL_P(op2)) {
        return php_decimal_compare_decimal_to_rational(op1, Z_RATIONAL_P(op2));
    }

    return php_decimal_compare_mpd_to_scalar(PHP_DECIMAL_MPD(op1), op2);
}

static int php_decimal_compare_rational_num_den(const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2)
{
    int result;
    PHP_DECIMAL_TEMP_MPD(a);
    PHP_DECIMAL_TEMP_MPD(b);

    uint32_t status = 0;

    mpd_qmul(&a, num1, den2, MAX_CONTEXT, &status);
    mpd_qmul(&b, num2, den1, MAX_CONTEXT, &status);

    result = php_decimal_compare_mpd_to_mpd(&a, &b);

    mpd_del(&a);
    mpd_del(&b);
    return result;
}

static int php_decimal_compare_rational_to_double(const php_rational_t *op1, double op2)
{
    int result;
    PHP_DECIMAL_TEMP_MPD(num);
    PHP_DECIMAL_TEMP_MPD(den);

    const mpd_t *num1 = PHP_RATIONAL_NUM(op1);
    const mpd_t *den1 = PHP_RATIONAL_DEN(op1);
    const mpd_t *num2 = &num;
    const mpd_t *den2 = &den;

    php_decimal_set_double(&num, op2);
    php_decimal_set_one(&den);
    php_decimal_rational_normalize(&num, &den);

    result = php_decimal_compare_rational_num_den(num1, den1, num2, den2);

    mpd_del(&num);
    mpd_del(&den);
    return result;
}

/**
 * Compares a rational to a non-rational zval.
 */
static int php_decimal_compare_rational_to_scalar(const php_rational_t *op1, const zval *op2)
{
    switch (Z_TYPE_P(op2)) {
        case IS_NULL:
        case IS_FALSE:
            return 1;

        case IS_TRUE:
            return 0;

        /* Allow comparing to float. */
        case IS_DOUBLE:
            return php_decimal_compare_rational_to_double(op1, Z_DVAL_P(op2));

        /* Attempt to parse the value, then compare. */
        /* Return unknown on failure to avoid false "greater than" indicator. */
        default: {
            int result;
            PHP_DECIMAL_TEMP_MPD(num);
            PHP_DECIMAL_TEMP_MPD(den);

            const mpd_t *num1 = PHP_RATIONAL_NUM(op1);
            const mpd_t *den1 = PHP_RATIONAL_DEN(op1);
            const mpd_t *num2 = &num;
            const mpd_t *den2 = &den;

            if (php_decimal_parse_scalar_quiet(&num, op2) == FAILURE) {
                return PHP_DECIMAL_COMPARISON_FAILURE;
            }

            php_decimal_set_one(&den);
            php_decimal_rational_normalize(&num, &den);

            result = php_decimal_compare_rational_num_den(num1, den1, num2, den2);

            mpd_del(&num);
            mpd_del(&den);
            return result;
        }
    }
}

static inline int php_decimal_compare_rational_to_decimal(const php_rational_t *op1, const php_decimal_t *op2)
{
    int result = php_decimal_compare_decimal_to_rational(op2, op1);

    /* */
    if (UNEXPECTED(result == PHP_DECIMAL_COMPARISON_UNDEFINED)) {
        return result;
    }

    return -result;
}

/**
 *
 */
static int php_decimal_compare_rational_to_rational(const php_rational_t *op1, const php_rational_t *op2)
{
    int result;

    const mpd_t *num1 = PHP_RATIONAL_NUM(op1);
    const mpd_t *den1 = PHP_RATIONAL_DEN(op1);
    const mpd_t *num2 = PHP_RATIONAL_NUM(op2);
    const mpd_t *den2 = PHP_RATIONAL_DEN(op2);

    uint32_t status = 0;

    PHP_DECIMAL_TEMP_MPD(a);
    PHP_DECIMAL_TEMP_MPD(b);

    mpd_qmul(&a, num1, den2, MAX_CONTEXT, &status);
    mpd_qmul(&b, num2, den1, MAX_CONTEXT, &status);

    result = php_decimal_compare_mpd_to_mpd(&a, &b);

    mpd_del(&a);
    mpd_del(&b);
    return result;
}

int php_decimal_compare_rational_to_zval(const php_rational_t *op1, const zval *op2)
{
    if (Z_IS_RATIONAL_P(op2)) {
        return php_decimal_compare_rational_to_rational(op1, Z_RATIONAL_P(op2));
    }

    if (Z_IS_DECIMAL_P(op2)) {
        return php_decimal_compare_rational_to_decimal(op1, Z_DECIMAL_P(op2));
    }

    return php_decimal_compare_rational_to_scalar(op1, op2);
}

