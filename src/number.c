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
#include <ext/json/php_json.h>
#include <mpdecimal.h>
#include "arginfo.h"
#include "context.h"
#include "limits.h"
#include "number.h"
#include "round.h"
#include "errors.h"
#include "params.h"

/**
 * Class entry.
 */
zend_class_entry *php_decimal_number_ce;


/**
 *
 */
static zend_object_handlers php_decimal_number_handlers;


/**
 *
 */
static php_decimal_success_t php_decimal_number_compare(zval *result, zval *op1, zval *op2)
{
    if (Z_IS_DECIMAL_NUMBER_P(op1)) {

        zend_call_method_with_0_params(op1, Z_OBJCE_P(op1), NULL, "isnan", result);
        if (UNEXPECTED(Z_TYPE_P(result) == IS_TRUE)) {
            ZVAL_LONG(result, 1);
            return SUCCESS;
        }

        if (Z_IS_DECIMAL_NUMBER_P(op2)) {

            zend_call_method_with_0_params(op2, Z_OBJCE_P(op2), NULL, "isnan", result);
            if (UNEXPECTED(Z_TYPE_P(result) == IS_TRUE)) {
                ZVAL_LONG(result, 1);
                return SUCCESS;
            }
        }

        if (Z_TYPE_P(op2) == IS_DOUBLE) {
            if (UNEXPECTED(zend_isnan(Z_DVAL_P(op2)))) {
                ZVAL_LONG(result, 1);
                return SUCCESS;
            }
        }

        zend_call_method_with_1_params(op1, Z_OBJCE_P(op1), NULL, "compareto", result, op2);

    } else {

        zend_call_method_with_0_params(op2, Z_OBJCE_P(op2), NULL, "isnan", result);
        if (UNEXPECTED(Z_TYPE_P(result) == IS_TRUE)) {
            ZVAL_LONG(result, 1);
            return SUCCESS;
        }

        if (Z_TYPE_P(op1) == IS_DOUBLE) {
            if (UNEXPECTED(zend_isnan(Z_DVAL_P(op1)))) {
                ZVAL_LONG(result, 1);
                return SUCCESS;
            }
        }

        zend_call_method_with_1_params(op2, Z_OBJCE_P(op2), NULL, "compareto", result, op1);
        ZVAL_LONG(result, -1 * Z_LVAL_P(result));
    }

    /* */
    if (UNEXPECTED(EG(exception))) {
        ZVAL_LONG(result, 1);
        return SUCCESS;
    }

    ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_LVAL_P(result)));
    return SUCCESS;
}

/**
 * Cast to string, int, float or bool.
 */
static php_decimal_success_t php_decimal_number_cast_object(zval *obj, zval *result, int type)
{
    switch (type) {
        case IS_STRING:
            zend_call_method_with_0_params(obj, Z_OBJCE_P(obj), NULL, "tostring", result);
            return SUCCESS;

        case IS_LONG:
            zend_call_method_with_0_params(obj, Z_OBJCE_P(obj), NULL, "toint", result);
            return SUCCESS;

        case IS_DOUBLE:
            zend_call_method_with_0_params(obj, Z_OBJCE_P(obj), NULL, "tofloat", result);
            return SUCCESS;

        case _IS_BOOL:
            ZVAL_BOOL(result, 1); /* Objects are always true */
            return SUCCESS;

        default:
            return FAILURE;
    }
}

/**
 *
 */
static const char *php_decimal_number_get_operation_function_name(zend_uchar opcode)
{
    switch (opcode) {
        case ZEND_ADD: return "add";
        case ZEND_SUB: return "sub";
        case ZEND_MUL: return "mul";
        case ZEND_DIV: return "div";
        case ZEND_MOD: return "mod";
        case ZEND_POW: return "pow";
        case ZEND_SL:  return "shiftl";
        case ZEND_SR:  return "shiftr";
        default:
            return NULL;
    }
}

/**
 *
 */
static php_decimal_success_t php_decimal_number_do_operation(zend_uchar opcode, zval *result, zval *op1, zval *op2)
{
    zval op1_copy;
    const char *func = php_decimal_number_get_operation_function_name(opcode);

    /* Unsupported operator - return success to avoid casting. */
    if (UNEXPECTED(func == NULL)) {
        php_decimal_operator_not_supported();
        return SUCCESS;
    }

    /* This allows for assign syntax, ie. $op1 /= $op2 */
    if (op1 == result) {
        ZVAL_COPY_VALUE(&op1_copy, op1);
        op1 = &op1_copy;
    }

    /* Attempt operation. */
    if (Z_IS_DECIMAL_NUMBER_P(op1)) {
        zend_call_method(op1, Z_OBJCE_P(op1), NULL, func, strlen(func), result, 1, op2, NULL);

        /* Check that nothing went wrong. */
        if (UNEXPECTED(EG(exception))) {
            ZVAL_UNDEF(result);
            return SUCCESS;
        }

    } else {
        zval tmp;
        zend_call_method_with_1_params(NULL, Z_OBJCE_P(op2), NULL, "valueof", &tmp, op1);

        /* */
        if (UNEXPECTED(EG(exception))) {
            ZVAL_UNDEF(result);
            return SUCCESS;
        }

        /* */
        zend_call_method(&tmp, Z_OBJCE_P(&tmp), NULL, func, strlen(func), result, 1, op2, NULL);
        zval_ptr_dtor(&tmp);

        /* */
        if (UNEXPECTED(EG(exception))) {
            ZVAL_UNDEF(result);
            return SUCCESS;
        }
    }

    if (op1 == &op1_copy) {
        zval_dtor(op1);
    }

    return SUCCESS;
}

/**
 * Number::valueOf
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, valueOf, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, valueOf)
{
    php_decimal_number_not_implemented();
}

/**
 * Number::add
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, add, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::sub
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, sub, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::mul
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, mul, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::div
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, div, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::mod
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, mod, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::pow
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, pow, 1)
PHP_DECIMAL_ARGINFO_ZVAL(exponent)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::shiftl
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, shiftl, 1)
PHP_DECIMAL_ARGINFO_LONG(places)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::shiftr
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, shiftr, 1)
PHP_DECIMAL_ARGINFO_LONG(places)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::floor
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, floor, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::ceil
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, ceil, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::trunc
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, trunc, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::round
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, round, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(mode)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::abs
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, abs, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::negate
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, negate, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::signum
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, signum, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::parity
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, parity, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::isNaN
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isNaN, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::isInf
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isInf, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::isInteger
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isInteger, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::isZero
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isZero, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::toFixed
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, toFixed, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_BOOL(commas)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(mode)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::toString
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, toString, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::toInt
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, toInt, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::toFloat
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, toFloat, IS_DOUBLE, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::toDecimal
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(Number, toDecimal, 1)
PHP_DECIMAL_ARGINFO_LONG(precision)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::toRational
 */
PHP_DECIMAL_ARGINFO_RETURN_RATIONAL(Number, toRational, 0)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::compareTo
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, compareTo, IS_LONG, 1)
PHP_DECIMAL_ARGINFO_ZVAL(other)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::__toString
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, __toString, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, __toString)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    zend_call_method_with_0_params(getThis(), Z_OBJCE_P(getThis()), NULL, "tostring", return_value);
}

/**
 * Number::jsonSerialize
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, jsonSerialize, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, jsonSerialize)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    zend_call_method_with_0_params(getThis(), Z_OBJCE_P(getThis()), NULL, "tostring", return_value);
}

/******************************************************************************/
/*                                 CLASS ENTRY                                */
/******************************************************************************/

static zend_function_entry number_methods[] = {

    /* Public entry-point to create new instances. */
    PHP_DECIMAL_ME_STATIC(Number, valueOf)

    PHP_DECIMAL_ME_ABSTRACT(Number, add)
    PHP_DECIMAL_ME_ABSTRACT(Number, sub)
    PHP_DECIMAL_ME_ABSTRACT(Number, mul)
    PHP_DECIMAL_ME_ABSTRACT(Number, div)
    PHP_DECIMAL_ME_ABSTRACT(Number, pow)
    PHP_DECIMAL_ME_ABSTRACT(Number, mod)
    PHP_DECIMAL_ME_ABSTRACT(Number, shiftl)
    PHP_DECIMAL_ME_ABSTRACT(Number, shiftr)

    PHP_DECIMAL_ME_ABSTRACT(Number, floor)
    PHP_DECIMAL_ME_ABSTRACT(Number, ceil)
    PHP_DECIMAL_ME_ABSTRACT(Number, trunc)
    PHP_DECIMAL_ME_ABSTRACT(Number, round)

    PHP_DECIMAL_ME_ABSTRACT(Number, abs)
    PHP_DECIMAL_ME_ABSTRACT(Number, negate)
    PHP_DECIMAL_ME_ABSTRACT(Number, signum)
    PHP_DECIMAL_ME_ABSTRACT(Number, parity)

    PHP_DECIMAL_ME_ABSTRACT(Number, isNaN)
    PHP_DECIMAL_ME_ABSTRACT(Number, isInf)
    PHP_DECIMAL_ME_ABSTRACT(Number, isInteger)
    PHP_DECIMAL_ME_ABSTRACT(Number, isZero)

    PHP_DECIMAL_ME_ABSTRACT(Number, toFixed)
    PHP_DECIMAL_ME_ABSTRACT(Number, toString)
    PHP_DECIMAL_ME_ABSTRACT(Number, toInt)
    PHP_DECIMAL_ME_ABSTRACT(Number, toFloat)
    PHP_DECIMAL_ME_ABSTRACT(Number, toDecimal)
    PHP_DECIMAL_ME_ABSTRACT(Number, toRational)

    PHP_DECIMAL_ME_ABSTRACT(Number, compareTo)

    /* Aliases */
    PHP_DECIMAL_ME(Number, __toString)
    PHP_DECIMAL_ME(Number, jsonSerialize)
    PHP_FE_END
};

/**
 *
 */
static zend_object *php_decimal_number_create_object(zend_class_entry *ce)
{
    /* */
    zend_object *obj = zend_objects_new(ce);
    object_properties_init(obj, ce);

    /* */
    obj->handlers = &php_decimal_number_handlers;

    return obj;
}

void php_decimal_register_number_class()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, PHP_DECIMAL_NUMBER_FQCN, number_methods);
    php_decimal_number_ce = zend_register_internal_class(&ce);

    /**
     *
     */
    zend_class_implements(php_decimal_number_ce, 1, php_json_serializable_ce);

    /**
     *
     */
    php_decimal_number_ce->create_object = php_decimal_number_create_object;

    /**
     *
     */
    memcpy(&php_decimal_number_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_decimal_number_handlers.do_operation = php_decimal_number_do_operation;
    php_decimal_number_handlers.compare      = php_decimal_number_compare;
    php_decimal_number_handlers.cast_object  = php_decimal_number_cast_object;

    /**
     * Rounding
     */
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "DEFAULT_ROUNDING",  PHP_DECIMAL_DEFAULT_ROUNDING);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "ROUND_UP",          PHP_DECIMAL_ROUND_UP);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "ROUND_DOWN",        PHP_DECIMAL_ROUND_DOWN);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "ROUND_CEILING",     PHP_DECIMAL_ROUND_CEILING);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "ROUND_FLOOR",       PHP_DECIMAL_ROUND_FLOOR);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "ROUND_HALF_UP",     PHP_DECIMAL_ROUND_HALF_UP);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "ROUND_HALF_DOWN",   PHP_DECIMAL_ROUND_HALF_DOWN);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "ROUND_HALF_EVEN",   PHP_DECIMAL_ROUND_HALF_EVEN);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "ROUND_HALF_ODD",    PHP_DECIMAL_ROUND_HALF_ODD);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "ROUND_TRUNCATE",    PHP_DECIMAL_ROUND_TRUNCATE);

    /**
     * Precision
     */
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "DEFAULT_PRECISION", PHP_DECIMAL_DEFAULT_PREC);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "MIN_PRECISION",     PHP_DECIMAL_MIN_PREC);
    PHP_DECIMAL_LONG_CONSTANT(php_decimal_number_ce, "MAX_PRECISION",     PHP_DECIMAL_MAX_PREC);
}
