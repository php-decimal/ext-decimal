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
#include <Zend/zend_interfaces.h>
#include <mpdecimal.h>
#include "bool.h"
#include "parse.h"
#include "convert.h"
#include "errors.h"
#include "decimal.h"
#include "number.h"
#include "rational.h"
#include "math.h"
#include "context.h"
#include "limits.h"

/**
 *
 */
static void php_decimal_copy_rational(mpd_t *num, mpd_t *den, const php_rational_t *obj)
{
    uint32_t status = 0;

    mpd_qcopy(num, PHP_RATIONAL_NUM(obj), &status);
    mpd_qcopy(den, PHP_RATIONAL_DEN(obj), &status);
}

/**
 *
 */
static php_decimal_success_t php_decimal_parse_number_to_decimal(zval *result, const zval *val, zend_long prec)
{
    zval _prec;
    ZVAL_LONG(&_prec, prec);
    zend_call_method_with_1_params((zval *) val, Z_OBJCE_P(val), NULL, "todecimal", result, &_prec);

    return EG(exception) ? FAILURE : SUCCESS;
}

/**
 *
 */
static php_decimal_success_t php_decimal_parse_number_to_rational(zval *result, const zval *val)
{
    zend_call_method_with_0_params((zval *) val, Z_OBJCE_P(val), NULL, "torational", result);

    return EG(exception) ? FAILURE : SUCCESS;
}

/**
 *
 */
static php_decimal_success_t php_decimal_copy_mpd_to_decimal(php_decimal_t *dst, const mpd_t *src)
{
    uint32_t status = 0;

    mpd_qcopy(PHP_DECIMAL_MPD(dst), src, &status);

    return SUCCESS;
}

/**
 *
 */
static php_decimal_success_t php_decimal_copy_decimal_to_mpd(mpd_t *dst, const php_decimal_t *src)
{
    uint32_t status = 0;

    mpd_qcopy(dst, PHP_DECIMAL_MPD(src), &status);

    return SUCCESS;
}

/**
 * Attempts to parse a non-object value as a decimal, using a given precision.
 */
static php_decimal_success_t php_decimal_parse_scalar_ex(mpd_t *res, const zval *val, zend_bool quiet)
{
    switch (Z_TYPE_P(val)) {

        /* */
        case IS_STRING: {
            if (php_decimal_set_string(res, Z_STR_P(val)) == SUCCESS) {
                return SUCCESS;
            }

            if (!quiet) {
                php_decimal_failed_to_parse_string(Z_STR_P(val));
            }

            return FAILURE;
        }

        case IS_LONG: {
            php_decimal_set_long(res, Z_LVAL_P(val));
            return SUCCESS;
        }

        /* Should only consider special values here - float is not supported. */
        case IS_DOUBLE: {
            if (php_decimal_set_special(res, Z_DVAL_P(val)) == SUCCESS) {
                return SUCCESS;
            }
        }

        default: {
            if (!quiet) {
                php_decimal_unsupported_type();
            }
            return FAILURE;
        }
    }
}

/**
 * Attempts to parse a non-object value as a decimal, using a given precision.
 */
php_decimal_success_t php_decimal_parse_scalar(mpd_t *res, const zval *val)
{
    return php_decimal_parse_scalar_ex(res, val, false);
}

php_decimal_success_t php_decimal_parse_scalar_quiet(mpd_t *res, const zval *val)
{
    return php_decimal_parse_scalar_ex(res, val, true);
}

php_decimal_success_t php_decimal_parse_num_den(mpd_t *num, mpd_t *den, const zval *val)
{
    if (Z_TYPE_P(val) != IS_OBJECT) {
        if (php_decimal_parse_scalar(num, val) == FAILURE) {
            return FAILURE;
        }

        /* */
        php_decimal_rational_from_mpd(num, den, num);
        return SUCCESS;
    }

    if (Z_OBJ_IS_DECIMAL_P(val)) {
        php_decimal_rational_from_mpd(num, den, Z_MPD_P(val));
        return SUCCESS;
    }

    if (Z_OBJ_IS_RATIONAL_P(val)) {
        php_decimal_copy_rational(num, den, Z_RATIONAL_P(val));
        return SUCCESS;
    }

    if (Z_IMPLEMENTS_NUMBER_P(val)) {
        zval tmp;
        if (php_decimal_parse_number_to_rational(&tmp, val) == FAILURE) {
            return FAILURE;
        }

        php_decimal_copy_rational(num, den, Z_RATIONAL_P(&tmp));
        zval_ptr_dtor(&tmp);
        return SUCCESS;
    }

    /**
     *
     */
    php_decimal_unsupported_type();
    return FAILURE;
}

/**
 *
 */
php_decimal_success_t php_decimal_parse_mpd(mpd_t *res, const zval *val, const zend_long prec)
{
    if (Z_TYPE_P(val) != IS_OBJECT) {
        return php_decimal_parse_scalar(res, val);
    }

    if (Z_OBJ_IS_DECIMAL_P(val)) {
        return php_decimal_copy_decimal_to_mpd(res, Z_DECIMAL_P(val));
    }

    if (Z_OBJ_IS_RATIONAL_P(val)) {
        php_decimal_rational_evaluate(res, Z_RATIONAL_P(val), prec);
        return SUCCESS;
    }

    if (Z_IMPLEMENTS_NUMBER_P(val)) {
        zval tmp;
        if (php_decimal_parse_number_to_decimal(&tmp, val, prec) == FAILURE) {
            php_decimal_set_nan(res);
            return FAILURE;
        }

        php_decimal_copy_decimal_to_mpd(res, Z_DECIMAL_P(&tmp));
        zval_ptr_dtor(&tmp);
        return SUCCESS;
    }

    /**
     *
     */
    php_decimal_unsupported_type();
    return FAILURE;
}

/**
 *
 */
php_decimal_success_t php_decimal_parse_decimal(zval *result, const zval *val, const zend_long prec, zend_bool inherit)
{
    /* */
    if (Z_TYPE_P(val) != IS_OBJECT) {
        ZVAL_DECIMAL(result, php_decimal_with_prec(prec));

        /* */
        if (php_decimal_parse_scalar(Z_MPD_P(result), val) == FAILURE) {
            zval_ptr_dtor(result);
            ZVAL_UNDEF(result);
            return FAILURE;
        }

        return SUCCESS;

    /* */
    } else if (Z_OBJ_IS_DECIMAL_P(val)) {
        php_decimal_t *src = Z_DECIMAL_P(val);
        php_decimal_t *dst = php_decimal();

        php_decimal_set_prec(dst, inherit ? php_decimal_get_prec(src) : prec);
        php_decimal_copy_mpd_to_decimal(dst, PHP_DECIMAL_MPD(src));

        ZVAL_DECIMAL(result, dst);
        return SUCCESS;

    /* */
    } else if (Z_OBJ_IS_RATIONAL_P(val)) {
        php_decimal_rational_evaluate(Z_MPD_P(result), Z_RATIONAL_P(val), prec);
        return SUCCESS;

    /* */
    } else if (Z_IMPLEMENTS_NUMBER_P(val)) {
        if (php_decimal_parse_number_to_decimal(result, val, prec) == SUCCESS) {
            return SUCCESS;
        }
    }

    /* */
    php_decimal_unsupported_type();
    return FAILURE;
}

/**
 *
 */
php_decimal_success_t php_decimal_parse_rational(zval *result, const zval *val)
{
    /* */
    if (Z_TYPE_P(val) != IS_OBJECT) {
        php_rational_t *res = php_rational();
        mpd_t          *num = PHP_RATIONAL_NUM(res);
        mpd_t          *den = PHP_RATIONAL_DEN(res);

        ZVAL_RATIONAL(result, res);

        /* */
        if (php_decimal_parse_num_den(num, den, val) == FAILURE) {
            zval_ptr_dtor(result);
            ZVAL_UNDEF(result);
            return FAILURE;
        }

        php_decimal_rational_simplify(num, den);
        return SUCCESS;
    }

    /* */
    if (Z_OBJ_IS_RATIONAL_P(val)) {
        ZVAL_COPY(result, val);
        return SUCCESS;
    }

    /* */
    if (Z_OBJ_IS_DECIMAL_P(val)) {
        php_rational_t *res = php_rational();
        mpd_t          *num = PHP_RATIONAL_NUM(res);
        mpd_t          *den = PHP_RATIONAL_DEN(res);

        php_decimal_rational_from_mpd(num, den, Z_MPD_P(val));
        php_decimal_rational_simplify(num, den);

        ZVAL_RATIONAL(result, res);
        return SUCCESS;
    }

    /* */
    if (Z_IMPLEMENTS_NUMBER_P(val)) {
        php_decimal_parse_number_to_rational(result, val);
        return SUCCESS;
    }

    /* */
    php_decimal_unsupported_type();
    return FAILURE;
}
