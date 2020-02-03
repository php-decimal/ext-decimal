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
#ifndef PHP_DECIMAL_PARAMS_H
#define PHP_DECIMAL_PARAMS_H

#include <php.h>

/**
 * Start parameter parsing block, throwing exceptions for incorrect arg count.
 */
#define PHP_DECIMAL_PARSE_PARAMS(min, max) \
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, min, max)

/**
 * End a parameter parse block. This exists to achieve consistent naming.
 */
#define PHP_DECIMAL_PARSE_PARAMS_END() \
    ZEND_PARSE_PARAMETERS_END();

/**
 * No parameters expected - warn only because that appears to be the custom.
 */
#define PHP_DECIMAL_PARSE_PARAMS_NONE() do { \
    if (zend_parse_parameters_none() == FAILURE) { \
        return; \
    } \
} while (0)

#endif
