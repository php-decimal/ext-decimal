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
#ifndef PHP_DECIMAL_PARSE_H
#define PHP_DECIMAL_PARSE_H

#include <php.h>
#include <mpdecimal.h>
#include "common.h"
#include "decimal.h"
#include "rational.h"

php_decimal_success_t php_decimal_parse_scalar(mpd_t *res, const zval *val);
php_decimal_success_t php_decimal_parse_scalar_quiet(mpd_t *res, const zval *val);

php_decimal_success_t php_decimal_parse_rational(zval *result, const zval *val);
php_decimal_success_t php_decimal_parse_decimal(zval *result, const zval *val, const zend_long prec, zend_bool inherit);
php_decimal_success_t php_decimal_parse_mpd(mpd_t *res, const zval *val, const zend_long prec);
php_decimal_success_t php_decimal_parse_num_den(mpd_t *num, mpd_t *den, const zval *val);

#endif
