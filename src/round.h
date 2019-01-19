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
#ifndef HAVE_PHP_DECIMAL_ROUND_H
#define HAVE_PHP_DECIMAL_ROUND_H

#include <php.h>
#include <ext/standard/php_math.h>
#include <mpdecimal.h>
#include "decimal.h"
#include "rational.h"

/**
 * Rounding modes.
 *
 * The layout of the constant values allows us to support PHP rounding constants
 * as well. There is a converter in the rounding module that translates these
 * values to the corresponding MPD rounding mode constant.
 */
#define PHP_DECIMAL_ROUND_UP         101
#define PHP_DECIMAL_ROUND_DOWN       102
#define PHP_DECIMAL_ROUND_CEILING    103
#define PHP_DECIMAL_ROUND_FLOOR      104
#define PHP_DECIMAL_ROUND_TRUNCATE   105

#define PHP_DECIMAL_ROUND_HALF_UP    PHP_ROUND_HALF_UP     // 1
#define PHP_DECIMAL_ROUND_HALF_DOWN  PHP_ROUND_HALF_DOWN   // 2
#define PHP_DECIMAL_ROUND_HALF_EVEN  PHP_ROUND_HALF_EVEN   // 3
#define PHP_DECIMAL_ROUND_HALF_ODD   PHP_ROUND_HALF_ODD    // 4

/**
 * This is the default rounding mode if none is given.
 */
#define PHP_DECIMAL_DEFAULT_ROUNDING PHP_DECIMAL_ROUND_HALF_EVEN

/**
 * This is the default internal rounding mode (eg. applied to arithmetic).
 */
#define PHP_DECIMAL_DEFAULT_MPD_ROUNDING MPD_ROUND_HALF_EVEN

/**
 * Marker to indicate an unconverted rounding mode.
 */
typedef int php_decimal_rounding_t;

php_decimal_success_t php_decimal_round(mpd_t *res, const mpd_t *mpd, zend_long scale, php_decimal_rounding_t mode);
php_decimal_success_t php_decimal_rational_round(mpd_t *res, const php_rational_t *obj, zend_long scale, php_decimal_rounding_t mode);

void php_decimal_shift(mpd_t *res, const php_decimal_t *obj, zend_long places);
void php_decimal_rational_shift(mpd_t *rnum, mpd_t *rden, const php_rational_t *obj, zend_long places);

#endif
