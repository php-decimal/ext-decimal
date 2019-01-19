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
#include <stdio.h>
#include <php.h>
#include <ext/standard/php_math.h>
#include <ext/standard/php_string.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>
#include <zend_smart_str.h>
#include <mpdecimal.h>
#include "arginfo.h"
#include "bool.h"
#include "common.h"
#include "compare.h"
#include "context.h"
#include "convert.h"
#include "decimal.h"
#include "errors.h"
#include "limits.h"
#include "math.h"
#include "number.h"
#include "params.h"
#include "parse.h"
#include "rational.h"
#include "round.h"

/*
CONSIDERATIONS
- Support initialization like "1/2"

/**
 * Class entry.
 */
zend_class_entry *php_decimal_rational_ce;

/**
 * Object handlers.
 */
static zend_object_handlers php_decimal_rational_handlers;


/******************************************************************************/
/*                         ALLOC, INIT, FREE, CLONE                           */
/******************************************************************************/

/**
 * Allocates a new php_decimal_t that has not been initialized. We don't want to
 * initialize a decimal until its constructor is called, so that we can check in
 * the constructor whether the object has already been initialized.
 */
static php_rational_t *php_decimal_rational_alloc()
{
    php_rational_t *obj = ecalloc(1, sizeof(php_rational_t));

    if (EXPECTED(obj)) {
        obj->std.handlers = &php_decimal_rational_handlers;
        zend_object_std_init((zend_object *) obj, php_decimal_rational_ce);
    } else {
        php_decimal_memory_error();
    }

    return obj;
}

/**
 * Creates a new decimal object, initialized to the default precision.
 */
php_rational_t *php_rational()
{
    php_rational_t *obj = php_decimal_rational_alloc();

    php_decimal_init_mpd(PHP_RATIONAL_NUM(obj));
    php_decimal_init_mpd(PHP_RATIONAL_DEN(obj));

    return obj;
}

/**
 * Creates a custom zend_object that is also an uninitialized decimal object.
 */
static zend_object *php_decimal_rational_create_object(zend_class_entry *ce)
{
    return (zend_object *) php_decimal_rational_alloc();
}

/**
 * Creates a copy of the given decimal object.
 */
static php_rational_t *php_decimal_rational_create_copy(php_rational_t *src)
{
    uint32_t status = 0;

    php_rational_t *dst = php_rational();

    mpd_qcopy(PHP_RATIONAL_NUM(dst), PHP_RATIONAL_NUM(src), &status);
    mpd_qcopy(PHP_RATIONAL_DEN(dst), PHP_RATIONAL_DEN(src), &status);

    return dst;
}

/**
 * Clones the given zval, which must be a decimal object.
 */
static zend_object *php_decimal_rational_clone_obj(zval *obj)
{
    return (zend_object *) php_decimal_rational_create_copy(Z_RATIONAL_P(obj));
}

/**
 * Free handler, should only free internal memory, not the object itself.
 */
static void php_decimal_rational_free_object(zend_object *obj)
{
    php_decimal_release_mpd(PHP_RATIONAL_NUM((php_rational_t*) obj));
    php_decimal_release_mpd(PHP_RATIONAL_DEN((php_rational_t*) obj));

    zend_object_std_dtor(obj);
}

/**
 *
 */
static inline zend_bool php_decimal_rational_is_integer(const php_rational_t *obj)
{
    return !mpd_isspecial(PHP_RATIONAL_NUM(obj)) && php_decimal_is_one(PHP_RATIONAL_DEN(obj));
}


/******************************************************************************/
/*                              OBJECT HANDLERS                               */
/******************************************************************************/

/**
 * Compares two zval's, one of which must be a decimal. This is the function
 * used by the compare handler, as well as compareTo.
 */
static php_decimal_success_t php_decimal_rational_compare(zval *res, zval *op1, zval *op2)
{
    int result;
    int invert;

    if (Z_IS_RATIONAL_P(op1)) {
        result = php_decimal_compare_rational_to_zval(Z_RATIONAL_P(op1), op2);
        invert = 0;
    } else {
        result = php_decimal_compare_rational_to_zval(Z_RATIONAL_P(op2), op1);
        invert = 1;
    }

    /* */
    if (UNEXPECTED(result == PHP_DECIMAL_COMPARISON_UNDEFINED)) {
        ZVAL_LONG(res, 1);
    } else {
        ZVAL_LONG(res, invert ? -result : result);
    }

    return SUCCESS;
}

/**
 * var_dump, print_r etc.
 */
static HashTable *php_decimal_rational_get_debug_info(zval *obj, int *is_temp)
{
    zval num;
    zval den;
    HashTable *debug_info;

    ALLOC_HASHTABLE(debug_info);
    zend_hash_init(debug_info, 2, NULL, ZVAL_PTR_DTOR, 0);

    ZVAL_STR(&num, php_decimal_to_string(PHP_RATIONAL_NUM(Z_RATIONAL_P(obj))));
    zend_hash_str_update(debug_info, "num", sizeof("num") - 1, &num);

    ZVAL_STR(&den, php_decimal_to_string(PHP_RATIONAL_DEN(Z_RATIONAL_P(obj))));
    zend_hash_str_update(debug_info, "den", sizeof("den") - 1, &den);

    *is_temp = 1;

    return debug_info;
}

/**
 * Cast to string, int, float or bool.
 */
static php_decimal_success_t php_decimal_rational_cast_object(zval *obj, zval *result, int type)
{
    switch (type) {
        case IS_STRING:
            ZVAL_STR(result, php_decimal_rational_to_string(Z_RATIONAL_P(obj)));
            return SUCCESS;

        case IS_LONG:
            ZVAL_LONG(result, php_decimal_rational_to_long(Z_RATIONAL_P(obj)));
            return SUCCESS;

        case IS_DOUBLE:
            ZVAL_DOUBLE(result, php_decimal_rational_to_double(Z_RATIONAL_P(obj)));
            return SUCCESS;

        case _IS_BOOL:
            ZVAL_BOOL(result, 1); /* Objects are always true */
            return SUCCESS;

        default:
            return FAILURE;
    }
}

/**
 * Converts a zend opcode to a binary arithmetic function pointer.
 *
 * Returns NULL if a function is not mapped.
 */
static php_decimal_binary_rop_t php_decimal_get_binary_rop(zend_uchar opcode)
{
    switch (opcode) {
        case ZEND_ADD: return php_decimal_radd;
        case ZEND_SUB: return php_decimal_rsub;
        case ZEND_MUL: return php_decimal_rmul;
        case ZEND_DIV: return php_decimal_rdiv;
        case ZEND_MOD: return php_decimal_rmod;
        case ZEND_POW: return php_decimal_rpow;
        case ZEND_SL:  return php_decimal_rshiftl;
        case ZEND_SR:  return php_decimal_rshiftr;
        default:
            return NULL;
    }
}

/**
 * Attempts a binary operation on two zval's, writing the result to res.
 *
 * We don't know which of the operands is a decimal, if not both.
 */
static void php_decimal_do_binary_rop(php_decimal_binary_rop_t rop, php_rational_t *res, zval *op1, zval *op2)
{
    mpd_t *num1;
    mpd_t *den1;
    mpd_t *num2;
    mpd_t *den2;
    mpd_t *rnum = PHP_RATIONAL_NUM(res);
    mpd_t *rden = PHP_RATIONAL_DEN(res);

    PHP_DECIMAL_TEMP_MPD(a);
    PHP_DECIMAL_TEMP_MPD(b);

    if (Z_IS_RATIONAL_P(op1)) {
        if (Z_IS_RATIONAL_P(op2)) {
            num1 = PHP_RATIONAL_NUM(Z_RATIONAL_P(op1));
            den1 = PHP_RATIONAL_DEN(Z_RATIONAL_P(op1));
            num2 = PHP_RATIONAL_NUM(Z_RATIONAL_P(op2));
            den2 = PHP_RATIONAL_DEN(Z_RATIONAL_P(op2));

        } else {
            num1 = PHP_RATIONAL_NUM(Z_RATIONAL_P(op1));
            den1 = PHP_RATIONAL_DEN(Z_RATIONAL_P(op1));
            num2 = &a;
            den2 = &b;

            if (php_decimal_parse_num_den(num2, den2, op2) == FAILURE) {
                goto failure;
            }
        }
    } else {
        num1 = &a;
        den1 = &b;
        num2 = PHP_RATIONAL_NUM(Z_RATIONAL_P(op2));
        den2 = PHP_RATIONAL_DEN(Z_RATIONAL_P(op2));

        if (php_decimal_parse_num_den(num1, den1, op1) == FAILURE) {
            goto failure;
        }
    }

    /* Parsed successfully, so we can attempt the rational op. */
    rop(rnum, rden, num1, den1, num2, den2);
    mpd_del(&a);
    mpd_del(&b);
    return;

failure:
    php_decimal_set_nan(rnum);
    php_decimal_set_one(rden);
    mpd_del(&a);
    mpd_del(&b);
}

/**
 * Operator overloading.
 *
 * We don't know which of op1 and op2 is a rational object (if not both).
 */
static php_decimal_success_t php_decimal_rational_do_operation(zend_uchar opcode, zval *result, zval *op1, zval *op2)
{
    zval op1_copy;
    php_decimal_binary_rop_t op = php_decimal_get_binary_rop(opcode);

    /* Unsupported operator - return success to avoid casting. */
    if (UNEXPECTED(op == NULL)) {
        php_decimal_operator_not_supported();
        return SUCCESS;
    }

    /* This allows for assign syntax, ie. $op1 /= $op2 */
    if (op1 == result) {
        ZVAL_COPY_VALUE(&op1_copy, op1);
        op1 = &op1_copy;
    }

    /* Attempt operation. */
    ZVAL_DECIMAL(result, php_rational());
    php_decimal_do_binary_rop(op, Z_RATIONAL_P(result), op1, op2);

    /**
     * Something went wrong so unset the result, but we don't want the engine to
     * carry on trying to cast the rational, so we return success.
     */
    if (UNEXPECTED(EG(exception))) {
        zval_ptr_dtor(result);
        ZVAL_UNDEF(result);
        return SUCCESS;
    }

    if (op1 == &op1_copy) {
        zval_dtor(op1);
    }

    return SUCCESS;
}


/******************************************************************************/
/*                               SERIALIZATION                                */
/******************************************************************************/

/**
 * Serialize
 */
static php_decimal_success_t php_decimal_rational_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    zval tmp;
    smart_str buf = {0};
    php_rational_t *obj = Z_RATIONAL_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    /* Serialize the numerator as a string. */
    ZVAL_STR(&tmp, php_decimal_to_string(PHP_RATIONAL_NUM(obj)));
    php_var_serialize(&buf, &tmp, &serialize_data);
    zval_ptr_dtor(&tmp);

    /* Serialize the denominator as a string. */
    ZVAL_STR(&tmp, php_decimal_to_string(PHP_RATIONAL_DEN(obj)));
    php_var_serialize(&buf, &tmp, &serialize_data);
    zval_ptr_dtor(&tmp);

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);

    *buffer = (unsigned char *) estrndup(ZSTR_VAL(buf.s), ZSTR_LEN(buf.s));
    *length = ZSTR_LEN(buf.s);

    smart_str_free(&buf);

    return SUCCESS;
}

/**
 * Unserialize
 */
static php_decimal_success_t php_decimal_rational_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    zval *num;
    zval *den;

    php_rational_t *res = php_rational();

    ZVAL_RATIONAL(object, res);

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *end = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    /* Unserialize the numerator. */
    num = var_tmp_var(&unserialize_data);
    if (!php_var_unserialize(num, &pos, end, &unserialize_data) || Z_TYPE_P(num) != IS_STRING) {
        goto error;
    }

    /* Unserialize the denominator. */
    den = var_tmp_var(&unserialize_data);
    if (!php_var_unserialize(den, &pos, end, &unserialize_data) || Z_TYPE_P(den) != IS_STRING) {
        goto error;
    }

    /* Check that we've parsed the entire serialized string. */
    if (pos != end) {
        goto error;
    }

    /* Attempt to parse the unserialized numerator, quietly, delegate to local error. */
    if (php_decimal_set_string(PHP_RATIONAL_NUM(res), Z_STR_P(num)) == FAILURE) {
        goto error;
    }

    /* Attempt to parse the unserialized denominator, quietly, delegate to local error. */
    if (php_decimal_set_string(PHP_RATIONAL_DEN(res), Z_STR_P(den)) == FAILURE) {
        goto error;
    }

    /* Success! Set as zval and return. */
    ZVAL_RATIONAL(object, res);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    zval_ptr_dtor(object);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    php_decimal_unserialize_error();
    return FAILURE;
}


/******************************************************************************/
/*                              PHP CLASS METHODS                             */
/******************************************************************************/


/**
 * Parse a rational binary operation (op1 OP op2).
 */
#define PHP_DECIMAL_PARSE_BINARY_ROP(rop) do { \
    zval *op2 = NULL; \
    \
    PHP_DECIMAL_PARSE_PARAMS(1, 1) \
        Z_PARAM_ZVAL(op2) \
    PHP_DECIMAL_PARSE_PARAMS_END() \
    { \
        php_rational_t *op1 = THIS_RATIONAL(); \
        php_rational_t *res = php_rational(); \
        \
        ZVAL_RATIONAL(return_value, res); \
        \
        PHP_DECIMAL_TEMP_MPD(a); \
        PHP_DECIMAL_TEMP_MPD(b); \
        \
        mpd_t *rnum = PHP_RATIONAL_NUM(res); \
        mpd_t *rden = PHP_RATIONAL_DEN(res); \
        mpd_t *num1 = PHP_RATIONAL_NUM(op1); \
        mpd_t *den1 = PHP_RATIONAL_DEN(op1); \
        mpd_t *num2 = &a; \
        mpd_t *den2 = &b; \
        \
        if (php_decimal_parse_num_den(num2, den2, op2) == FAILURE) { \
            zval_ptr_dtor(return_value); \
            mpd_del(&a); \
            mpd_del(&b); \
            return; \
        } \
        rop(rnum, rden, num1, den1, num2, den2); \
        mpd_del(&a); \
        mpd_del(&b); \
        \
        RETURN_RATIONAL(res); \
    } \
} while (0)

/**
 * Parse a rational unary operation (OP op1).
 */
#define PHP_DECIMAL_PARSE_UNARY_ROP(op) do { \
    php_rational_t *obj = THIS_RATIONAL(); \
    php_rational_t *res = php_rational(); \
    \
    mpd_t *rnum = PHP_RATIONAL_NUM(res); \
    mpd_t *rden = PHP_RATIONAL_DEN(res); \
    mpd_t *num  = PHP_RATIONAL_NUM(obj); \
    mpd_t *den  = PHP_RATIONAL_DEN(obj); \
    \
    PHP_DECIMAL_PARSE_PARAMS_NONE(); \
    op(rnum, rden, num, den); \
    RETURN_RATIONAL(res); \
} while (0)

/**
 * Rational::valueOf
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, valueOf, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, valueOf)
{
    zval *val = NULL;

    PHP_DECIMAL_PARSE_PARAMS(1, 1)
        Z_PARAM_ZVAL(val)
    PHP_DECIMAL_PARSE_PARAMS_END()

    php_decimal_parse_rational(return_value, val);
}

/**
 * Rational::__construct
 */
PHP_DECIMAL_ARGINFO(Rational, __construct, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, __construct)
{
    /* Does nothing, is private. */
}

/**
 * Rational::add
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, add, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, add)
{
    PHP_DECIMAL_PARSE_BINARY_ROP(php_decimal_radd);
}

/**
 * Rational::sub
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, sub, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, sub)
{
    PHP_DECIMAL_PARSE_BINARY_ROP(php_decimal_rsub);
}

/**
 * Rational::mul
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, mul, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, mul)
{
    PHP_DECIMAL_PARSE_BINARY_ROP(php_decimal_rmul);
}

/**
 * Rational::div
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, div, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, div)
{
    PHP_DECIMAL_PARSE_BINARY_ROP(php_decimal_rdiv);
}

/**
 * Rational::mod
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, mod, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, mod)
{
    PHP_DECIMAL_PARSE_BINARY_ROP(php_decimal_rmod);
}

/**
 * Rational::rem
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, rem, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, rem)
{
    PHP_DECIMAL_PARSE_BINARY_ROP(php_decimal_rrem);
}

/**
 * Rational::pow
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, pow, 1)
PHP_DECIMAL_ARGINFO_ZVAL(exponent)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, pow)
{
    PHP_DECIMAL_PARSE_BINARY_ROP(php_decimal_rpow);
}

/**
 * Rational::floor
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, floor, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, floor)
{
    PHP_DECIMAL_PARSE_UNARY_ROP(php_decimal_rfloor);
}

/**
 * Rational::ceil
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, ceil, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, ceil)
{
    PHP_DECIMAL_PARSE_UNARY_ROP(php_decimal_rceil);
}

/**
 * Rational::trunc
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, trunc, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, trunc)
{
    PHP_DECIMAL_PARSE_UNARY_ROP(php_decimal_rtrunc);
}

/**
 * Rational::round
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, round, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(rounding)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, round)
{
    zend_long places = 0;
    zend_long mode   = PHP_DECIMAL_DEFAULT_ROUNDING;

    PHP_DECIMAL_PARSE_PARAMS(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRICT_LONG(places)
        Z_PARAM_STRICT_LONG(mode)
    PHP_DECIMAL_PARSE_PARAMS_END()
    {
        php_rational_t *res = php_rational();
        php_rational_t *obj = THIS_RATIONAL();

        mpd_t *num = PHP_RATIONAL_NUM(res);
        mpd_t *den = PHP_RATIONAL_DEN(res);
        mpd_t *tmp = num;

        // TODO We should be able to round the fraction inplace, no
        // need to evaluate then normalize again and then simply as well.
        // It should be a lot easier.
        php_decimal_rational_round(tmp, obj, places, mode);
        php_decimal_rational_from_mpd(num, den, tmp);
        php_decimal_rational_simplify(num, den);

        RETURN_RATIONAL(res);
    }
}

/**
 * Rational::shiftl
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, shiftl, 1)
PHP_DECIMAL_ARGINFO_ZVAL(places)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, shiftl)
{
    PHP_DECIMAL_PARSE_BINARY_ROP(php_decimal_rshiftl);
}

/**
 * Rational::shiftr
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, shiftr, 1)
PHP_DECIMAL_ARGINFO_ZVAL(places)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, shiftr)
{
    PHP_DECIMAL_PARSE_BINARY_ROP(php_decimal_rshiftr);
}

/**
 * Rational::signum
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, signum, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, signum)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_LONG(mpd_arith_sign(PHP_RATIONAL_NUM(THIS_RATIONAL())));
}

/**
 * Rational::parity
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, parity, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, parity)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_LONG(php_decimal_rational_parity(THIS_RATIONAL()));
}

/**
 * Rational::abs
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, abs, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, abs)
{
    PHP_DECIMAL_PARSE_UNARY_ROP(php_decimal_rabs);
}

/**
 * Rational::negate
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Rational, negate, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, negate)
{
    PHP_DECIMAL_PARSE_UNARY_ROP(php_decimal_rneg);
}

/**
 * Rational::isEven
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, isEven, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, isEven)
{
    php_rational_t *obj = THIS_RATIONAL();
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(php_decimal_rational_is_integer(obj) && !php_decimal_rational_parity(obj));
}


/**
 * Rational::isOdd
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, isOdd, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, isOdd)
{
    php_rational_t *obj = THIS_RATIONAL();
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(php_decimal_rational_is_integer(obj) && php_decimal_rational_parity(obj));
}


/**
 * Rational::isPositive
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, isPositive, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, isPositive)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_ispositive(PHP_RATIONAL_NUM(THIS_RATIONAL())));
}

/**
 * Rational::isNegative
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, isNegative, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, isNegative)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_isnegative(PHP_RATIONAL_NUM(THIS_RATIONAL())));
}

/**
 * Rational::isNaN
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, isNaN, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, isNaN)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_isnan(PHP_RATIONAL_NUM(THIS_RATIONAL())));
}

/**
 * Rational::isInf
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, isInf, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, isInf)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_isinfinite(PHP_RATIONAL_NUM(THIS_RATIONAL())));
}

/**
 * Rational::isInteger
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, isInteger, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, isInteger)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(php_decimal_rational_is_integer(THIS_RATIONAL()));
}

/**
 * Rational::isZero
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, isZero, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, isZero)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_iszero(PHP_RATIONAL_NUM(THIS_RATIONAL())));
}

/**
 * Rational::toFixed
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, toFixed, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_BOOL(commas)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(mode)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, toFixed)
{
    zend_long places = 0;
    zend_bool commas = false;
    zend_long mode   = PHP_DECIMAL_DEFAULT_ROUNDING;

    PHP_DECIMAL_PARSE_PARAMS(0, 3)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRICT_LONG(places)
        Z_PARAM_BOOL(commas)
        Z_PARAM_STRICT_LONG(mode)
    PHP_DECIMAL_PARSE_PARAMS_END()

    RETURN_STR(php_decimal_rational_to_fixed(THIS_RATIONAL(), places, commas, mode));
}

/**
 * Rational::__toString
 * Rational::toString
 * Rational::jsonSerialize
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, toString, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, toString)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_STR(php_decimal_rational_to_string(THIS_RATIONAL()));
}

/**
 * Rational::toInt
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, toInt, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, toInt)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_LONG(php_decimal_rational_to_long(THIS_RATIONAL()));
}

/**
 * Rational::toFloat
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, toFloat, IS_DOUBLE, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, toFloat)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_DOUBLE(php_decimal_rational_to_double(THIS_RATIONAL()));
}

/**
 * Rational::toDecimal
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(Rational, toDecimal, 1)
PHP_DECIMAL_ARGINFO_LONG(precision)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, toDecimal)
{
    zend_long prec = PHP_DECIMAL_DEFAULT_PREC;

    PHP_DECIMAL_PARSE_PARAMS(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRICT_LONG(prec)
    PHP_DECIMAL_PARSE_PARAMS_END()

    if (ZEND_NUM_ARGS() == 0 || php_decimal_validate_prec(prec)) {
        php_rational_t *obj = THIS_RATIONAL();
        php_decimal_t  *res = php_decimal_with_prec(prec);

        php_decimal_rational_evaluate(PHP_DECIMAL_MPD(res), obj, prec);
        RETURN_DECIMAL(res);
    }
}

/**
 * Rational::toRational
 */
PHP_DECIMAL_ARGINFO_RETURN_RATIONAL(Rational, toRational, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, toRational)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    ZVAL_COPY(return_value, getThis());
}

/**
 * Rational::compareTo
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Rational, compareTo, IS_LONG, 1)
PHP_DECIMAL_ARGINFO_ZVAL(other)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Rational, compareTo)
{
    zval *op2 = NULL;

    PHP_DECIMAL_PARSE_PARAMS(1, 1)
        Z_PARAM_ZVAL(op2)
    PHP_DECIMAL_PARSE_PARAMS_END()

    php_decimal_rational_compare(return_value, getThis(), op2);
}

/******************************************************************************/
/*                                 CLASS ENTRY                                */
/******************************************************************************/

static zend_function_entry rational_methods[] = {

    /* */
    PHP_DECIMAL_ME_STATIC(Rational, valueOf)

    /* */
    PHP_DECIMAL_ME_EX(Rational, __construct, ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)

    PHP_DECIMAL_ME(Rational, add)
    PHP_DECIMAL_ME(Rational, sub)
    PHP_DECIMAL_ME(Rational, mul)
    PHP_DECIMAL_ME(Rational, div)
    PHP_DECIMAL_ME(Rational, pow)
    PHP_DECIMAL_ME(Rational, rem)
    PHP_DECIMAL_ME(Rational, mod)
    PHP_DECIMAL_ME(Rational, shiftl)
    PHP_DECIMAL_ME(Rational, shiftr)

    PHP_DECIMAL_ME(Rational, floor)
    PHP_DECIMAL_ME(Rational, ceil)
    PHP_DECIMAL_ME(Rational, trunc)
    PHP_DECIMAL_ME(Rational, round)

    PHP_DECIMAL_ME(Rational, signum)
    PHP_DECIMAL_ME(Rational, parity)

    PHP_DECIMAL_ME(Rational, abs)
    PHP_DECIMAL_ME(Rational, negate)

    PHP_DECIMAL_ME(Rational, isEven)
    PHP_DECIMAL_ME(Rational, isOdd)

    PHP_DECIMAL_ME(Rational, isPositive)
    PHP_DECIMAL_ME(Rational, isNegative)

    PHP_DECIMAL_ME(Rational, isNaN)
    PHP_DECIMAL_ME(Rational, isInf)
    PHP_DECIMAL_ME(Rational, isInteger)
    PHP_DECIMAL_ME(Rational, isZero)

    PHP_DECIMAL_ME(Rational, toFixed)
    PHP_DECIMAL_ME(Rational, toString)
    PHP_DECIMAL_ME(Rational, toInt)
    PHP_DECIMAL_ME(Rational, toFloat)
    PHP_DECIMAL_ME(Rational, toDecimal)
    PHP_DECIMAL_ME(Rational, toRational)

    PHP_DECIMAL_ME(Rational, compareTo)
    PHP_FE_END
};

/**
 * Registers the class entry and constants.
 */
void php_decimal_register_rational_class()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, PHP_DECIMAL_RATIONAL_FQCN, rational_methods);
    php_decimal_rational_ce = zend_register_internal_class_ex(&ce, php_decimal_number_ce);

    /**
     *
     */
    php_decimal_rational_ce->ce_flags |= ZEND_ACC_FINAL;

    /**
     *
     */
    php_decimal_rational_ce->create_object = php_decimal_rational_create_object;
    php_decimal_rational_ce->serialize     = php_decimal_rational_serialize;
    php_decimal_rational_ce->unserialize   = php_decimal_rational_unserialize;

    /**
     *
     */
    memcpy(&php_decimal_rational_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    /**
     * No need for an offset because we're casting back and forth between
     * zend_object and php_decimal. Malloc should know the size of the block so
     * when the engine frees the zend_object, it will free the php_decimal. We
     * also don't have any class properties and the class is final.
     */
    php_decimal_rational_handlers.offset           = 0;
    php_decimal_rational_handlers.free_obj         = php_decimal_rational_free_object;
    php_decimal_rational_handlers.clone_obj        = php_decimal_rational_clone_obj;
    php_decimal_rational_handlers.cast_object      = php_decimal_rational_cast_object;
    php_decimal_rational_handlers.compare          = php_decimal_rational_compare;
    php_decimal_rational_handlers.do_operation     = php_decimal_rational_do_operation;
    php_decimal_rational_handlers.get_debug_info   = php_decimal_rational_get_debug_info;
    php_decimal_rational_handlers.read_property    = php_decimal_guarded_read_property;
    php_decimal_rational_handlers.write_property   = php_decimal_guarded_write_property;
    php_decimal_rational_handlers.has_property     = php_decimal_guarded_has_property;
    php_decimal_rational_handlers.unset_property   = php_decimal_guarded_unset_property;
}
