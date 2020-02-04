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
#include <Zend/zend_interfaces.h>
#include <ext/standard/basic_functions.h>
#include <mpdecimal.h>
#include "bool.h"
#include "decimal.h"
#include "rational.h"
#include "number.h"
#include "compare.h"
#include "context.h"
#include "convert.h"
#include "errors.h"
#include "limits.h"
#include "math.h"

/**
 *
 */
zend_bool php_decimal_rational_is_nan(const php_rational_t *obj)
{
    return mpd_isnan(PHP_RATIONAL_NUM(obj)); 
}

/**
 *
 */
zend_bool php_decimal_number_is_nan(const zval *obj)
{
    // if (Z_OBJ_IS_DECIMAL_P(obj)) {
    //     return mpd_isnan(Z_MPD_P(obj));
    // }

    // /* */
    // if (Z_OBJ_IS_RATIONAL_P(obj)) {
    //     return mpd_isnan(PHP_RATIONAL_NUM(Z_RATIONAL_P(obj))) 
    //         || mpd_isnan(PHP_RATIONAL_DEN(Z_RATIONAL_P(obj)));
    // }

    /* */
    return zend_isnan(php_decimal_number_to_double(obj));
}

/**
 *
 */
zend_bool php_decimal_number_is_inf(const zval *obj)
{
    // if (Z_OBJ_IS_DECIMAL_P(obj)) {
    //     return mpd_isinfinite(Z_MPD_P(obj));
    // }

    // /* */
    // if (Z_OBJ_IS_RATIONAL_P(obj)) {
    //     return mpd_isinfinite(PHP_RATIONAL_NUM(Z_RATIONAL_P(obj))) 
    //         || mpd_isinfinite(PHP_RATIONAL_DEN(Z_RATIONAL_P(obj)));
    // }

    /* */
    return zend_isinf(php_decimal_number_to_double(obj));
}

zend_bool php_decimal_number_is_zero(const zval *obj)
{
    zval zero;
    ZVAL_LONG(&zero, 0);

    return php_decimal_number_compare(obj, &zero) == 0;
}

/**
 *
 */
zend_bool php_decimal_number_is_integer(const zval *obj)
{
    zval trunc;
    zend_bool result;

    /* Check if NAN or INF first. */
    double dval = php_decimal_number_to_double(obj);

    if (UNEXPECTED(zend_isnan(dval) || zend_isinf(dval))) {
        return false;
    }

    /* Check if equal to truncated self. */
    zend_call_method_with_0_params((zval *) obj, Z_OBJCE_P(obj), NULL, "trunc", &trunc);
    result = php_decimal_number_compare(obj, &trunc) == 0;
    
    zval_ptr_dtor(&trunc);
    return result;
}

static zend_bool php_decimal_number_starts_with_minus(const zval *obj)
{
    zend_bool res;
    zend_string *str;
    zval tmp;

    /* Convert to string to check first character. */
    php_decimal_number_to_string(&tmp, obj);
    str = Z_STR(tmp);

    if (EXPECTED(ZSTR_LEN(str))) {
        res = ZSTR_VAL(str)[0] == '-';
    } else {
        res = false;
    }

    zval_ptr_dtor(&tmp);
    return res;
}

zend_bool php_decimal_number_is_positive(const zval *obj)
{
    return ! php_decimal_number_starts_with_minus(obj)
        && ! php_decimal_number_is_nan(obj)
        && ! php_decimal_number_is_zero(obj);
}

zend_bool php_decimal_number_is_negative(const zval *obj)
{
    return php_decimal_number_starts_with_minus(obj);
}

void php_decimal_mpd_set_one(mpd_t *mpd)
{
    mpd_set_uint(mpd, 1, MAX_CONTEXT);
}

void php_decimal_mpd_set_exp(mpd_t *mpd, mpd_ssize_t exp)
{
    mpd->exp = exp;
}

void php_decimal_mpd_set_zero(mpd_t *mpd)
{
    mpd_zerocoeff(mpd);
    php_decimal_mpd_set_exp(mpd, 0);
}

void php_decimal_mpd_set_nan(mpd_t *mpd)
{
    mpd_set_qnan(mpd);
}

void php_decimal_mpd_set_inf(mpd_t *mpd, uint8_t sign)
{
    mpd_set_infinity(mpd);
    mpd_set_sign(mpd, sign);
}

zend_bool php_decimal_mpd_is_one(const mpd_t *mpd)
{
    uint32_t status = 0;

    return mpd_qabs_uint(mpd, &status) == 1;
}

/**
 * Parses a string to a given precision. Trailing zeroes are preserved.
 */
php_decimal_success_t php_decimal_mpd_set_charptr(mpd_t *mpd, const char *str)
{
    uint32_t status = 0;

    mpd_qset_string(mpd, str, MAX_CONTEXT, &status);

    /* Check that the conversion was successful. */
    if (UNEXPECTED(status & MPD_Conversion_syntax)) {
        return FAILURE;
    }

    return SUCCESS;
}

/**
 * Parses a string to a given precision. Trailing zeroes are preserved.
 */
php_decimal_success_t php_decimal_mpd_set_string(mpd_t *mpd, const zend_string *str)
{
    return php_decimal_mpd_set_charptr(mpd, ZSTR_VAL(str));
}

/**
 * Sets an mpd to a given double value. Will only be successful if the double is
 * a special value, ie INF, -INF and NAN.
 */
php_decimal_success_t php_decimal_mpd_set_special(mpd_t *res, double dval)
{
    /* NAN */
    if (zend_isnan(dval)) {
        mpd_set_qnan(res);
        return SUCCESS;
    }

    /* INF, -INF */
    if (zend_isinf(dval)) {
        mpd_set_infinity(res);
        mpd_set_sign(res, dval > 0 ? MPD_POS : MPD_NEG);
        return SUCCESS;
    }

    return FAILURE;
}

/**
 * Sets an mpd to a given double value. If the double is not a special value, it
 * will be cast to string first. This is useful because we don't want to parse
 * floats when constructing, but we do when comparing.
 */
void php_decimal_mpd_set_double(mpd_t *res, double dval)
{
    zval tmp;
    zend_string *str;

    /**
     * Write the double to a stack zval, then parse its string value. This
     * makes sure that we stay consistent with string casting, at the cost
     * of a slower conversion.
     */
    ZVAL_DOUBLE(&tmp, dval);
    str = zval_get_string(&tmp);
    php_decimal_mpd_set_string(res, str);

    zend_string_free(str);
}

/**
 * Sets an mpd to a given long value.
 */
void php_decimal_mpd_set_long(mpd_t *mpd, zend_long lval)
{
    uint32_t status = 0;

    mpd_qset_ssize(mpd, lval, MAX_CONTEXT, &status);
}

/**
 * Sets an mpd to a given unsigned long value.
 */
void php_decimal_mpd_set_ulong(mpd_t *mpd, zend_ulong lval)
{
    uint32_t status = 0;

    mpd_qset_uint(mpd, (mpd_uint_t) lval, MAX_CONTEXT, &status);
}

double php_decimal_mpd_to_double(const mpd_t *mpd)
{
    if (UNEXPECTED(mpd_isspecial(mpd))) {
        if (mpd_isnan(mpd)) {
            return php_get_nan();
        }

        /* INF, -INF */
        return mpd_ispositive(mpd)
            ? +php_get_inf()
            : -php_get_inf();

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

zend_long php_decimal_mpd_to_long(const mpd_t *mpd)
{
    uint32_t  status = 0;
    zend_long result = 0;

    /* PHP converts to zero but that does not make sense and could hide bugs. */
    if (UNEXPECTED(mpd_isspecial(mpd))) {
        php_decimal_integer_from_special_is_not_defined();
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
 *
 * TODO: Could we make these persistent, so that we don't allocate every time?
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

zend_string *php_decimal_mpd_to_sci(const mpd_t *mpd)
{
    char          *str;
    zend_string   *res;
    mpd_ssize_t    len;

    if (UNEXPECTED(mpd_isspecial(mpd))) {
        return php_decimal_to_special_string(mpd);
    }

    str = mpd_format(mpd, "-E", MAX_CONTEXT);
    len = strlen(str);
    res = zend_string_init(str, len, 0);

    mpd_free(str);

    return res;
}

zend_string *php_decimal_mpd_to_serialized(const mpd_t *mpd)
{
    return php_decimal_mpd_to_sci(mpd);
}

zend_string *php_decimal_mpd_to_string(const mpd_t *mpd)
{
    char          *str;
    zend_string   *res;
    mpd_ssize_t    len;

    if (UNEXPECTED(mpd_isspecial(mpd))) {
        return php_decimal_to_special_string(mpd);
    }

    str = mpd_format(mpd, "-F", MAX_CONTEXT);
    len = strlen(str);
    res = zend_string_init(str, len, 0);

    mpd_free(str);

    return res;
}

zend_string *php_decimal_mpd_to_fixed(const mpd_t *mpd, zend_long places, zend_bool commas, php_decimal_rounding_t mode)
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

zend_string *php_decimal_rational_to_sci(const php_rational_t *obj, zend_long prec)
{
    zend_string *str;

    PHP_DECIMAL_TEMP_MPD(tmp);
    php_decimal_rational_evaluate(&tmp, obj, prec);

    str = php_decimal_mpd_to_sci(&tmp);
    mpd_del(&tmp);

    return str;
}

zend_string *php_decimal_rational_to_fixed(const php_rational_t *obj, zend_long places, zend_bool commas, php_decimal_rounding_t mode)
{
    PHP_DECIMAL_TEMP_MPD(tmp);

    if (php_decimal_rational_round(&tmp, obj, places, mode) == SUCCESS) {
        return php_decimal_mpd_to_fixed(&tmp, places, commas, mode);
    }

    return zend_string_init("", 0, 0);
}


zend_string *php_decimal_rational_to_string(const php_rational_t *obj)
{
    const mpd_t *num = PHP_RATIONAL_NUM(obj);
    const mpd_t *den = PHP_RATIONAL_DEN(obj);

    /* */
    if (php_decimal_mpd_is_one(den)) {
        return php_decimal_mpd_to_string(num);

    } else {
        zend_string *result = NULL;
        smart_str    buffer = {0};

        zend_string *snum = php_decimal_mpd_to_string(num);
        zend_string *sden = php_decimal_mpd_to_string(den);

        smart_str_append(&buffer, snum);
        smart_str_appendc(&buffer, '/');
        smart_str_append(&buffer, sden);
        smart_str_0(&buffer);

        zend_string_free(snum);
        zend_string_free(sden);

        return buffer.s;
    }
}

zend_long php_decimal_rational_to_long(const php_rational_t *obj)
{
    const mpd_t *num = PHP_RATIONAL_NUM(obj);
    const mpd_t *den = PHP_RATIONAL_DEN(obj);

    /* PHP converts to zero but that does not make sense and could hide bugs. */
    if (UNEXPECTED(mpd_isspecial(num))) {
        php_decimal_integer_from_special_is_not_defined();
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
    dval = php_decimal_mpd_to_double(&tmp);

    mpd_del(&tmp);
    return dval;
}

double php_decimal_number_to_double(const zval *obj)
{
    double dval;

    zval tmp;
    zend_call_method_with_0_params((zval *) obj, Z_OBJCE_P(obj), NULL, "tofloat", &tmp);
    dval = Z_DVAL(tmp);

    zval_ptr_dtor(&tmp);
    return dval;
}

zend_long php_decimal_number_to_long(const zval *obj)
{
    zend_long lval;

    zval tmp;
    zend_call_method_with_0_params((zval *) obj, Z_OBJCE_P(obj), NULL, "toint", &tmp);
    lval = Z_LVAL(tmp);

    zval_ptr_dtor(&tmp);
    return lval;
}

void php_decimal_number_to_string(zval *result, const zval *obj)
{
    zend_call_method_with_0_params((zval *) obj, Z_OBJCE_P(obj), NULL, "tostring", result);
}

void php_decimal_number_to_mpd(mpd_t *mpd, const zval *obj, const zend_long prec)
{
    zval zprec;
    zval result;
    
    uint32_t status = 0;
    
    ZVAL_LONG(&zprec, prec);
    zend_call_method_with_1_params((zval *) obj, Z_OBJCE_P(obj), NULL, "todecimal", &result, &zprec);
    mpd_qcopy(mpd, Z_MPD_P(&result), &status);

    assert(status == 0);
    zval_ptr_dtor(&result);
    zval_ptr_dtor(&zprec);
}

void php_decimal_number_to_num_den(mpd_t *num, mpd_t *den, const zval *obj)
{
    zval result;
    
    uint32_t status = 0;
    
    zend_call_method_with_0_params((zval *) obj, Z_OBJCE_P(obj), NULL, "torational", &result);
    
    mpd_qcopy(num, PHP_RATIONAL_NUM(Z_RATIONAL_P(&result)), &status);
    mpd_qcopy(den, PHP_RATIONAL_DEN(Z_RATIONAL_P(&result)), &status);

    assert(status == 0);
    zval_ptr_dtor(&result);
}
