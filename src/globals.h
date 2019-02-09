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
#ifndef HAVE_PHP_DECIMAL_MODULE_GLOBALS_H
#define HAVE_PHP_DECIMAL_MODULE_GLOBALS_H

#include <mpdecimal.h>

#ifdef ZTS
    #define DECIMAL_GLOBALS(v) TSRMG(decimal_globals_id, zend_decimal_globals *, v)
#else
    #define DECIMAL_GLOBALS(v) (decimal_globals.v)
#endif

/**
 * The global, shared mpd context.
 */
#define SHARED_CONTEXT (&DECIMAL_GLOBALS(ctx))

/**
 * The global, max mpd context.
 */
#define MAX_CONTEXT (&DECIMAL_GLOBALS(max))

/**
 * Declare module globals, which can be accessed using DECIMAL_GLOBALS.
 */
ZEND_BEGIN_MODULE_GLOBALS(decimal)
mpd_context_t ctx;
mpd_context_t max;
ZEND_END_MODULE_GLOBALS(decimal)

ZEND_EXTERN_MODULE_GLOBALS(decimal)

void php_decimal_init_globals(zend_decimal_globals *g);
void php_decimal_init_context();

#endif
