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
#ifndef HAVE_PHP_DECIMAL_NUMBER_H
#define HAVE_PHP_DECIMAL_NUMBER_H

#include <php.h>
#include "common.h"

#define PHP_DECIMAL_NUMBER_CLASS_NAME "Number"
#define PHP_DECIMAL_NUMBER_FQCN        PHP_DECIMAL_NAMESPACE "\\" PHP_DECIMAL_NUMBER_CLASS_NAME

#define Z_OBJ_IS_NUMBER_P(z) (instanceof_function(Z_OBJCE_P(z), php_decimal_number_ce))
#define Z_IS_DECIMAL_NUMBER_P(z) (Z_TYPE_P(z) == IS_OBJECT && Z_OBJ_IS_NUMBER_P(z))

/**
 *
 */
extern zend_class_entry *php_decimal_number_ce;

/**
 *
 */
void php_decimal_register_number_class();

#endif
