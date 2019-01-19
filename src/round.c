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
#include <mpdecimal.h>
#include "common.h"
#include "context.h"
#include "errors.h"
#include "math.h"
#include "round.h"

/**
 * This library supports both its own rounding constants and the PHP rounding
 * mode constants. The user should not have to wonder which constant to use.
 *
 * In order to support both, as well as some modes not supported by mpdecimal,
 * we need to convert the requested mode to an mpdecimal rounding mode.
 */
static int php_decimal_convert_to_mpd_rounding_mode(const mpd_t *mpd, zend_long scale, php_decimal_rounding_t mode)
{
    switch (mode) {
        case PHP_DECIMAL_ROUND_UP:        return MPD_ROUND_UP;
        case PHP_DECIMAL_ROUND_DOWN:      return MPD_ROUND_DOWN;
        case PHP_DECIMAL_ROUND_CEILING:   return MPD_ROUND_CEILING;
        case PHP_DECIMAL_ROUND_FLOOR:     return MPD_ROUND_FLOOR;
        case PHP_DECIMAL_ROUND_TRUNCATE:  return MPD_ROUND_TRUNC;
        case PHP_DECIMAL_ROUND_HALF_UP:   return MPD_ROUND_HALF_UP;
        case PHP_DECIMAL_ROUND_HALF_DOWN: return MPD_ROUND_HALF_DOWN;
        case PHP_DECIMAL_ROUND_HALF_EVEN: return MPD_ROUND_HALF_EVEN;

        /**
         * Special case for half-odd.
         *
         * This depends on the value's parity because half-odd is not
         * implemented by mpdecimal. We want to support the PHP constant, so we
         * need to adjust to a mode that is equivalent to an MPD rounding mode.
         */
        case PHP_DECIMAL_ROUND_HALF_ODD: {
            uint32_t status = 0;

            /* This guards against undefined behaviour when checking parity. */
            if (UNEXPECTED(mpd_isspecial(mpd))) {
                return MPD_ROUND_TRUNC;
            }

            /**
             * Determine the integer parity at the point of rounding to know
             * which way we should round to get to the nearest odd number.
             *
             * For example, 0.12345, rounded to 4 decimal places is on the 4.
             */
            PHP_DECIMAL_TEMP_MPD(tmp);
            mpd_qshiftl(&tmp, mpd, scale, &status);
            mpd_qtrunc(&tmp, &tmp, SHARED_CONTEXT, &status);

            /* An odd digit should round down towards itself. */
            mode = mpd_isodd(&tmp) ? MPD_ROUND_HALF_DOWN : MPD_ROUND_HALF_UP;

            mpd_del(&tmp);
            return mode;
        }
    }

    /* Couldn't determine a match, rounding mode is not supported. */
    return -1;
}

/**
 * Rounds a given mpd to a number of decimal places (scale), using a given
 * php decimal rounding mode. If the scale is beyond the number of decimal
 * places, trailing zeroes should be appended, increasing significance.
 */
php_decimal_success_t php_decimal_round(mpd_t *res, const mpd_t *mpd, zend_long places, php_decimal_rounding_t mode)
{
    uint32_t status = 0;

    /* Convert the rounding constant to an MPD constant. */
    mode = php_decimal_convert_to_mpd_rounding_mode(mpd, places, mode);

    if (mode < 0) {
        php_decimal_unsupported_rounding_mode(mode);
        return FAILURE;
    }

    PHP_DECIMAL_WITH_ROUNDING(mode, {
        mpd_qrescale(res, mpd, -places, SHARED_CONTEXT, &status);
    });

    /* This can happen when `places` is out of range (exponent). */
    if (UNEXPECTED(status & MPD_Invalid_operation)) {
        php_decimal_failed_to_round();
        return FAILURE;
    }

    return SUCCESS;
}

php_decimal_success_t php_decimal_rational_round(mpd_t *res, const php_rational_t *obj, zend_long places, php_decimal_rounding_t mode)
{
    php_decimal_rational_evaluate(res, obj, PHP_RATIONAL_NUM(obj)->digits + places + 1);

    return php_decimal_round(res, res, places, mode);
}
