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
#include <Zend/zend_exceptions.h>
#include <ext/spl/spl_exceptions.h>

#include "errors.h"
#include "decimal.h"

static void php_decimal_wrong_type(const char *expected)
{
    const char *space;
    const char *cname = get_active_class_name(&space);

    zend_throw_exception_ex(zend_ce_type_error, 0, "%s%s%s() expected parameter %d to be %s",
        cname,
        space,
        get_active_function_name(),
        1, // index
        expected
    );
}

void php_decimal_unsupported_type()
{
    php_decimal_wrong_type("a decimal number, string, or integer");
}

void php_decimal_expected_iterable()
{
    php_decimal_wrong_type("an array or traversable object");
}

void php_decimal_precision_out_of_range(zend_long prec)
{
    zend_throw_exception(spl_ce_OutOfRangeException, "Decimal precision out of range", 0);
}

void php_decimal_memory_error()
{
    zend_error(E_ERROR, "Failed to allocate memory for decimal");
}

void php_decimal_unsupported_rounding_mode(php_decimal_rounding_t mode)
{
    zend_throw_exception(spl_ce_InvalidArgumentException, "Unsupported rounding mode", 0);
}

void php_decimal_failed_to_parse_string(zend_string *str)
{
    zend_throw_exception_ex(spl_ce_DomainException, 0, "Failed to parse string as decimal: \"%s\"", ZSTR_VAL(str));
}

void php_decimal_failed_to_round()
{
    zend_throw_exception(spl_ce_RuntimeException, "Failed to round decimal - exponent out of range", 0);
}

void php_decimal_division_by_zero_error()
{
    zend_throw_exception(zend_ce_division_by_zero_error, "Division by zero", 0);
}

void php_decimal_unserialize_error()
{
    zend_throw_exception(spl_ce_InvalidArgumentException, "Could not unserialize decimal", 0);
}

void php_decimal_object_properties_not_supported()
{
    zend_error(E_NOTICE, "Object properties are not supported");
}

void php_decimal_floating_point_overflow()
{
    zend_throw_exception(spl_ce_OverflowException, "Floating point overflow", 0);
}

void php_decimal_floating_point_underflow()
{
    zend_throw_exception(spl_ce_UnderflowException, "Floating point underflow", 0);
}

void php_decimal_integer_overflow()
{
    zend_throw_exception(spl_ce_OverflowException, "Integer overflow", 0);
}

void php_decimal_operator_not_supported()
{
    zend_throw_exception(zend_ce_arithmetic_error, "Operator not supported by decimal numbers", 0);
}

void php_decimal_precision_overflow()
{
    zend_error(E_WARNING, "Possible loss of precision: operation exceeds maximum precision");
}

void php_decimal_operand_truncated_to_integer()
{
    zend_error(E_WARNING, "Rational arithmetic operand truncated to integer");
}

void php_decimal_number_not_implemented()
{
    zend_error(E_NOTICE, "Number implementations must implement 'valueOf'");
}

void php_decimal_mpd_traphandler(mpd_context_t *ctx)
{
    uint32_t status = mpd_getstatus(ctx);

    /* Potentially out of memory, which should be caught by the PHP allocator? */
    if (status & MPD_Malloc_error) {
        php_decimal_memory_error();
        return;
    }

    /* ¯\_(ツ)_/¯ */
    zend_error(E_ERROR, "Unexpected error - please report!");
}
