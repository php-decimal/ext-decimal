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
#ifndef HAVE_PHP_DECIMAL_RATIONAL_H
#define HAVE_PHP_DECIMAL_RATIONAL_H

#include <php.h>
#include "common.h"

#define PHP_RATIONAL_CLASS_NAME "Rational"
#define PHP_DECIMAL_RATIONAL_FQCN        PHP_DECIMAL_NAMESPACE "\\" PHP_RATIONAL_CLASS_NAME

/**
 * Object and zval access
 */
#define PHP_RATIONAL_NUM(f)     (&(f)->num)
#define PHP_RATIONAL_DEN(f)     (&(f)->den)

#define Z_RATIONAL_P(z)         ((php_rational_t*) Z_OBJ_P(z))
#define Z_OBJ_IS_RATIONAL_P(z)  (Z_OBJCE_P(z) == php_decimal_rational_ce)
#define Z_IS_RATIONAL_P(z)      (Z_TYPE_P(z) == IS_OBJECT && Z_OBJ_IS_RATIONAL_P(z))
#define ZVAL_RATIONAL(z, f)     ZVAL_OBJ(z, (zend_object*) f)
#define THIS_RATIONAL()         Z_RATIONAL_P(getThis())

#define RETURN_RATIONAL(f) \
do { \
    php_rational_t *_f = f; \
    if (_f) { \
        ZVAL_DECIMAL(return_value, _f); \
    } else { \
        ZVAL_NULL(return_value); \
    } \
    return; \
} while(0)

#define PHP_RATIONAL_OBJ_IS_INITIALIZED(f) (PHP_DECIMAL_MPD_IS_INITIALIZED(PHP_RATIONAL_NUM(f)))

/**
 *
 */
extern zend_class_entry *php_decimal_rational_ce;

/**
 * PHP decimal object.
 */
typedef struct _php_rational_t {
    zend_object  std; /* Must be first! */
    mpd_t        num;
    mpd_t        den;
} php_rational_t;

void php_decimal_register_rational_class();

php_rational_t *php_rational();

#endif
