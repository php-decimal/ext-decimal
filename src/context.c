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
#include <mpdecimal.h>
#include "bool.h"
#include "context.h"
#include "round.h"
#include "errors.h"
#include "limits.h"

/**
 * Returns true if the given precision is valid, false otherwise.
 */
static inline zend_bool php_decimal_prec_is_valid(const zend_long prec)
{
    return prec >= PHP_DECIMAL_MIN_PREC && prec <= PHP_DECIMAL_MAX_PREC;
}

/**
 *
 */
zend_bool php_decimal_validate_prec(const zend_long prec)
{
    if (php_decimal_prec_is_valid(prec)) {
        return true;
    }

    /* */
    php_decimal_precision_out_of_range(prec);
    return false;
}

