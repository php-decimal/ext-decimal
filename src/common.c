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
#include "errors.h"
#include "common.h"
#include "convert.h"

void php_decimal_init_mpd(mpd_t *mpd)
{
    mpd->data   = mpd_alloc(MPD_MINALLOC, sizeof(mpd_uint_t));
    mpd->flags  = 0;
    mpd->exp    = 0;
    mpd->digits = 0;
    mpd->len    = 0;
    mpd->alloc  = MPD_MINALLOC;

    if (UNEXPECTED(mpd->data == NULL)) {
        php_decimal_memory_error();
        return;
    }
}

void php_decimal_release_mpd(mpd_t *mpd)
{
    if (mpd->data) {
        mpd_free(mpd->data);
    }
}

/**
 * Object property read - not supported.
 */
zval *php_decimal_guarded_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv)
{
    php_decimal_object_properties_not_supported();
    return &EG(uninitialized_zval);
}

/**
 *   Object property write - not supported.
 */
void php_decimal_guarded_write_property(zval *object, zval *member, zval *value, void **cache_slot)
{
    php_decimal_object_properties_not_supported();
}

/**
 * Object property isset/empty - not supported.
 */
int php_decimal_guarded_has_property(zval *object, zval *member, int has_set_exists, void **cache_slot)
{
    php_decimal_object_properties_not_supported();
    return 0;
}

/**
 * Object property unset - not supported.
 */
void php_decimal_guarded_unset_property(zval *object, zval *member, void **cache_slot)
{
    php_decimal_object_properties_not_supported();
}
