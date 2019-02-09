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
#include <zend_smart_str.h>
#include <ext/standard/basic_functions.h>
#include <mpdecimal.h>
#include "bool.h"
#include "context.h"
#include "convert.h"
#include "errors.h"
#include "limits.h"
#include "math.h"

void php_decimal_set_one(mpd_t *mpd)
{
    mpd_set_uint(mpd, 1, MAX_CONTEXT);
}

void php_decimal_set_exp(mpd_t *mpd, mpd_ssize_t exp)
{
    mpd->exp = exp;
}

void php_decimal_set_zero(mpd_t *mpd)
{
    mpd_zerocoeff(mpd);
    php_decimal_set_exp(mpd, 0);
}

void php_decimal_set_nan(mpd_t *mpd)
{
    mpd_set_qnan(mpd);
}

void php_decimal_set_inf(mpd_t *mpd, uint8_t sign)
{
    mpd_set_infinity(mpd);
    mpd_set_sign(mpd, sign);
}

zend_bool php_decimal_is_one(const mpd_t *mpd)
{
    uint32_t status = 0;

    return mpd_qabs_uint(mpd, &status) == 1;
}

/**
 * Parses a string to a given precision. Trailing zeroes are preserved.
 */
php_decimal_success_t php_decimal_set_string(mpd_t *mpd, zend_string *str)
{
    uint32_t status = 0;

    mpd_qset_string(mpd, ZSTR_VAL(str), MAX_CONTEXT, &status);

    /* Check that the conversion was successful. */
    if (UNEXPECTED(status & MPD_Conversion_syntax)) {
        return FAILURE;
    }

    return SUCCESS;
}


/**
 * Sets an mpd to a given double value. Will only be successful if the double is
 * a special value, ie INF, -INF and NAN.
 */
php_decimal_success_t php_decimal_set_special(mpd_t *res, double dval)
{
    if (zend_isinf(dval)) {
        mpd_set_infinity(res);
        mpd_set_sign(res, dval > 0 ? MPD_POS : MPD_NEG);
        return SUCCESS;
    }

    if (zend_isnan(dval)) {
        mpd_set_qnan(res);
        return SUCCESS;
    }

    return FAILURE;
}

/**
 * Sets an mpd to a given double value. If the double is not a special value, it
 * will be cast to string first. This is useful because we don't want to parse
 * floats when constructing, but we do when comparing.
 */
void php_decimal_set_double(mpd_t *res, double dval)
{
    zend_string *str;

    zval tmp;
    ZVAL_DOUBLE(&tmp, dval);

    str = zval_get_string(&tmp);
    php_decimal_set_string(res, str);
    zend_string_free(str);
}


/**
 * Sets an mpd to a given long value.
 */
void php_decimal_set_long(mpd_t *mpd, zend_long lval)
{
    uint32_t status = 0;

    mpd_qset_ssize(mpd, lval, MAX_CONTEXT, &status);
}

/**
 * Sets an mpd to a given unsigned long value.
 */
void php_decimal_set_ulong(mpd_t *mpd, zend_ulong lval)
{
    uint32_t status = 0;

    mpd_qset_uint(mpd, (mpd_uint_t) lval, MAX_CONTEXT, &status);
}

double php_decimal_to_double(const mpd_t *mpd)
{
    if (UNEXPECTED(mpd_isspecial(mpd))) {
        if (mpd_isnan(mpd)) {
            return php_get_nan();
        }
        /* Infinity */
        return mpd_ispositive(mpd)
            ? +php_get_inf()
            : -php_get_inf();

    /* Priority path for zero. */
    } else if (mpd_iszerocoeff(mpd)) {
        return 0;

    /* Convert the decimal to a string first. */
    } else {
        char *str = mpd_to_sci(mpd, PHP_DECIMAL_EXP_STR_CASE);

        /* Attempt to parse the string to double. */
        double dval = zend_strtod(str, NULL);

        /* Check if limits were reached. */
        if (zend_isinf(dval)) {
            php_decimal_floating_point_overflow();

        } else if (dval == 0 && !mpd_iszero(mpd)) {
            php_decimal_floating_point_underflow();
        }

        mpd_free(str);
        return dval;
    }
}

zend_long php_decimal_to_long(const mpd_t *mpd)
{
    uint32_t  status = 0;
    zend_long result = 0;

    /* PHP converts to zero but that does not make sense and could hide bugs. */
    if (UNEXPECTED(mpd_isspecial(mpd))) {
        php_decimal_integer_from_special_is_undefined();
        return 0;
    }

    if (mpd_isinteger(mpd)) {
        result = mpd_qget_ssize(mpd, &status);

    /* Truncate to an integer value first, otherwise mpd_qget_ssize fails. */
    } else {
        PHP_DECIMAL_TEMP_MPD(tmp);
        mpd_qtrunc(&tmp, mpd, SHARED_CONTEXT, &status);

        result = mpd_qget_ssize(&tmp, &status);
        mpd_del(&tmp);
    }

    /* Check for overflow. */
    if (status & MPD_Invalid_operation) {
        php_decimal_integer_overflow();
        return 0;
    }

    return result;
}

/**
 * Special numbers should use the 3-letter uppercase representation. This macro
 * checks if mpd is special, and returns a zend_string immediately if it is.
 */
static inline zend_string *php_decimal_to_special_string(const mpd_t *mpd)
{
    if (mpd_isnan(mpd)) {
        return zend_string_init("NAN", 3, 0);
    }

    return mpd_ispositive(mpd)
        ? zend_string_init( "INF", 3, 0)
        : zend_string_init("-INF", 4, 0);
}

zend_string *php_decimal_to_string(const mpd_t *mpd)
{
    char          *str;
    zend_string   *res;
    mpd_ssize_t    len;

    if (mpd_isspecial(mpd)) {
        return php_decimal_to_special_string(mpd);
    }

    /* */
    if (abs(mpd->exp) >= PHP_DECIMAL_EXP_THRESHOLD) {
        len = mpd_to_sci_size(&str, mpd, PHP_DECIMAL_EXP_STR_CASE);

    /* */
    } else {
        str = mpd_format(mpd, "-F", MAX_CONTEXT);
        len = strlen(str);
    }

    res = zend_string_init(str, len, 0);
    mpd_free(str);

    return res;
}

zend_string *php_decimal_to_fixed(const mpd_t *mpd, zend_long places, zend_bool commas, php_decimal_rounding_t mode)
{
    char        *str;
    zend_string *res;
    smart_str    fmt = {0};

    PHP_DECIMAL_TEMP_MPD(tmp);

    if (UNEXPECTED(mpd_isspecial(mpd))) {
        return php_decimal_to_special_string(mpd);
    }

    /* Round first */
    php_decimal_round(&tmp, mpd, places, mode);

    /* Specify whether we want to separate thousands with a comma. */
    if (commas) {
        smart_str_appendl(&fmt, "-,.", 3);
    } else {
        smart_str_appendl(&fmt, "-.",  2);
    }

    /* Specify how many decimal places we want. */
    smart_str_append_long(&fmt, places);

    /* Fixed point representation. */
    smart_str_appendc(&fmt, 'F');
    smart_str_0(&fmt);

    str = mpd_format(&tmp, ZSTR_VAL(fmt.s), SHARED_CONTEXT);
    res = zend_string_init(str, strlen(str), 0);

    smart_str_free(&fmt);
    mpd_free(str);
    mpd_del(&tmp);

    return res;
}

zend_string *php_decimal_rational_to_string(const php_rational_t *obj)
{
    const mpd_t *num = PHP_RATIONAL_NUM(obj);
    const mpd_t *den = PHP_RATIONAL_DEN(obj);

    /* */
    if (php_decimal_is_one(den)) {
        return php_decimal_to_string(num);

    } else {
        zend_string *result = NULL;
        smart_str    buffer = {0};

        zend_string *snum = php_decimal_to_string(num);
        zend_string *sden = php_decimal_to_string(den);

        smart_str_append(&buffer, snum);
        smart_str_appendc(&buffer, '/');
        smart_str_append(&buffer, sden);
        smart_str_0(&buffer);

        zend_string_free(snum);
        zend_string_free(sden);

        return buffer.s;
    }
}

zend_string *php_decimal_rational_to_fixed(const php_rational_t *obj, zend_long places, zend_bool commas, php_decimal_rounding_t mode)
{
    PHP_DECIMAL_TEMP_MPD(tmp);

    if (php_decimal_rational_round(&tmp, obj, places, mode) == SUCCESS) {
        return php_decimal_to_fixed(&tmp, places, commas, mode);
    }

    return zend_string_init("", 0, 0);
}

zend_long php_decimal_rational_to_long(const php_rational_t *obj)
{
    const mpd_t *num = PHP_RATIONAL_NUM(obj);
    const mpd_t *den = PHP_RATIONAL_DEN(obj);

    /* PHP converts to zero but that does not make sense and could hide bugs. */
    if (UNEXPECTED(mpd_isspecial(num))) {
        php_decimal_integer_from_special_is_undefined();
        return 0;

    } else {
        uint32_t  status = 0;
        zend_long result = 0;

        PHP_DECIMAL_TEMP_MPD(quot);

        mpd_qdivint(&quot, num, den, MAX_CONTEXT, &status);
        result = mpd_qget_ssize(&quot, &status);
        mpd_del(&quot);

        /* Check for overflow. */
        if (status & MPD_Invalid_operation) {
            php_decimal_integer_overflow();
            return 0;
        }

        return result;
    }
}

double php_decimal_rational_to_double(const php_rational_t *obj)
{
    double dval;

    PHP_DECIMAL_TEMP_MPD(tmp);
    php_decimal_rational_evaluate(&tmp, obj, PHP_DECIMAL_DOUBLE_DIGITS);
    dval = php_decimal_to_double(&tmp);

    mpd_del(&tmp);
    return dval;
}

