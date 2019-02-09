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
#ifndef HAVE_PHP_DECIMAL_ERRORS_H
#define HAVE_PHP_DECIMAL_ERRORS_H

#include <php.h>
#include <mpdecimal.h>
#include "common.h"
#include "round.h"

/**
 * Called when a decimal is constructed using an unsupported data type.
 */
void php_decimal_unsupported_type();

/**
 * Called when an operation on many failed, because the values were not given as
 * an array or traversable object.
 */
void php_decimal_expected_iterable();

/**
 * Throws an exception when an invalid precision was given, eg. -1
 */
void php_decimal_precision_out_of_range(zend_long prec);

/**
 * Called when a memory allocation error has been detected.
 */
void php_decimal_memory_error();

/**
 * Called when a rounding operation is attempted with an invalid mode.
 */
void php_decimal_unsupported_rounding_mode(php_decimal_rounding_t mode);

/**
 * Called when a string conversion failed.
 */
void php_decimal_failed_to_parse_string(zend_string * str);

/**
 * Called when a decimal could not be rounded, potentially beyond max prec.
 */
void php_decimal_failed_to_round();

/**
 * Called when an attempt was made to divide a decimal by 0.
 */
void php_decimal_division_by_zero_error();

/**
 * Called when a value could not be unserialized. This should never happen unless
 * someone is trying to do something they're not supposed to.
 */
void php_decimal_unserialize_error();

/**
 * Called when an attempt is made to read or write object properties.
 */
void php_decimal_object_properties_not_supported();

/**
 * Called when a decimal is too large to be converted to double.
 */
void php_decimal_floating_point_overflow();

/**
 * Called when a decimal is too small to be converted to double, eg. 1E-1000
 */
void php_decimal_floating_point_underflow();

/**
 * Called when a decimal is too large to be converted to int, eg. 1E+1000
 */
void php_decimal_integer_overflow();

/**
 * Called when NaN or Inf is converted to integer.
 */
void php_decimal_integer_from_special_is_not_defined();

/**
 * Called when attempting to query the signum of NaN.
 */
void php_decimal_sign_of_nan_is_not_defined();

/**
 * Called when an operation is attempted using an unsupported operator, like "|".
 */
void php_decimal_operator_not_supported();

/**
 * Called when an operation's precision is higher than the maximum precision.
 */
void php_decimal_precision_overflow();

/**
 * Called when a rational approximation is attempted using a non-integer operand.
 * This only applies to operator-overloaded attempts because there is no way to
 * indicate a precision. Instead of using a default precision, we truncate the
 * operand to integer, resulting in a rational result.
 */
void php_decimal_operand_truncated_to_integer();

/**
 * Called when a userland number implemented has not implemented its "valueOf".
 */
void php_decimal_number_not_implemented();

/**
 * Called when a trap is triggered in mpdecimal when calling signalling
 * functions (non-quiet). These methods usually don't have the "q" prefix in
 * their names and don't require a status argument. The non-signalling functions
 * should be used whenever we do not expect anything to go wrong or when we want
 * to handle the status directly. This is basically a catch-all error handler.
 *
 * See PHP_DECIMAL_TRAPS
 */
void php_decimal_mpd_traphandler(mpd_context_t *ctx);

#endif
