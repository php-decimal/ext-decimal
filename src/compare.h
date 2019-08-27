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
#ifndef HAVE_PHP_DECIMAL_COMPARE_H
#define HAVE_PHP_DECIMAL_COMPARE_H

#include <php.h>
#include <mpdecimal.h>
#include "decimal.h"
#include "rational.h"
#include "number.h"

/**
 *
 */
#define PHP_DECIMAL_COMPARISON_FAILURE   1
#define PHP_DECIMAL_COMPARISON_UNDEFINED 2

/**
 *
 */
#define PHP_DECIMAL_COMPARE_BETWEEN_INCLUSIVE_DEFAULT true

/**
 *
 */
#define PHP_DECIMAL_COMPARE(a, b) \
	((a) == (b) ? 0 : ((a) < (b) ? -1 : 1))

/**
 *
 */
int       php_decimal_compare(const php_decimal_t *obj, const zval *op2);
zend_bool php_decimal_between(const php_decimal_t *obj, const zval *op1, const zval *op2, const zend_bool inclusive);

// /**
//  *
//  */
int       php_decimal_rational_compare(const php_rational_t *obj, const zval *op2);
zend_bool php_decimal_rational_between(const php_rational_t *obj, const zval *op1, const zval *op2, const zend_bool inclusive);

/**
 *
 */
int       php_decimal_number_compare(const zval *obj, const zval *op2);
zend_bool php_decimal_number_between(const zval *obj, const zval *op1, const zval *op2, const zend_bool inclusive);
zend_bool php_decimal_number_equals(const zval *obj, const zval *op2);

#endif
