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
#include "globals.h"
#include "context.h"

/**
 *
 */
ZEND_DECLARE_MODULE_GLOBALS(decimal)

/**
 *
 */
void php_decimal_init_globals(zend_decimal_globals *g)
{
    memset(g, 0, sizeof(zend_decimal_globals));
}

void php_decimal_init_context()
{
    /* Initialize the default shared context. */
    mpd_defaultcontext(SHARED_CONTEXT);
    mpd_qsettraps(SHARED_CONTEXT, PHP_DECIMAL_CONTEXT_TRAPS);
    mpd_qsetround(SHARED_CONTEXT, PHP_DECIMAL_CONTEXT_ROUNDING);

    /* Initialize the maximum precision context (for rational) */
    mpd_maxcontext(MAX_CONTEXT);
    mpd_qsettraps(MAX_CONTEXT, PHP_DECIMAL_CONTEXT_TRAPS);
    mpd_qsetround(MAX_CONTEXT, PHP_DECIMAL_CONTEXT_ROUNDING);
}
