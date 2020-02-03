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
#ifndef HAVE_PHP_DECIMAL_CONVERT_H
#define HAVE_PHP_DECIMAL_CONVERT_H

#include <php.h>
#include <mpdecimal.h>
#include "bool.h"
#include "common.h"
#include "context.h"
#include "round.h"

/**
 * The string case of the "e" in scientific notation output.
 *
 * 0: Lowercase, eg. 1.5e+6
 * 1: Uppercase, eg. 1.5E+6
 */
#define PHP_DECIMAL_EXP_STR_CASE 1

/**
 *
 */
zend_bool php_decimal_mpd_is_one(const mpd_t *mpd);

void                  php_decimal_mpd_set_one(mpd_t *mpd);
void                  php_decimal_mpd_set_double(mpd_t *res, double dval);
void                  php_decimal_mpd_set_long(mpd_t *mpd, zend_long lval);
void                  php_decimal_mpd_set_ulong(mpd_t *mpd, zend_ulong lval);
void                  php_decimal_mpd_set_exp(mpd_t *mpd, mpd_ssize_t exp);
void                  php_decimal_mpd_set_zero(mpd_t *mpd);
void                  php_decimal_mpd_set_nan(mpd_t *mpd);
void                  php_decimal_mpd_set_inf(mpd_t *mpd, uint8_t sign);
php_decimal_success_t php_decimal_mpd_set_special(mpd_t *res, double dval);
php_decimal_success_t php_decimal_mpd_set_string(mpd_t *mpd, const zend_string *str);
php_decimal_success_t php_decimal_mpd_set_charptr(mpd_t *mpd, const char *str);

zend_string *php_decimal_mpd_to_serialized(const mpd_t *mpd);
zend_string *php_decimal_mpd_to_eng(const mpd_t *mpd);
zend_string *php_decimal_mpd_to_sci(const mpd_t *mpd);
zend_string *php_decimal_mpd_to_string(const mpd_t *mpd);
zend_string *php_decimal_mpd_to_fixed(const mpd_t *mpd, zend_long places, zend_bool commas, php_decimal_rounding_t mode);
zend_long    php_decimal_mpd_to_long(const mpd_t *mpd);
double       php_decimal_mpd_to_double(const mpd_t *mpd);

zend_bool php_decimal_rational_is_nan(const php_rational_t *obj);
void      php_decimal_rational_set_inf(php_rational_t *obj, uint8_t sign);
void      php_decimal_rational_set_nan(php_rational_t *obj);
void      php_decimal_rational_set_zero(php_rational_t *obj);

zend_string *php_decimal_rational_to_sci(const php_rational_t *obj, zend_long prec);
zend_string *php_decimal_rational_to_eng(const php_rational_t *obj, zend_long prec);
zend_string *php_decimal_rational_to_string(const php_rational_t *obj);
zend_string *php_decimal_rational_to_fixed(const php_rational_t *obj, zend_long places, zend_bool commas, php_decimal_rounding_t mode);
zend_long    php_decimal_rational_to_long(const php_rational_t *obj);
double       php_decimal_rational_to_double(const php_rational_t *obj);

zend_bool php_decimal_number_is_zero(const zval *obj);
zend_bool php_decimal_number_is_integer(const zval *obj);
zend_bool php_decimal_number_is_positive(const zval *obj);
zend_bool php_decimal_number_is_negative(const zval *obj);
zend_bool php_decimal_number_is_nan(const zval *obj);
zend_bool php_decimal_number_is_inf(const zval *obj);
double    php_decimal_number_to_double(const zval *obj);
zend_long php_decimal_number_to_long(const zval *obj);
void      php_decimal_number_to_string(zval *result, const zval *obj);
void      php_decimal_number_to_mpd(mpd_t *mpd, const zval *obj, const zend_long prec);
void      php_decimal_number_to_num_den(mpd_t *num, mpd_t *den, const zval *obj);

#endif
