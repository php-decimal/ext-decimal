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
#include "convert.h"
#include "errors.h"
#include "math.h"

/**
 * Alias for consistency.
 */
#define mpd_negate mpd_qcopy_negate


/******************************************************************************/
/*                                 DECIMAL                                    */
/******************************************************************************/

// TODO: Check for unsafe cases where res is used as tmp (res might be op1)

int php_decimal_signum(const mpd_t *mpd)
{
    if (UNEXPECTED(mpd_isnan(mpd))) {
        php_decimal_sign_of_nan_is_not_defined();
        return 0;
    }

    return mpd_iszero(mpd) ? 0 : mpd_arith_sign(mpd);
}

void php_decimal_abs(mpd_t *res, const mpd_t *op1, zend_long prec)
{
    uint32_t status = 0;

    mpd_qabs(res, op1, MAX_CONTEXT, &status);
}

void php_decimal_neg(mpd_t *res, const mpd_t *op1, zend_long prec)
{
    uint32_t status = 0;

    mpd_negate(res, op1, &status);
}

void php_decimal_add(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qadd(res, op1, op2, SHARED_CONTEXT, &status);
    });
}

void php_decimal_sub(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qsub(res, op1, op2, SHARED_CONTEXT, &status);
    });
}

void php_decimal_mul(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qmul(res, op1, op2, SHARED_CONTEXT, &status);
    });
}

void php_decimal_div(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec)
{
    uint32_t status = 0;

    /* Any divide by zero should throw, even if op1 is NAN. */
    if (UNEXPECTED(mpd_iszero(op2))) {
        php_decimal_division_by_zero_error();
    }

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qdiv(res, op1, op2, SHARED_CONTEXT, &status);
    });

    /* 0/INF will be 0 with a very small EXP - normalize this to 0E+0. */
    if (UNEXPECTED(mpd_isspecial(op2) && mpd_iszero(res))) {
        php_decimal_set_exp(res, 0);
    }
}

void php_decimal_pow(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec)
{
    uint32_t status = 0;

    if (mpd_iszero(op2)) {
        php_decimal_set_long(res, 1);
        return;
    }

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qpow(res, op1, op2, SHARED_CONTEXT, &status);
    });
}

void php_decimal_shiftl(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec)
{
    uint32_t status = 0;
    PHP_DECIMAL_TEMP_MPD(tmp);

    if (!mpd_isinteger(op2)) {
        php_decimal_operand_truncated_to_integer();
        mpd_qtrunc(&tmp, op2, MAX_CONTEXT, &status);
        op2 = &tmp;
    }

    mpd_qscaleb(res, op1, op2, MAX_CONTEXT, &status);
    mpd_del(&tmp);
}

void php_decimal_shiftr(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec)
{
    uint32_t status = 0;
    PHP_DECIMAL_TEMP_MPD(tmp);

    if (mpd_isinteger(op2)) {
        mpd_qcopy_negate(&tmp, op2, &status);
        op2 = &tmp;

    } else {
        php_decimal_operand_truncated_to_integer();
        mpd_qtrunc(&tmp, op2, MAX_CONTEXT, &status);
        mpd_negate(&tmp, res, &status);
        op2 = &tmp;
    }

    mpd_qscaleb(res, op1, op2, MAX_CONTEXT, &status);
    mpd_del(&tmp);
}

void php_decimal_mod(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec)
{
    uint32_t status = 0;

    if (UNEXPECTED(mpd_iszero(op2))) {
        php_decimal_division_by_zero_error();
        php_decimal_set_inf(res, mpd_sign(op1));

    } else {
        PHP_DECIMAL_TEMP_MPD(a);
        PHP_DECIMAL_TEMP_MPD(b);

        PHP_DECIMAL_WITH_PRECISION(prec, {
            mpd_qtrunc(&a, op1, SHARED_CONTEXT, &status);
            mpd_qtrunc(&b, op2, SHARED_CONTEXT, &status);

            mpd_qrem(res, &a, &b, SHARED_CONTEXT, &status);
        });

        mpd_del(&a);
        mpd_del(&b);
    }
}

void php_decimal_rem(mpd_t *res, const mpd_t *op1, const mpd_t *op2, zend_long prec)
{
    uint32_t status = 0;

    if (UNEXPECTED(mpd_iszero(op2))) {
        php_decimal_division_by_zero_error();
        php_decimal_set_inf(res, mpd_sign(op1));
        return;
    }

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qrem(res, op1, op2, SHARED_CONTEXT, &status);
    });
}

void php_decimal_ln(mpd_t *res, const mpd_t *op1, zend_long prec)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qln(res, op1, SHARED_CONTEXT, &status);
    });
}

void php_decimal_log10(mpd_t *res, const mpd_t *op1, zend_long prec)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qlog10(res, op1, SHARED_CONTEXT, &status);
    });
}

void php_decimal_sqrt(mpd_t *res, const mpd_t *op1, zend_long prec)
{
    uint32_t status = 0;

    if (UNEXPECTED(mpd_isspecial(op1))) {
        mpd_qcopy(res, op1, &status);
        return;
    }

    if (mpd_isnegative(op1)) {
        php_decimal_set_nan(res);
        return;
    }

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qsqrt(res, op1, SHARED_CONTEXT, &status);
    });
}

void php_decimal_exp(mpd_t *res, const mpd_t *op1, zend_long prec)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qexp(res, op1, SHARED_CONTEXT, &status);
    });
}

/**
 * Sets res to the floor of op1, ie. rounded down towards negative infinity.
 */
void php_decimal_floor(mpd_t *res, const mpd_t *op1, zend_long prec)
{
    uint32_t status = 0;

    mpd_qfloor(res, op1, MAX_CONTEXT, &status);
}

/**
 * Sets res to the ceiling of op1, ie. rounded up towards positive infinity.
 */
void php_decimal_ceil(mpd_t *res, const mpd_t *op1, zend_long prec)
{
    uint32_t status = 0;

    mpd_qceil(res, op1, MAX_CONTEXT, &status);
}

/**
 * Sets res to the integer value of op1, ie. discard all digits behind the
 * decimal point. The result is guaranteed to be an integer, unless op1 is a
 * special number in which case it should be copied as is.
 */
void php_decimal_trunc(mpd_t *res, const mpd_t *op1, zend_long prec)
{
    uint32_t status = 0;

    mpd_qtrunc(res, op1, MAX_CONTEXT, &status);
}



/******************************************************************************/
/*                                RATIONAL                                    */
/******************************************************************************/

/**
 * Calculates the GCD of two unsigned long values. This is a lot faster than
 * decimal arithmetic when numbers are small enough to be native.
 *
 * https://en.wikipedia.org/wiki/Binary_GCD_algorithm#Iterative_version_in_C
 */
static zend_always_inline zend_ulong php_decimal_gcd_long(zend_ulong a, zend_ulong b)
{
    int shift;

    /* GCD(0,v) == v; GCD(u,0) == u, GCD(0,0) == 0 */
    if (a == 0) return b;
    if (b == 0) return a;

    /* Let shift := lg K, where K is the greatest power of 2 dividing both a and b. */
    for (shift = 0; ((a | b) & 1) == 0; shift++) {
        a >>= 1;
        b >>= 1;
    }

    while ((a & 1) == 0) {
        a >>= 1;
    }

    /* From here on, a is always odd. */
    do {
        /**
         * Remove all factors of 2 in b -- they are not common.
         *
         * Note: b is not zero, so while will terminate.
         */
        while ((b & 1) == 0) {
            b >>= 1;
        }

        /**
         * Now a and b are both odd. Swap if necessary so a <= b,
         * then set b = b - a (which is even). For bignums, the
         * swapping is just pointer movement, and the subtraction
         * can be done in-place.
         */
        if (a > b) {
            zend_ulong t = b;
                       b = a;
                       a = t;
        }

        /* Here b >= a. */
        b = b - a;

    } while (b != 0);

    /* restore common factors of 2 */
    return a << shift;
}

/**
 * Calculates the GCD of two mpd_t's.
 *
 * We can assume that both num and den are integers.
 */
static void php_decimal_gcd(mpd_t *gcd, const mpd_t *num, const mpd_t *den)
{
    uint32_t status = 0;

    zend_ulong lnum = mpd_qabs_uint(num, &status);
    zend_ulong lden = mpd_qabs_uint(den, &status);

    /* Use a faster  algorithm if both integer conversions were successful, */
    if (status == 0) {
        php_decimal_set_ulong(gcd, php_decimal_gcd_long(lnum, lden));

    /* Fall back to Euclidean algorithm for larger numbers. */
    /* TODO: Could we simplify partially then use the ulong algorithm? */
    } else {
        PHP_DECIMAL_TEMP_MPD(a);
        PHP_DECIMAL_TEMP_MPD(b);

        mpd_qreduce(&a, num, MAX_CONTEXT, &status);
        mpd_qreduce(&b, den, MAX_CONTEXT, &status);

        do {
            mpd_qcopy(gcd, &b,                   &status);
            mpd_qrem (&b,  &a,  &b, MAX_CONTEXT, &status);
            mpd_qcopy(&a,  gcd,                  &status);

        } while (!mpd_iszerocoeff(&b));

        mpd_del(&a);
        mpd_del(&b);
    }
}

void php_decimal_rational_simplify(mpd_t *num, mpd_t *den)
{
    if (UNEXPECTED(mpd_isspecial(num) || mpd_isspecial(den))) {

        /* NAN in num or den is NAN */
        if (mpd_isnan(num) || mpd_isnan(den)) {
            php_decimal_set_nan(num);
            php_decimal_set_one(den);
            return;
        }

        /* Dividing antyhing by INF is 0, unless the numerator is special. */
        if (mpd_isinfinite(den)) {
            if (mpd_isspecial(num)) {
                php_decimal_set_nan(num);
                php_decimal_set_one(den);
                return;
            }

            /* The result is 0, respecting the sign of the op. */
            mpd_set_sign(num, mpd_sign(num) ^ mpd_sign(den));
            php_decimal_set_zero(num);
            php_decimal_set_one(den);
            return;
        }

    } else {
        uint32_t status = 0;

        /* We only need to simplify if the rational is not an integer. */
        /* This is the primary bottleneck for rational numbers. */
        if ( ! php_decimal_is_one(den)) {

            /* Calculate the GCD and simplify.  */
            PHP_DECIMAL_TEMP_MPD(gcd);
            php_decimal_gcd(&gcd, num, den);

            mpd_qdivint(num, num, &gcd, MAX_CONTEXT, &status);
            mpd_qdivint(den, den, &gcd, MAX_CONTEXT, &status);

            mpd_del(&gcd);
        }

        /* Remove trailing zeroes. */
        mpd_qreduce(num, num, MAX_CONTEXT, &status);
        mpd_qreduce(den, den, MAX_CONTEXT, &status);

        /* Fix sign */
        mpd_set_sign(num, mpd_sign(num) ^ mpd_sign(den));
        mpd_set_positive(den);
    }
}

void php_decimal_rational_normalize(mpd_t *num, mpd_t *den)
{
    uint32_t status = 0;

    /* We can assume that the denominator is always an integer. */
    const zend_long shift = mpd_trail_zeros(num) - num->exp;

    if (shift > 0) {
        mpd_qshiftl(num, num, shift, &status);
        mpd_qshiftl(den, den, shift, &status);
    }
}

void php_decimal_rational_evaluate(mpd_t *res, const php_rational_t *obj, zend_long prec)
{
    uint32_t status = 0;

    const mpd_t *num = PHP_RATIONAL_NUM(obj);
    const mpd_t *den = PHP_RATIONAL_DEN(obj);

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qdiv(res, num, den, SHARED_CONTEXT, &status);
    });
}

void php_decimal_rational_from_mpd(mpd_t *num, mpd_t *den, const mpd_t *mpd)
{
    uint32_t status = 0;

    mpd_qcopy(num, mpd, &status);
    php_decimal_set_one(den);
    php_decimal_rational_normalize(num, den);
}

/**
 *
 */
int php_decimal_rational_parity(const php_rational_t *obj)
{
    if (mpd_isspecial(PHP_RATIONAL_NUM(obj))) {
        return 1;

    } else {
        int parity;
        uint32_t status = 0;

        PHP_DECIMAL_TEMP_MPD(quot);

        const mpd_t *num = PHP_RATIONAL_NUM(obj);
        const mpd_t *den = PHP_RATIONAL_DEN(obj);

        mpd_qdivint(&quot, num, den, SHARED_CONTEXT, &status);
        parity = mpd_isodd(&quot);

        mpd_del(&quot);
        return parity;
    }
}


void php_decimal_rabs(mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den)
{
    uint32_t status = 0;

    mpd_qcopy_abs(rnum, num, &status);
    mpd_qcopy    (rden, den, &status);
}

void php_decimal_rneg(mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den)
{
    uint32_t status = 0;

    mpd_negate(rnum, num, &status);
    mpd_qcopy (rden, den, &status);
}

void php_decimal_radd(
    mpd_t *rnum,
    mpd_t *rden,
    const mpd_t *num1,
    const mpd_t *den1,
    const mpd_t *num2,
    const mpd_t *den2
) {
    uint32_t status = 0;

    /* a/b + c/d = (a*d + b*c) / (b*d) */
    mpd_qmul(rden, den1, num2,       MAX_CONTEXT, &status); /* b*c       */
    mpd_qfma(rnum, num1, den2, rden, MAX_CONTEXT, &status); /* a*d + b*c */
    mpd_qmul(rden, den1, den2,       MAX_CONTEXT, &status); /* b*d       */

    php_decimal_rational_simplify(rnum, rden);
}

void php_decimal_rsub(
    mpd_t *rnum,
    mpd_t *rden,
    const mpd_t *num1,
    const mpd_t *den1,
    const mpd_t *num2,
    const mpd_t *den2
) {
    uint32_t status = 0;

    /* a/b - c/d = (a*d - b*c) / (b*d) */
    mpd_qmul  (rnum, den1, num2,       MAX_CONTEXT, &status); /* b*c          */
    mpd_negate(rnum, rnum,                          &status); /*              */
    mpd_qfma  (rnum, num1, den2, rnum, MAX_CONTEXT, &status); /* a*d - b*c    */
    mpd_qmul  (rden, den1, den2,       MAX_CONTEXT, &status); /* b*d          */

    php_decimal_rational_simplify(rnum, rden);
}

void php_decimal_rmul(
    mpd_t *rnum,
    mpd_t *rden,
    const mpd_t *num1,
    const mpd_t *den1,
    const mpd_t *num2,
    const mpd_t *den2
) {
    uint32_t status = 0;

    // Beta: instead of doing a cross multiplication, lets get the LCM first,
    // and scale only

    /* a/b * c/d = a*c / b*d */
    mpd_qmul(rnum, num1, num2, MAX_CONTEXT, &status); /* a*c */
    mpd_qmul(rden, den1, den2, MAX_CONTEXT, &status); /* b*d */

    php_decimal_rational_simplify(rnum, rden);
}

void php_decimal_rdiv(
    mpd_t *rnum,
    mpd_t *rden,
    const mpd_t *num1,
    const mpd_t *den1,
    const mpd_t *num2,
    const mpd_t *den2
) {
    if (mpd_iszero(num2)) {
        php_decimal_division_by_zero_error();
        php_decimal_set_inf(rnum, mpd_sign(num2));
        php_decimal_set_one(rden);
        return;
    }

    php_decimal_rmul(rnum, rden, num1, den1, den2, num2);
}

void php_decimal_rpow(
    mpd_t *rnum,
    mpd_t *rden,
    const mpd_t *num1,
    const mpd_t *den1,
    const mpd_t *num2, /* exponent num */
    const mpd_t *den2  /* exponent den */
) {
    uint32_t status = 0;

    /* Anything to the power of zero is 1 */
    if (mpd_iszero(num2)) {
        php_decimal_set_one(rnum);
        php_decimal_set_one(rden);
        return;
    }

    /* Anything to the power of NAN is NAN */
    if (UNEXPECTED(mpd_isnan(num2))) {
        php_decimal_set_nan(rnum);
        php_decimal_set_one(rden);
        return;
    }

    /**
     * NAN to the power of anything is NAN, and either INF or 0 depending on the
     * sign of the exponent when the base is INF.
     */
    if (UNEXPECTED(mpd_isspecial(num1))) {
        if (mpd_isnan(num1)) {
            php_decimal_set_nan(rnum);
            php_decimal_set_one(rden);
            return;
        }

        if (mpd_ispositive(num2)) {
            php_decimal_set_inf(rnum, MPD_POS);
            php_decimal_set_one(rden);
            return;
        }

        php_decimal_set_zero(rnum);
        php_decimal_set_one(rden);
        return;
    }

    /**
     * Real numbers.
     *
     * This algorithm follows a naive approach where the exponent is cast to an
     * integer, and both the numerator and denominator of the base is raised to
     * this power. Fractional powers are not possible because they can result in
     * irrational values - a warning is raised to notify the user of this.
     *
     * This does NOT break Decimal\Number's interface, because it enforces only
     * operator consistency and programming interface - not behaviour!
     *
     * Note: We can assume here that all rational components are integers, and
     *       that the sign is determined by the numerator.
     */
    {
        PHP_DECIMAL_TEMP_MPD(a);
        PHP_DECIMAL_TEMP_MPD(b);

        /* Truncate the exponent if it is not already an integer. */
        if ( ! php_decimal_is_one(den2)) {
            mpd_qdivint(&a, num2, den2, MAX_CONTEXT, &status);
            php_decimal_operand_truncated_to_integer();
            num2 = &a;
        }

        /**
         * A positive base to the power of a negative exponent is equal to the
         * reciprocal of the base, to the power of a positive exponent.
         */
        if (mpd_ispositive(num2)) {
            mpd_qpow  (rnum, num1, num2, MAX_CONTEXT, &status);
            mpd_qpow  (rden, den1, num2, MAX_CONTEXT, &status);

        } else {
            mpd_negate(rden, num2,                    &status);
            mpd_qpow  (rnum, den1, rden, MAX_CONTEXT, &status);
            mpd_qpow  (rden, num1, rden, MAX_CONTEXT, &status);
        }

        /* Simply the result, because we raised the denominator as well. */
        php_decimal_rational_simplify(rnum, rden);
        mpd_del(&a);
        mpd_del(&b);
    }
}

void php_decimal_rmod(
    mpd_t *rnum,
    mpd_t *rden,
    const mpd_t *num1,
    const mpd_t *den1,
    const mpd_t *num2,
    const mpd_t *den2
) {
    uint32_t status = 0;

    if (UNEXPECTED(mpd_iszero(num2))) {
        php_decimal_division_by_zero_error();
        php_decimal_set_inf(rnum, mpd_sign(num2));
        php_decimal_set_one(rden);
        return;
    }

    mpd_qdivint(rnum, num1, den1, MAX_CONTEXT, &status);
    mpd_qdivint(rden, num2, den2, MAX_CONTEXT, &status);

    mpd_qrem(rnum, rnum, rden, MAX_CONTEXT, &status);
    php_decimal_set_one(rden);
}

void php_decimal_rrem(
    mpd_t *rnum,
    mpd_t *rden,
    const mpd_t *num1,
    const mpd_t *den1,
    const mpd_t *num2,
    const mpd_t *den2
) {
    uint32_t status = 0;

    if (UNEXPECTED(mpd_iszero(num2))) {
        php_decimal_division_by_zero_error();
        php_decimal_set_inf(rnum, mpd_sign(num2));
        php_decimal_set_one(rden);
        return;
    }

    /* Cross-multiply numerators to normalize denominators. */
    mpd_qmul(rnum, num1, den2, MAX_CONTEXT, &status);
    mpd_qmul(rden, den1, num2, MAX_CONTEXT, &status);

    /* Calculate the numerator of the remainder, scale denoninators. */
    mpd_qrem(rnum, rnum, rden, MAX_CONTEXT, &status);
    mpd_qmul(rden, den1, den2, MAX_CONTEXT, &status);

    php_decimal_rational_simplify(rnum, rden);
}


void php_decimal_rshiftl(
    mpd_t *rnum,
    mpd_t *rden,
    const mpd_t *num1,
    const mpd_t *den1,
    const mpd_t *num2,
    const mpd_t *den2
) {
    uint32_t status = 0;

     /* We can assume that den2 is 1 - can only shift by integer */
    mpd_qdivint(rnum, num2, den2, MAX_CONTEXT, &status);
    mpd_qscaleb(rnum, num1, rnum, MAX_CONTEXT, &status);
    mpd_qcopy  (rden, den1,                    &status);

    php_decimal_rational_normalize(rnum, rden);
    php_decimal_rational_simplify(rnum, rden);
}

void php_decimal_rshiftr(
    mpd_t *rnum,
    mpd_t *rden,
    const mpd_t *num1,
    const mpd_t *den1,
    const mpd_t *num2,
    const mpd_t *den2
) {
    uint32_t status = 0;

    /* We can assume that den2 is 1 - can only shift by integer */
    mpd_qdivint(rnum, num2, den2, MAX_CONTEXT, &status);
    mpd_negate (rnum, rnum,                    &status);
    mpd_qscaleb(rnum, num1, rnum, MAX_CONTEXT, &status);
    mpd_qcopy  (rden, den1,                    &status);

    php_decimal_rational_normalize(rnum, rden);
    php_decimal_rational_simplify(rnum, rden);
}

void php_decimal_rfloor(mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den)
{
    uint32_t status = 0;

    if (mpd_isspecial(num) || php_decimal_is_one(den)) {
        mpd_qcopy(rnum, num, &status);
        mpd_qcopy(rden, den, &status);
        return;
    }

    /* Evaluate the rational to an integer. */
    mpd_qdivint(rnum, num, den, MAX_CONTEXT, &status);
    php_decimal_set_one(rden);

    /* Negative values should truncate toward negative infinity.  */
    if (mpd_isnegative(num)) {
        mpd_qsub_uint(rnum, rnum, 1, MAX_CONTEXT, &status);
    }
}

void php_decimal_rceil(mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den)
{
    uint32_t status = 0;

    if (mpd_isspecial(num) || php_decimal_is_one(den)) {
        mpd_qcopy(rnum, num, &status);
        mpd_qcopy(rden, den, &status);
        return;
    }

    /* Evaluate the rational to an integer. */
    mpd_qdivint(rnum, num, den, MAX_CONTEXT, &status);
    php_decimal_set_one(rden);

    /* Positive values should round up toward positive infinity.  */
    if (mpd_ispositive(num)) {
        mpd_qadd_uint(rnum, rnum, 1, MAX_CONTEXT, &status);
    }
}

void php_decimal_rtrunc(mpd_t *rnum, mpd_t *rden, const mpd_t *num, const mpd_t *den)
{
    uint32_t status = 0;

    if (mpd_isspecial(num) || php_decimal_is_one(den)) {
        mpd_qcopy(rnum, num, &status);
        mpd_qcopy(rden, den, &status);
        return;
    }

    /* Evaluate the rational to an integer. */
    mpd_qdivint(rnum, num, den, MAX_CONTEXT, &status);
    php_decimal_set_one(rden);
}
