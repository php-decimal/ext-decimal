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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <ext/standard/info.h>
#include "php_decimal.h"
#include "src/context.h"
#include "src/decimal.h"
#include "src/errors.h"
#include "src/number.h"
#include "src/number.h"
#include "src/rational.h"

/**
 * Set custom allocators.
 */
static void *php_decimal_mpd_malloc(size_t size)
{
    return emalloc(size);
}

static void *php_decimal_mpd_calloc(size_t nmemb, size_t size)
{
    return ecalloc(nmemb, size);
}

static void *php_decimal_mpd_realloc(void *ptr, size_t size)
{
    return erealloc(ptr, size);
}

static void php_decimal_mpd_free(void *ptr)
{
    efree(ptr);
}

/**
 * The second Opcache pass will convert numeric string constants to float,
 * so statements like `$decimal * "0.75"` will throw because floats are not
 * supported. Otherwise, this conversion will be transparent which means you
 * are using float internally when your code uses a string.
 *
 * Disabling it gives us guaranteed consistency at a small performance cost.
 */
static void php_decimal_disable_opcache_pass2()
{
    zend_long level = INI_INT("opcache.optimization_level");

    if (level) {
        zend_string *key = zend_string_init(ZEND_STRL("opcache.optimization_level"), 1);
        zend_string *val = strpprintf(0, "0x%08X", (unsigned int) (level & ~2));

        zend_alter_ini_entry(key, val, ZEND_INI_SYSTEM, ZEND_INI_STAGE_ACTIVATE);
        zend_string_release(key);
        zend_string_release(val);
    }
}

/**
 * Module information displayed by phpinfo()
 */
PHP_MINFO_FUNCTION(decimal)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "decimal support", "enabled");
    php_info_print_table_row(2, "decimal version", PHP_DECIMAL_VERSION);
    php_info_print_table_row(2, "libmpdec version", MPD_VERSION);
    php_info_print_table_end();
}

/**
 * Module startup
 */
PHP_MINIT_FUNCTION(decimal)
{
    ZEND_INIT_MODULE_GLOBALS(decimal, php_decimal_init_globals, NULL);

    /* Set guaranteed minimum number of coefficient words based on default prec. */
    mpd_setminalloc(2 * ((PHP_DECIMAL_DEFAULT_PREC + MPD_RDIGITS - 1) / MPD_RDIGITS));

    /* Set custom memory allocation functions */
    mpd_callocfunc  = php_decimal_mpd_calloc;
    mpd_mallocfunc  = php_decimal_mpd_malloc;
    mpd_reallocfunc = php_decimal_mpd_realloc;
    mpd_free        = php_decimal_mpd_free;
    mpd_traphandler = php_decimal_mpd_traphandler;

    php_decimal_register_number_class();
    php_decimal_register_decimal_class();
    php_decimal_register_rational_class();
    
    return SUCCESS;
}

/**
 * Module shutdown
 */
PHP_MSHUTDOWN_FUNCTION(decimal)
{
    return SUCCESS;
}

/**
 * Request startup
 */
PHP_RINIT_FUNCTION(decimal)
{
#if defined(COMPILE_DL_DECIMAL) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    php_decimal_disable_opcache_pass2();
    php_decimal_context_init();
    php_decimal_init_decimal_constants();

    return SUCCESS;
}

/**
 * Request shutdown
 */
PHP_RSHUTDOWN_FUNCTION(decimal)
{
    php_decimal_context_dtor();
    php_decimal_dtor_decimal_constants();

    return SUCCESS;
}

/**
 * Module entry
 */
zend_module_entry decimal_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_DECIMAL_EXTNAME,
    NULL, /* no functions */
    PHP_MINIT(decimal),
    PHP_MSHUTDOWN(decimal),
    PHP_RINIT(decimal),
    PHP_RSHUTDOWN(decimal),
    PHP_MINFO(decimal),
    PHP_DECIMAL_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DECIMAL
#ifdef ZTS
    ZEND_TSRMLS_CACHE_DEFINE();
#endif
    ZEND_GET_MODULE(decimal)
#endif
