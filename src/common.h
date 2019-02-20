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
#ifndef HAVE_PHP_DECIMAL_COMMON_H
#define HAVE_PHP_DECIMAL_COMMON_H

#include <php.h>
#include <mpdecimal.h>
#include "globals.h"

#define PHP_DECIMAL_NAMESPACE "Decimal"

/**
 * These should be free'd with mpd_del if used.
 */
#define PHP_DECIMAL_TEMP_MPD(name) \
    mpd_uint_t name##_data[MPD_MINALLOC_MAX]; \
    mpd_t      name = {MPD_STATIC|MPD_STATIC_DATA, 0, 0, 0, MPD_MINALLOC_MAX, name##_data} \

// *
//  * Checks if a decimal object has been constructed or otherwise intialized.

// #define PHP_DECIMAL_MPD_IS_INITIALIZED(d) ((d)->data != NULL)

/**
 * Class, method, and function entry
 */
#define PHP_DECIMAL_METHOD(cls, name) \
    PHP_METHOD(cls, name)

#define PHP_DECIMAL_ME_STATIC_EX(cls, name, flags) \
    PHP_ME(cls, name, PHP_DECIMAL_ARGINFO_NAME(cls, name), ZEND_ACC_STATIC | (flags))

#define PHP_DECIMAL_ME_STATIC(cls, name) \
    PHP_DECIMAL_ME_STATIC_EX(cls, name, ZEND_ACC_PUBLIC)

#define PHP_DECIMAL_ME_EX(cls, name, flags) \
    PHP_ME(cls, name, PHP_DECIMAL_ARGINFO_NAME(cls, name), flags)

#define PHP_DECIMAL_ME(cls, name) \
    PHP_DECIMAL_ME_EX(cls, name, ZEND_ACC_PUBLIC)

#define PHP_DECIMAL_ME_ABSTRACT_EX(cls, name, flags) \
    ZEND_FENTRY(name, NULL, PHP_DECIMAL_ARGINFO_NAME(cls, name), ZEND_ACC_ABSTRACT | (flags))

#define PHP_DECIMAL_ME_ABSTRACT(cls, name) \
    PHP_DECIMAL_ME_ABSTRACT_EX(cls, name, ZEND_ACC_PUBLIC)


#define PHP_DECIMAL_LONG_CONSTANT(ce, name, value) \
    zend_declare_class_constant_long(ce, name, sizeof(name) - 1, value)

/**
 * Custom definitions.
 */
typedef int php_decimal_success_t;

/**
 * Resets all flags and allocates the data for a given MPD. We are using this
 * manual approach to avoid allocating an mpd_t when we allocate a decimal.
 *
 * We instead embed the struct itself, rather than a pointer to a new one.
 * This requires that we borrow some internals from mpdecimal to initialize an
 * already allocated mpd_t. This would usually be done by mpd_new.
 */
void php_decimal_init_mpd(mpd_t *mpd);
void php_decimal_release_mpd(mpd_t *mpd);

/**
 *
 */
zval *php_decimal_blocked_read_property(zend_object *zobj, zend_string *name, int type, void **cache_slot, zval *rv);
zval *php_decimal_blocked_write_property(zend_object *zobj, zend_string *name, zval *value, void **cache_slot);
int   php_decimal_blocked_has_property(zend_object *zobj, zend_string *name, int has_set_exists, void **cache_slot);
void  php_decimal_blocked_unset_property(zend_object *zobj, zend_string *name, void **cache_slot);

#endif
