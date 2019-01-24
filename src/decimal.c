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
#include <ext/standard/php_math.h>
#include <ext/standard/php_var.h>
#include <ext/standard/php_string.h>
#include <mpdecimal.h>
#include "bool.h"
#include "arginfo.h"
#include "common.h"
#include "compare.h"
#include "context.h"
#include "convert.h"
#include "decimal.h"
#include "errors.h"
#include "math.h"
#include "number.h"
#include "params.h"
#include "parse.h"
#include "rational.h"
#include "round.h"

/**
 * Class entry.
 */
zend_class_entry *php_decimal_decimal_ce;

/**
 * Object handlers.
 */
static zend_object_handlers php_decimal_handlers;


/******************************************************************************/
/*                         ALLOC, INIT, FREE, CLONE                           */
/******************************************************************************/

/**
 * Allocates a new php_decimal_t that has not been initialized. We don't want to
 * initialize a decimal until its constructor is called, so that we can check in
 * the constructor whether the object has already been initialized.
 */
static php_decimal_t *php_decimal_alloc()
{
    php_decimal_t *obj = ecalloc(1, sizeof(php_decimal_t));

    // TODO we should still go through everything to check for branch prediction.
    if (obj) {
        obj->std.handlers = &php_decimal_handlers;
        zend_object_std_init((zend_object *) obj, php_decimal_decimal_ce);
    } else {
        php_decimal_memory_error();
    }

    return obj;
}

/**
 * Creates a new decimal object, initialized to a given precision.
 */
php_decimal_t *php_decimal_with_prec(zend_long prec)
{
    php_decimal_t *obj = php_decimal_alloc();

    php_decimal_init_mpd(PHP_DECIMAL_MPD(obj));
    php_decimal_set_prec(obj, prec);

    return obj;
}

/**
 * Creates a new decimal object, initialized to the default precision.
 */
php_decimal_t *php_decimal()
{
    return php_decimal_with_prec(PHP_DECIMAL_DEFAULT_PREC);
}

/**
 * Creates a custom zend_object that is also an uninitialized decimal object.
 */
static zend_object *php_decimal_create_object(zend_class_entry *ce)
{
    return (zend_object *) php_decimal_alloc();
}

/**
 * Creates a copy of the given decimal object.
 */
php_decimal_t *php_decimal_create_copy(const php_decimal_t *src)
{
    php_decimal_t *dst = php_decimal_with_prec(php_decimal_get_prec(src));

    mpd_copy(PHP_DECIMAL_MPD(dst), PHP_DECIMAL_MPD(src), MAX_CONTEXT);

    return dst;
}

/**
 * Clones the given zval, which must be a decimal object.
 */
static zend_object *php_decimal_clone_obj(zval *obj)
{
    return (zend_object *) php_decimal_create_copy(Z_DECIMAL_P(obj));
}

/**
 * Free handler, should only free internal memory, not the object itself.
 */
static void php_decimal_free_object(zend_object *obj)
{
    php_decimal_release_mpd(PHP_DECIMAL_MPD((php_decimal_t*) obj));

    zend_object_std_dtor(obj);
}

/**
 * Converts a zend opcode to a binary arithmetic function pointer.
 *
 * Returns NULL if a function is not mapped.
 */
static php_decimal_binary_op_t php_decimal_get_binary_op(zend_uchar opcode)
{
    switch (opcode) {
        case ZEND_ADD: return php_decimal_add;
        case ZEND_SUB: return php_decimal_sub;
        case ZEND_MUL: return php_decimal_mul;
        case ZEND_DIV: return php_decimal_div;
        case ZEND_MOD: return php_decimal_mod;
        case ZEND_POW: return php_decimal_pow;
        case ZEND_SL:  return php_decimal_shiftl;
        case ZEND_SR:  return php_decimal_shiftr;
        default:
            return NULL;
    }
}

/**
 * Attempts a binary operation on two zval's, writing the result to res.
 *
 * We don't know which of the operands is a decimal, if not both.
 */
static void php_decimal_do_binary_op(php_decimal_binary_op_t op, php_decimal_t *res, zval *op1, zval *op2)
{
    mpd_t *mpd1;
    mpd_t *mpd2;

    zend_long prec;
    PHP_DECIMAL_TEMP_MPD(tmp);

    if (Z_IS_DECIMAL_P(op1)) {
        if (Z_IS_DECIMAL_P(op2)) {
            mpd1 = Z_MPD_P(op1);
            mpd2 = Z_MPD_P(op2);
            prec = MIN(php_decimal_get_prec(Z_DECIMAL_P(op1)),
                       php_decimal_get_prec(Z_DECIMAL_P(op2)));

        } else {
            mpd1 = Z_MPD_P(op1);
            mpd2 = &tmp;
            prec = php_decimal_get_prec(Z_DECIMAL_P(op1));

            if (php_decimal_parse_mpd(mpd2, op2, prec + 2) == FAILURE) {
                goto failure;
            }
        }
    } else {
        mpd1 = &tmp;
        mpd2 = Z_MPD_P(op2);
        prec = php_decimal_get_prec(Z_DECIMAL_P(op2));

        if (php_decimal_parse_mpd(mpd1, op1, prec + 2) == FAILURE) {
            goto failure;
        }
    }

    /* Parsed successfully, so we can set the parsed precision and do the op. */
    php_decimal_set_prec(res, prec);
    op(PHP_DECIMAL_MPD(res), mpd1, mpd2, prec);
    mpd_del(&tmp);
    return;

failure:
    php_decimal_set_nan(PHP_DECIMAL_MPD(res));
    mpd_del(&tmp);
}


/******************************************************************************/
/*                               SERIALIZATION                                */
/******************************************************************************/

/**
 * Serialize
 */
static php_decimal_success_t php_decimal_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    zval tmp;
    smart_str buf = {0};
    php_decimal_t *obj = Z_DECIMAL_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    /* Serialize the internal value as a string. */
    ZVAL_STR(&tmp, php_decimal_to_string(PHP_DECIMAL_MPD(obj)));
    php_var_serialize(&buf, &tmp, &serialize_data);
    zval_ptr_dtor(&tmp);

    /* Serialize the precision as an integer. */
    ZVAL_LONG(&tmp, php_decimal_get_prec(obj));
    php_var_serialize(&buf, &tmp, &serialize_data);

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);

    *buffer = (unsigned char *) estrndup(ZSTR_VAL(buf.s), ZSTR_LEN(buf.s));
    *length = ZSTR_LEN(buf.s);

    smart_str_free(&buf);

    return SUCCESS;
}

/**
 * Unserialize
 */
static php_decimal_success_t php_decimal_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    zval *value;
    zval *prec;

    php_decimal_t *res = php_decimal();

    ZVAL_DECIMAL(object, res);

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *end = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    /* Unserialize internal decimal value, which was serialized as a string. */
    value = var_tmp_var(&unserialize_data);
    if (!php_var_unserialize(value, &pos, end, &unserialize_data) || Z_TYPE_P(value) != IS_STRING) {
        goto error;
    }

    /* Unserialize precision, which was serialized as an integer. */
    prec = var_tmp_var(&unserialize_data);
    if (!php_var_unserialize(prec, &pos, end, &unserialize_data) || Z_TYPE_P(prec) != IS_LONG) {
        goto error;
    }

    /* Check that we've parsed the entire serialized string. */
    if (pos != end) {
        goto error;
    }

    /* Check precision is valid. */
    if (!php_decimal_validate_prec(Z_LVAL_P(prec))) {
        goto error;
    }

    /* Set the precision. */
    php_decimal_set_prec(res, Z_LVAL_P(prec));

    /* Attempt to parse the unserialized string, quietly, delegate to local error. */
    if (php_decimal_set_string(PHP_DECIMAL_MPD(res), Z_STR_P(value)) == FAILURE) {
        goto error;
    }

    /* Success! */
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    zval_ptr_dtor(object);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    php_decimal_unserialize_error();
    return FAILURE;
}


/******************************************************************************/
/*                              OBJECT HANDLERS                               */
/******************************************************************************/

/**
 * Compares two zval's, one of which must be a decimal. This is the function
 * used by the compare handler, as well as compareTo.
 */
static php_decimal_success_t php_decimal_compare(zval *res, zval *op1, zval *op2)
{
    int result;
    int invert;

    if (Z_IS_DECIMAL_P(op1)) {
        result = php_decimal_compare_decimal_to_zval(Z_DECIMAL_P(op1), op2);
        invert = 0;
    } else {
        result = php_decimal_compare_decimal_to_zval(Z_DECIMAL_P(op2), op1);
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
static HashTable *php_decimal_get_debug_info(zval *obj, int *is_temp)
{
    zval tmp;
    HashTable *debug_info;

    ALLOC_HASHTABLE(debug_info);
    zend_hash_init(debug_info, 2, NULL, ZVAL_PTR_DTOR, 0);

    ZVAL_STR(&tmp, php_decimal_to_string(Z_MPD_P(obj)));
    zend_hash_str_update(debug_info, "value", sizeof("value") - 1, &tmp);

    ZVAL_LONG(&tmp, php_decimal_get_prec(Z_DECIMAL_P(obj)));
    zend_hash_str_update(debug_info, "precision", sizeof("precision") - 1, &tmp);

    *is_temp = 1;

    return debug_info;
}

/**
 * Cast to string, int, float or bool.
 */
static php_decimal_success_t php_decimal_cast_object(zval *obj, zval *result, int type)
{
    switch (type) {
        case IS_STRING:
            ZVAL_STR(result, php_decimal_to_string(Z_MPD_P(obj)));
            return SUCCESS;

        case IS_LONG:
            ZVAL_LONG(result, php_decimal_to_long(Z_MPD_P(obj)));
            return SUCCESS;

        case IS_DOUBLE:
            ZVAL_DOUBLE(result, php_decimal_to_double(Z_MPD_P(obj)));
            return SUCCESS;

        case _IS_BOOL:
            ZVAL_BOOL(result, 1); /* Objects are always true */
            return SUCCESS;

        default:
            return FAILURE;
    }
}

/**
 * Operator overloading.
 *
 * We don't know which of op1 and op2 is a decimal object (if not both).
 */
static php_decimal_success_t php_decimal_do_operation(zend_uchar opcode, zval *result, zval *op1, zval *op2)
{
    zval op1_copy;
    php_decimal_binary_op_t op = php_decimal_get_binary_op(opcode);

    /* Unsupported op type - return success to avoid casting. */
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
    ZVAL_DECIMAL(result, php_decimal());
    php_decimal_do_binary_op(op, Z_DECIMAL_P(result), op1, op2);

    /* Operation failed - return success to avoid casting. */
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
/*                            PARAMETER PARSING                               */
/******************************************************************************/

/**
 * Parse a decimal binary operation (op1 OP op2).
 */
#define PHP_DECIMAL_PARSE_BINARY_OP(op) do { \
    php_decimal_t *res = php_decimal(); \
    zval          *op2 = NULL; \
    \
    PHP_DECIMAL_PARSE_PARAMS(1, 1) \
        Z_PARAM_ZVAL(op2) \
    PHP_DECIMAL_PARSE_PARAMS_END() \
    php_decimal_do_binary_op(op, res, getThis(), op2); \
    RETURN_DECIMAL(res); \
} while (0)

/**
 * Parse a decimal unary operation (OP op1).
 */
#define PHP_DECIMAL_PARSE_UNARY_OP(op) do { \
    php_decimal_t *obj = THIS_DECIMAL(); \
    php_decimal_t *res = php_decimal_with_prec(php_decimal_get_prec(obj)); \
    \
    PHP_DECIMAL_PARSE_PARAMS_NONE(); \
    op(PHP_DECIMAL_MPD(res), PHP_DECIMAL_MPD(obj), php_decimal_get_prec(res)); \
    RETURN_DECIMAL(res); \
} while(0)


/******************************************************************************/
/*                              PHP CLASS METHODS                             */
/******************************************************************************/

/**
 * Decimal::valueOf
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, valueOf, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(precision)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, valueOf)
{
    zval      *val = NULL;
    zend_long prec = PHP_DECIMAL_DEFAULT_PREC;

    PHP_DECIMAL_PARSE_PARAMS(1, 2)
        Z_PARAM_ZVAL(val)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(prec)
    PHP_DECIMAL_PARSE_PARAMS_END()

    if (ZEND_NUM_ARGS() == 1) {
        php_decimal_parse_decimal(return_value, val, prec, true);

    } else if (EXPECTED(php_decimal_validate_prec(prec))) {
        php_decimal_parse_decimal(return_value, val, prec, false);
    }
}

/**
 * Decimal::__construct
 */
PHP_DECIMAL_ARGINFO(Decimal, __construct, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, __construct)
{
    /* Does nothing, is private. */
}

/**
 * Decimal::add
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, add, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, add)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_add);
}

/**
 * Decimal::sub
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, sub, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, sub)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_sub);
}

/**
 * Decimal::mul
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, mul, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, mul)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_mul);
}

/**
 * Decimal::div
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, div, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, div)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_div);
}

/**
 * Decimal::mod
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, mod, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, mod)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_mod);
}

/**
 * Decimal::pow
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, pow, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, pow)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_pow);
}

/**
 * Decimal::rem
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, rem, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, rem)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_rem);
}

/**
 * Decimal::ln
 * Decimal::log
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, ln, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, ln)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_ln);
}

/**
 * Decimal::exp
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, exp, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, exp)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_exp);
}

/**
 * Decimal::log10
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, log10, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, log10)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_log10);
}

/**
 * Decimal::sqrt
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, sqrt, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, sqrt)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_sqrt);
}

/**
 * Decimal::round
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, round, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(mode)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, round)
{
    zend_long places = 0;
    zend_long mode   = PHP_DECIMAL_DEFAULT_ROUNDING;

    PHP_DECIMAL_PARSE_PARAMS(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(places)
        Z_PARAM_LONG(mode)
    PHP_DECIMAL_PARSE_PARAMS_END()
    {
        php_decimal_t *obj = THIS_DECIMAL();
        php_decimal_t *res = php_decimal_with_prec(php_decimal_get_prec(obj));

        ZVAL_DECIMAL(return_value, res);

        if (php_decimal_round(PHP_DECIMAL_MPD(res), PHP_DECIMAL_MPD(obj), places, mode) == FAILURE) {
            zval_ptr_dtor(return_value);
            ZVAL_UNDEF(return_value);
        }
    }
}

/**
 * Decimal::floor
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, floor, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, floor)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_floor);
}

/**
 * Decimal::ceil
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, ceil, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, ceil)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_ceil);
}

/**
 * Decimal::trunc
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, trunc, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, trunc)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_trunc);
}

/**
 * Decimal::shiftl
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, shiftl, 1)
PHP_DECIMAL_ARGINFO_ZVAL(places)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, shiftl)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_shiftl);
}

/**
 * Decimal::shiftr
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, shiftr, 1)
PHP_DECIMAL_ARGINFO_ZVAL(places)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, shiftr)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_shiftr);
}

/**
 * Decimal::trim
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, trim, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, trim)
{
    uint32_t status = 0;

    php_decimal_t *obj = THIS_DECIMAL();
    php_decimal_t *res = php_decimal_create_copy(obj);

    PHP_DECIMAL_PARSE_PARAMS_NONE();

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_prec(obj), {
        mpd_qreduce(PHP_DECIMAL_MPD(res), PHP_DECIMAL_MPD(res), SHARED_CONTEXT, &status);
    });

    RETURN_DECIMAL(res);
}

/**
 * Decimal::reduce
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, reduce, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, reduce)
{
    uint32_t status = 0;

    php_decimal_t *obj = THIS_DECIMAL();
    php_decimal_t *res = php_decimal_create_copy(obj);

    PHP_DECIMAL_PARSE_PARAMS_NONE();

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_prec(obj), {
        mpd_qfinalize(PHP_DECIMAL_MPD(res), SHARED_CONTEXT, &status);
    });

    RETURN_DECIMAL(res);
}

/**
 * Decimal::precision
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, precision, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, precision)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_LONG(php_decimal_get_prec(THIS_DECIMAL()));
}

/**
 * Decimal::signum
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, signum, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, signum)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_LONG(php_decimal_signum(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::parity
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, parity, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, parity)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();

    if (UNEXPECTED(mpd_isspecial(THIS_DECIMAL_MPD()))) {
        RETURN_LONG(1);

    } else {
        PHP_DECIMAL_TEMP_MPD(tmp);
        mpd_trunc(&tmp, THIS_DECIMAL_MPD(), SHARED_CONTEXT);
        RETVAL_LONG(mpd_isodd(&tmp));
        mpd_del(&tmp);
    }
}

/**
 * Decimal::abs
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, abs, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, abs)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_abs);
}

/**
 * Decimal::negate
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Decimal, negate, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, negate)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_neg);
}

/**
 * Decimal::isEven
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, isEven, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, isEven)
{
    mpd_t *mpd = THIS_DECIMAL_MPD();
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_isinteger(mpd) && !mpd_isodd(mpd));
}


/**
 * Decimal::isOdd
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, isOdd, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, isOdd)
{
    mpd_t *mpd = THIS_DECIMAL_MPD();
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_isinteger(mpd) && mpd_isodd(mpd));
}


/**
 * Decimal::isPositive
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, isPositive, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, isPositive)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_ispositive(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::isNegative
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, isNegative, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, isNegative)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_isnegative(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::isNaN
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, isNaN, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, isNaN)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_isnan(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::isInf
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, isInf, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, isInf)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_isinfinite(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::isInteger
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, isInteger, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, isInteger)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_isinteger(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::isZero
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, isZero, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, isZero)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(mpd_iszero(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::toFixed
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, toFixed, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_BOOL(commas)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(mode)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, toFixed)
{
    zend_long places = 0;
    zend_bool commas = false;
    zend_long mode   = PHP_DECIMAL_DEFAULT_ROUNDING;

    PHP_DECIMAL_PARSE_PARAMS(0, 3)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(places)
        Z_PARAM_BOOL(commas)
        Z_PARAM_LONG(mode)
    PHP_DECIMAL_PARSE_PARAMS_END()

    RETURN_STR(php_decimal_to_fixed(THIS_DECIMAL_MPD(), places, commas, mode));
}

/**
 * Decimal::__toString
 * Decimal::toString
 * Decimal::jsonSerialize
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, toString, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, toString)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_STR(php_decimal_to_string(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::toInt
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, toInt, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, toInt)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_LONG(php_decimal_to_long(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::toFloat
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, toFloat, IS_DOUBLE, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, toFloat)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_DOUBLE(php_decimal_to_double(THIS_DECIMAL_MPD()));
}

/**
 * Decimal::toDecimal
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(Decimal, toDecimal, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(precision)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, toDecimal)
{
    zend_long prec;

    PHP_DECIMAL_PARSE_PARAMS(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(prec)
    PHP_DECIMAL_PARSE_PARAMS_END()

    ZVAL_COPY(return_value, getThis());

    /* */
    if (ZEND_NUM_ARGS() == 1 && php_decimal_validate_prec(prec)) {
        php_decimal_set_prec(Z_DECIMAL_P(return_value), prec);
    }
}

/**
 * Decimal::toRational
 */
PHP_DECIMAL_ARGINFO_RETURN_RATIONAL(Decimal, toRational, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, toRational)
{
    php_rational_t *res = php_rational();

    mpd_t *num = PHP_RATIONAL_NUM(res);
    mpd_t *den = PHP_RATIONAL_DEN(res);

    PHP_DECIMAL_PARSE_PARAMS_NONE();

    php_decimal_rational_from_mpd(num, den, THIS_DECIMAL_MPD());
    php_decimal_rational_simplify(num, den);

    RETURN_RATIONAL(res);
}

/**
 * Decimal::compareTo
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Decimal, compareTo, IS_LONG, 1)
PHP_DECIMAL_ARGINFO_ZVAL(other)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Decimal, compareTo)
{
    zval *op2 = NULL;

    PHP_DECIMAL_PARSE_PARAMS(1, 1)
        Z_PARAM_ZVAL(op2)
    PHP_DECIMAL_PARSE_PARAMS_END()

    /* */
    if (php_decimal_compare(return_value, getThis(), op2) == FAILURE) {
        compare_function(return_value, getThis(), op2);
    }
}


/******************************************************************************/
/*                                 CLASS ENTRY                                */
/******************************************************************************/

static zend_function_entry decimal_methods[] = {

    /* */
    PHP_DECIMAL_ME_STATIC(Decimal, valueOf)

    /* */
    PHP_DECIMAL_ME_EX(Decimal, __construct, ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)

    PHP_DECIMAL_ME(Decimal, add)
    PHP_DECIMAL_ME(Decimal, sub)
    PHP_DECIMAL_ME(Decimal, mul)
    PHP_DECIMAL_ME(Decimal, div)
    PHP_DECIMAL_ME(Decimal, rem)
    PHP_DECIMAL_ME(Decimal, mod)
    PHP_DECIMAL_ME(Decimal, pow)
    PHP_DECIMAL_ME(Decimal, shiftl)
    PHP_DECIMAL_ME(Decimal, shiftr)

    PHP_DECIMAL_ME(Decimal, ln)
    PHP_DECIMAL_ME(Decimal, exp)
    PHP_DECIMAL_ME(Decimal, log10)
    PHP_DECIMAL_ME(Decimal, sqrt)

    PHP_DECIMAL_ME(Decimal, floor)
    PHP_DECIMAL_ME(Decimal, ceil)
    PHP_DECIMAL_ME(Decimal, trunc)
    PHP_DECIMAL_ME(Decimal, round)
    PHP_DECIMAL_ME(Decimal, trim)
    PHP_DECIMAL_ME(Decimal, reduce)

    PHP_DECIMAL_ME(Decimal, signum)
    PHP_DECIMAL_ME(Decimal, parity)
    PHP_DECIMAL_ME(Decimal, precision)

    PHP_DECIMAL_ME(Decimal, abs)
    PHP_DECIMAL_ME(Decimal, negate)

    PHP_DECIMAL_ME(Decimal, isEven)
    PHP_DECIMAL_ME(Decimal, isOdd)

    PHP_DECIMAL_ME(Decimal, isPositive)
    PHP_DECIMAL_ME(Decimal, isNegative)

    PHP_DECIMAL_ME(Decimal, isNaN)
    PHP_DECIMAL_ME(Decimal, isInf)
    PHP_DECIMAL_ME(Decimal, isInteger)
    PHP_DECIMAL_ME(Decimal, isZero)

    PHP_DECIMAL_ME(Decimal, toFixed)
    PHP_DECIMAL_ME(Decimal, toString)
    PHP_DECIMAL_ME(Decimal, toInt)
    PHP_DECIMAL_ME(Decimal, toFloat)
    PHP_DECIMAL_ME(Decimal, toDecimal)
    PHP_DECIMAL_ME(Decimal, toRational)

    // PHP_DECIMAL_ME(Decimal, equals)
    PHP_DECIMAL_ME(Decimal, compareTo)
    PHP_FE_END
};

/**
 * Registers the class entry and constants.
 */
void php_decimal_register_decimal_class()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, PHP_DECIMAL_DECIMAL_FQCN, decimal_methods);
    php_decimal_decimal_ce = zend_register_internal_class_ex(&ce, php_decimal_number_ce);

    /**
     *
     */
    php_decimal_decimal_ce->ce_flags |= ZEND_ACC_FINAL;

    /**
     *
     */
    php_decimal_decimal_ce->create_object = php_decimal_create_object;
    php_decimal_decimal_ce->serialize     = php_decimal_serialize;
    php_decimal_decimal_ce->unserialize   = php_decimal_unserialize;

    /**
     *
     */
    memcpy(&php_decimal_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    /**
     * No need for an offset because we're casting back and forth between
     * zend_object and php_decimal. Malloc should know the size of the block so
     * when the engine frees the zend_object, it will free the php_decimal. We
     * also don't have any class properties and the class is final.
     */
    php_decimal_handlers.offset           = 0;
    php_decimal_handlers.free_obj         = php_decimal_free_object;
    php_decimal_handlers.clone_obj        = php_decimal_clone_obj;
    php_decimal_handlers.cast_object      = php_decimal_cast_object;
    php_decimal_handlers.compare          = php_decimal_compare;
    php_decimal_handlers.do_operation     = php_decimal_do_operation;
    php_decimal_handlers.get_debug_info   = php_decimal_get_debug_info;
    php_decimal_handlers.read_property    = php_decimal_guarded_read_property;
    php_decimal_handlers.write_property   = php_decimal_guarded_write_property;
    php_decimal_handlers.has_property     = php_decimal_guarded_has_property;
    php_decimal_handlers.unset_property   = php_decimal_guarded_unset_property;
}
