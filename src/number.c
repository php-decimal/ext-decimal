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
#include "compare.h"
#include "context.h"
#include "convert.h"
#include "errors.h"
#include "limits.h"
#include "math.h"
#include "number.h"
#include "params.h"
#include "parse.h"
#include "round.h"

zend_class_entry *php_decimal_number_ce;


/***************************************PHP_DECIMAL_EX***************************************/
/*                              OBJECT HANDLERS                               */
/******************************************************************************/

/**
 *
 */
static zend_object_handlers php_decimal_number_handlers;

/**
 * Cast to string, int, float or bool.
 */
static php_decimal_success_t php_decimal_number_cast_object(zval *obj, zval *result, int type)
{
    switch (type) {
        case IS_STRING:
            php_decimal_number_to_string(result, obj);
            return SUCCESS;

        case IS_LONG:
            ZVAL_LONG(result, php_decimal_number_to_long(obj));
            return SUCCESS;

        case IS_DOUBLE:
            ZVAL_DOUBLE(result, php_decimal_number_to_double(obj));
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

    /* */
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
 * Compares two zval's, one of which must be a decimal. This is the function
 * used by the compare handler, as well as compareTo.
 */
static php_decimal_success_t php_decimal_number_compare_handler(zval *res, zval *op1, zval *op2)
{
    int result;
    int invert;

    if (Z_IS_DECIMAL_NUMBER_P(op1)) {
        result = php_decimal_number_compare(op1, op2);
        invert = 0;
    } else {
        result = php_decimal_number_compare(op2, op1);
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


/******************************************************************************/
/*                              PHP CLASS METHODS                             */
/******************************************************************************/

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
PHP_DECIMAL_ARGINFO_ZVAL(places)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::shiftr
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, shiftr, 1)
PHP_DECIMAL_ARGINFO_ZVAL(places)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::round
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, round, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(mode)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::floor
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, floor, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, floor)
{
    zval places;
    zval mode;

    zval *obj = getThis();

    PHP_DECIMAL_PARSE_PARAMS_NONE();
    
    ZVAL_LONG(&places, 0);
    ZVAL_LONG(&mode, PHP_DECIMAL_ROUND_FLOOR);

    zend_call_method_with_2_params(obj, Z_OBJCE_P(obj), NULL, "round", return_value, &places, &mode);
}

/**
 * Number::ceil
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, ceil, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, ceil)
{
    zval places;
    zval mode;

    zval *obj = getThis();
    
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    
    ZVAL_LONG(&places, 0);
    ZVAL_LONG(&mode, PHP_DECIMAL_ROUND_CEILING);
 
    zend_call_method_with_2_params(obj, Z_OBJCE_P(obj), NULL, "round", return_value, &places, &mode);
}

/**
 * Number::trunc
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, trunc, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, trunc)
{
    zval places;
    zval mode;

    zval *obj = getThis();
    
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    
    ZVAL_LONG(&places, 0);
    ZVAL_LONG(&mode, PHP_DECIMAL_ROUND_TRUNCATE);
 
    zend_call_method_with_2_params(obj, Z_OBJCE_P(obj), NULL, "round", return_value, &places, &mode);
}

/**
 * Number::abs
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, abs, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, abs)
{
    zval *obj = getThis();

    PHP_DECIMAL_PARSE_PARAMS_NONE();

    /* */
    if (php_decimal_number_is_negative(obj)) {
        zend_call_method_with_0_params(obj, Z_OBJCE_P(obj), NULL, "negate", return_value);
    } else {
        ZVAL_COPY(return_value, obj);
    }
}

/**
 * Number::negate
 */
PHP_DECIMAL_ARGINFO_RETURN_NUMBER(Number, negate, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, negate)
{
    zval negative_one;

    zval *obj = getThis();

    PHP_DECIMAL_PARSE_PARAMS_NONE();

    ZVAL_LONG(&negative_one, -1);
    zend_call_method_with_1_params(obj, Z_OBJCE_P(obj), NULL, "mul", return_value, &negative_one);
}

/**
 * Number::isNaN
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isNaN, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, isNaN)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(php_decimal_number_is_nan(getThis()));
}

/**
 * Number::isInf
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isInf, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, isInf)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(php_decimal_number_is_inf(getThis()));
}

/**
 * Number::isInteger
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isInteger, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, isInteger)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();   
    RETURN_BOOL(php_decimal_number_is_integer(getThis()));
}

/**
 * Number::isZero
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isZero, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, isZero)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(php_decimal_number_is_zero(getThis())); 
}

/**
 * Number::isEven
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isEven, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, isEven)
{
    const zval *obj = getThis();

    PHP_DECIMAL_PARSE_PARAMS_NONE();

    RETURN_BOOL(
        php_decimal_number_is_integer(obj) &&
        php_decimal_number_parity(obj) == 0
    );
}

/**
 * Number::isOdd
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isOdd, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, isOdd)
{
    const zval *obj = getThis();

    PHP_DECIMAL_PARSE_PARAMS_NONE();

    RETURN_BOOL(
        php_decimal_number_is_integer(obj) &&
        php_decimal_number_parity(obj) == 1
    );
}


/**
 * Number::isPositive
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isPositive, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, isPositive)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(php_decimal_number_is_positive(getThis()));
}


/**
 * Number::isNegative
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, isNegative, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, isNegative)
{
    PHP_DECIMAL_PARSE_PARAMS_NONE();
    RETURN_BOOL(php_decimal_number_is_negative(getThis()));
}

/**
 * Number::toFixed
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, toFixed, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_BOOL(commas)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(mode)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::ToSci
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, toSci, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_END()
 */

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
PHP_DECIMAL_METHOD(Number, toDecimal)
{
    zval str;
    zend_long prec;

    PHP_DECIMAL_PARSE_PARAMS(1, 1)
        Z_PARAM_STRICT_LONG(prec)
    PHP_DECIMAL_PARSE_PARAMS_END()
    {
        if (EXPECTED(php_decimal_validate_prec(prec))) {
            php_decimal_number_to_string(&str, getThis());
            php_decimal_parse_decimal(return_value, &str, prec, false);
            zval_ptr_dtor(&str);
        }
    }
}

/**
 * Number::toRational
 */
PHP_DECIMAL_ARGINFO_RETURN_RATIONAL(Number, toRational, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, toRational)
{
    zval str;

    PHP_DECIMAL_PARSE_PARAMS_NONE();
 
    php_decimal_number_to_string(&str, getThis());
    php_decimal_parse_rational(return_value, &str);
    zval_ptr_dtor(&str);
}

/**
 * Number::compareTo
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, compareTo, IS_LONG, 1)
PHP_DECIMAL_ARGINFO_ZVAL(other)
PHP_DECIMAL_ARGINFO_END()

/**
 * Number::between
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, between, _IS_BOOL, 2)
PHP_DECIMAL_ARGINFO_ZVAL(a)
PHP_DECIMAL_ARGINFO_ZVAL(b)
PHP_DECIMAL_ARGINFO_OPTIONAL_BOOL(inclusive)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, between)
{
    zval *a, *b;
    zend_bool inclusive = PHP_DECIMAL_COMPARE_BETWEEN_INCLUSIVE_DEFAULT;

    PHP_DECIMAL_PARSE_PARAMS(2, 3)
        Z_PARAM_ZVAL(a)
        Z_PARAM_ZVAL(b)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(inclusive)
    PHP_DECIMAL_PARSE_PARAMS_END()

    ZVAL_BOOL(return_value, php_decimal_number_between(getThis(), a, b, inclusive));
    zval_ptr_dtor(a);
    zval_ptr_dtor(b);
}

/**
 * Number::equals
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(Number, equals, _IS_BOOL, 1)
PHP_DECIMAL_ARGINFO_ZVAL(other)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(Number, equals)
{
    zval *other;

    PHP_DECIMAL_PARSE_PARAMS(1, 1)
        Z_PARAM_ZVAL(other)
    PHP_DECIMAL_PARSE_PARAMS_END()

    ZVAL_BOOL(return_value, php_decimal_number_equals(getThis(), other));
    zval_ptr_dtor(other);
}

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

    PHP_DECIMAL_ME_ABSTRACT(Number, round)
    PHP_DECIMAL_ME         (Number, floor)
    PHP_DECIMAL_ME         (Number, ceil)
    PHP_DECIMAL_ME         (Number, trunc)

    PHP_DECIMAL_ME         (Number, abs)
    PHP_DECIMAL_ME         (Number, negate)

    PHP_DECIMAL_ME         (Number, isNaN)
    PHP_DECIMAL_ME         (Number, isInf)
    PHP_DECIMAL_ME         (Number, isInteger)
    PHP_DECIMAL_ME         (Number, isZero)

    PHP_DECIMAL_ME         (Number, isEven)
    PHP_DECIMAL_ME         (Number, isOdd)

    PHP_DECIMAL_ME         (Number, isPositive)
    PHP_DECIMAL_ME         (Number, isNegative)

    PHP_DECIMAL_ME_ABSTRACT(Number, toString)
    PHP_DECIMAL_ME_ABSTRACT(Number, toFixed)
    PHP_DECIMAL_ME_ABSTRACT(Number, toInt)
    PHP_DECIMAL_ME_ABSTRACT(Number, toFloat)
    PHP_DECIMAL_ME         (Number, toDecimal)
    PHP_DECIMAL_ME         (Number, toRational)

    PHP_DECIMAL_ME_ABSTRACT(Number, compareTo)
    PHP_DECIMAL_ME         (Number, between)
    PHP_DECIMAL_ME         (Number, equals)
 
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

/**
 *
 */
static void php_decimal_register_scalar_constants(zend_class_entry *ce)
{
    /**
     * Rounding
     */
    PHP_DECIMAL_LONG_CONSTANT(ce, "DEFAULT_ROUNDING",  PHP_DECIMAL_DEFAULT_ROUNDING);
    PHP_DECIMAL_LONG_CONSTANT(ce, "ROUND_UP",          PHP_DECIMAL_ROUND_UP);
    PHP_DECIMAL_LONG_CONSTANT(ce, "ROUND_DOWN",        PHP_DECIMAL_ROUND_DOWN);
    PHP_DECIMAL_LONG_CONSTANT(ce, "ROUND_CEILING",     PHP_DECIMAL_ROUND_CEILING);
    PHP_DECIMAL_LONG_CONSTANT(ce, "ROUND_FLOOR",       PHP_DECIMAL_ROUND_FLOOR);
    PHP_DECIMAL_LONG_CONSTANT(ce, "ROUND_HALF_UP",     PHP_DECIMAL_ROUND_HALF_UP);
    PHP_DECIMAL_LONG_CONSTANT(ce, "ROUND_HALF_DOWN",   PHP_DECIMAL_ROUND_HALF_DOWN);
    PHP_DECIMAL_LONG_CONSTANT(ce, "ROUND_HALF_EVEN",   PHP_DECIMAL_ROUND_HALF_EVEN);
    PHP_DECIMAL_LONG_CONSTANT(ce, "ROUND_HALF_ODD",    PHP_DECIMAL_ROUND_HALF_ODD);
    PHP_DECIMAL_LONG_CONSTANT(ce, "ROUND_TRUNCATE",    PHP_DECIMAL_ROUND_TRUNCATE);

    /**
     * Precision
     */
    PHP_DECIMAL_LONG_CONSTANT(ce, "DEFAULT_PRECISION", PHP_DECIMAL_DEFAULT_PREC);
    PHP_DECIMAL_LONG_CONSTANT(ce, "MIN_PRECISION",     PHP_DECIMAL_MIN_PREC);
    PHP_DECIMAL_LONG_CONSTANT(ce, "MAX_PRECISION",     PHP_DECIMAL_MAX_PREC);
}

void php_decimal_register_number_class()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, PHP_DECIMAL_NUMBER_FQCN, number_methods);
    php_decimal_number_ce = zend_register_internal_class(&ce);

    /**
     *
     */
    php_decimal_register_scalar_constants(php_decimal_number_ce);

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
    memcpy(&php_decimal_number_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    php_decimal_number_handlers.do_operation = php_decimal_number_do_operation;
    php_decimal_number_handlers.compare      = php_decimal_number_compare_handler;
    php_decimal_number_handlers.cast_object  = php_decimal_number_cast_object;
}

