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
#ifndef HAVE_PHP_DECIMAL_H
#define HAVE_PHP_DECIMAL_H

#ifdef PHP_WIN32
#   define PHP_DECIMAL_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#   define PHP_DECIMAL_API __attribute__ ((visibility("default")))
#else
#   define PHP_DECIMAL_API
#endif

#ifdef ZTS
#   include "TSRM.h"
#endif

#include <php.h>
#include <mpdecimal.h>
#include "src/globals.h"

#define PHP_DECIMAL_EXTNAME "decimal"
#define PHP_DECIMAL_VERSION "2.0.0"

/**
 * Module and class entry
 */
extern zend_module_entry php_decimal_module_entry;

#define phpext_decimal_ptr &php_decimal_module_entry

#if defined(ZTS) && defined(COMPILE_DL_DECIMAL)
    ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif
