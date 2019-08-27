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
#ifndef HAVE_PHP_DECIMAL_DECIMAL_H
#define HAVE_PHP_DECIMAL_DECIMAL_H

#include <php.h>
#include "common.h"

#define PHP_DECIMAL_CLASS_NAME "Decimal"
#define PHP_DECIMAL_DECIMAL_FQCN        PHP_DECIMAL_NAMESPACE "\\" PHP_DECIMAL_CLASS_NAME


/**
 * Checks if a decimal object has been constructed or otherwise intialized.
 */
#define PHP_DECIMAL_OBJ_IS_INITIALIZED(d) (PHP_DECIMAL_MPD(d)->data != NULL)

/**
 * Object and zval access
 */
#define PHP_DECIMAL_MPD(p)      (&(p)->mpd)
#define Z_DECIMAL_P(z)          ((php_decimal_t*) Z_OBJ_P(z))
#define Z_DECIMAL(z)            ((php_decimal_t*) Z_OBJ(z))
#define Z_MPD_P(z)              PHP_DECIMAL_MPD(Z_DECIMAL_P(z))
#define Z_MPD(z)                PHP_DECIMAL_MPD(Z_DECIMAL(z))
#define Z_OBJ_IS_DECIMAL_P(z)   (Z_OBJCE_P(z) == php_decimal_decimal_ce)
#define Z_IS_DECIMAL_P(z)       (Z_TYPE_P(z) == IS_OBJECT && Z_OBJ_IS_DECIMAL_P(z))
#define ZVAL_DECIMAL(z, dec)    ZVAL_OBJ(z, (zend_object*) dec)
#define THIS_DECIMAL()          Z_DECIMAL_P(getThis())
#define THIS_DECIMAL_MPD()      Z_MPD_P(getThis())

#define RETURN_DECIMAL(dec) \
do { \
    php_decimal_t *_dec = dec; \
    if (_dec) { \
        ZVAL_DECIMAL(return_value, _dec); \
    } else { \
        ZVAL_NULL(return_value); \
    } \
    return; \
} while(0)


/******************************************************************************/
/*                             CONTEXT & PRECISION                            */
/******************************************************************************/

/**
 * Returns the significand precision of a given decimal object.
 */
#define php_decimal_get_prec(o) ((o)->prec)

/**
 * Sets the significand precision of a given decimal object.
 */
#define php_decimal_set_prec(o, p) ((o)->prec = p)

/**
 * Decimal\Decimal class entry.
 */
extern zend_class_entry *php_decimal_decimal_ce;

/**
 * PHP decimal object.
 */
typedef struct _php_decimal_t {
    zend_object  std;    /* Zend object, must be first! */
    mpd_t        mpd;    /* Embedded MPD object */
    zend_long    prec;   /* Precision */
} php_decimal_t;

/**
 *
 */
void php_decimal_register_decimal_class();

/**
 *
 */
void php_decimal_init_decimal_constants();

/**
 *
 */
void php_decimal_dtor_decimal_constants();

/**
 * Prints a decimal object to stdout.
 */
void php_decimal_print(const php_decimal_t *obj);

/**
 *
 */
php_decimal_t *php_decimal();

/**
 *
 */
php_decimal_t *php_decimal_with_prec(zend_long prec);

/**
 *
 */
php_decimal_t *php_decimal_create_copy(const php_decimal_t *src);

#endif
