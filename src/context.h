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
#ifndef HAVE_PHP_DECIMAL_CONTEXT_H
#define HAVE_PHP_DECIMAL_CONTEXT_H

#include <php.h>
#include <mpdecimal.h>
#include "bool.h"
#include "globals.h"
#include "errors.h"
#include "limits.h"

/**
 * Default precision.
 */
#define PHP_DECIMAL_DEFAULT_PREC  34

/**
 * Rounding applied to arithmetic, rather than rounding functions.
 */
#define PHP_DECIMAL_CONTEXT_ROUNDING  MPD_ROUND_HALF_EVEN

/**
 * Defines which conditions call the trap handler.
 */
#define PHP_DECIMAL_CONTEXT_TRAPS (MPD_Errors | MPD_Traps)

/**
 * Used to perform a task using a temporary precision.
 */
#define PHP_DECIMAL_WITH_PRECISION(p, task) do { \
    const zend_long _prec = p; \
    const zend_long _prev = SHARED_CONTEXT->prec; \
    if (UNEXPECTED(_prec > PHP_DECIMAL_MAX_PREC)) { \
        php_decimal_precision_overflow(); \
    } \
    SHARED_CONTEXT->prec = _prec; \
    task; \
    SHARED_CONTEXT->prec = _prev; \
} while(0)

/**
 * Used to perform a task using a temporary precision.
 */
#define PHP_DECIMAL_WITH_ROUNDING(m, task) do { \
    const int _mode = m; \
    const int _prev = SHARED_CONTEXT->round; \
    SHARED_CONTEXT->round = _mode; \
    task; \
    SHARED_CONTEXT->round = _prev; \
} while(0)

/**
 * Checks whether a given precision is valid.
 */
zend_bool php_decimal_validate_prec(const zend_long prec);

#endif
