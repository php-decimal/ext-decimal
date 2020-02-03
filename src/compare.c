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
#include <Zend/zend_interfaces.h>
#include <mpdecimal.h>
#include "common.h"
#include "compare.h"
#include "context.h"
#include "convert.h"
#include "limits.h"
#include "math.h"
#include "parse.h"


/**
 * Attempt to compare op1 and op2. It is possible that the comparison is not
 * defined (when comparing to NAN), so we should return a special comparison
 * result that indicates an undefined result. Returning 1 here is no good
 * because operations like "greater than" would be true for NAN.
 */
static int php_decimal_compare_mpd_to_mpd(const mpd_t *op1, const mpd_t *op2)
{
    uint32_t status = 0;

    int result = mpd_qcmp(op1, op2, &status);

    if (UNEXPECTED(status & MPD_Invalid_operation)) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    }

    return result;
}

/**
 *
 */
static int php_decimal_compare_double_to_double(const double op1, const double op2)
{
    if (UNEXPECTED(zend_isnan(op1) || zend_isnan(op2))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    } else {
        return PHP_DECIMAL_COMPARE(op1, op2);
    }
}

/**
 *
 */
static int php_decimal_compare_long_to_long(const zend_long op1, const zend_long op2)
{
    return PHP_DECIMAL_COMPARE(op1, op2);
}











/**
 * COMPARE TO DOUBLE
 */

/**
 *
 */
static int php_decimal_compare_mpd_to_double(const mpd_t *op1, const double op2)
{
    return php_decimal_compare_double_to_double(php_decimal_mpd_to_double(op1), op2);
}

/**
 *
 */
static int php_decimal_compare_decimal_to_double(const php_decimal_t *op1, const double op2)
{
    return php_decimal_compare_mpd_to_double(PHP_DECIMAL_MPD(op1), op2);
}

/**
 *
 */
static int php_decimal_compare_rational_to_double(const php_rational_t *op1, const double op2)
{
    return php_decimal_compare_double_to_double(php_decimal_rational_to_double(op1), op2);
}   

/**
 *
 */
static int php_decimal_compare_number_to_double(const zval *op1, const double op2)
{
    return php_decimal_compare_double_to_double(php_decimal_number_to_double(op1), op2);
}


/**
 * COMPARE TO LONG
 */

/**
 *
 */
static int php_decimal_compare_mpd_to_long(const mpd_t *op1, const zend_long op2)
{
    return php_decimal_compare_mpd_to_double(op1, (double) op2);
}

/**
 *
 */
static int php_decimal_compare_decimal_to_long(const php_decimal_t *op1, const zend_long op2)
{
    return php_decimal_compare_mpd_to_long(PHP_DECIMAL_MPD(op1), op2); 
}

/**
 *
 */
static int php_decimal_compare_rational_to_long(const php_rational_t *op1, const zend_long op2)
{
    return php_decimal_compare_rational_to_double(op1, (double) op2);
}

/**
 *
 */
static int php_decimal_compare_number_to_long(const zval *op1, const zend_long op2)
{
    return php_decimal_compare_number_to_double(op1, (double) op2);
}

/**
 * COMPARE TO STRING
 */

/**
 *
 */
static int php_decimal_compare_mpd_to_string(const mpd_t *op1, const zend_string *op2)
{
    int result;

    /* */
    if (UNEXPECTED(mpd_isnan(op1))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    }

    /* */
    PHP_DECIMAL_TEMP_MPD(tmp);
    if (php_decimal_mpd_set_string(&tmp, op2) == SUCCESS) {
        result = php_decimal_compare_mpd_to_mpd(op1, &tmp);
    } else {
        result = PHP_DECIMAL_COMPARISON_FAILURE;
    }

    mpd_del(&tmp);
    return result;
}

/**
 *
 */
static int php_decimal_compare_decimal_to_string(const php_decimal_t *op1, const zend_string *op2)
{
    return php_decimal_compare_mpd_to_string(PHP_DECIMAL_MPD(op1), op2);
}

/**
 *
 */
static int php_decimal_compare_rational_to_string(const php_rational_t *op1, const zend_string *op2)
{
    if (UNEXPECTED(php_decimal_rational_is_nan(op1))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;

    } else {
        int result;
        PHP_DECIMAL_TEMP_MPD(tmp);

        php_decimal_rational_evaluate(&tmp, op1, ZSTR_LEN(op2) + 10);
        result = php_decimal_compare_mpd_to_string(&tmp, op2);

        mpd_del(&tmp);
        return result;
    }
}

/**
 *
 */
static int php_decimal_compare_number_to_string(const zval *op1, const zend_string *op2)
{
    if (UNEXPECTED(php_decimal_number_is_nan(op1))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    
    } else {
        int result;

        zval str;
        php_decimal_number_to_string(&str, op1);
        
        /* */
        if (UNEXPECTED(EG(exception))) {
            result = PHP_DECIMAL_COMPARISON_FAILURE;
        } else {
            result = zendi_smart_strcmp(Z_STR(str), (zend_string *) op2);
        }

        zval_ptr_dtor(&str);
        return result;
    }
}



/**
 * COMPARE TO OBJECT
 */

/**
 *
 */
static int php_decimal_compare_invoke(const zval *obj, const zval *op2)
{
    zval cmp;
    int result;
    assert(Z_IS_DECIMAL_NUMBER_P(op2));

    /* */
    assert(!php_decimal_number_is_nan(obj));
    if (UNEXPECTED(php_decimal_number_is_nan(op2))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    }

    /* */
    zend_call_method_with_1_params((zval *) obj, Z_OBJCE_P(obj), NULL, "compareto", &cmp, (zval *) op2);
    
    /* */
    if (UNEXPECTED(EG(exception))) {
        result = PHP_DECIMAL_COMPARISON_FAILURE;
    } else {
        result = ZEND_NORMALIZE_BOOL(Z_LVAL(cmp));
    }

    zval_ptr_dtor(&cmp);
    return result;
}

/**
 *
 */
static int php_decimal_compare_precision(const php_decimal_t *op1, const php_decimal_t *op2)
{
    const zend_long a = php_decimal_get_prec(op1);
    const zend_long b = php_decimal_get_prec(op2);

    return PHP_DECIMAL_COMPARE(a, b);
}

/**
 * Compares two decimals using value-only comparison, precision is ignored.
 */
static int php_decimal_compare_decimal_to_decimal(const php_decimal_t *op1, const php_decimal_t *op2)
{
    const mpd_t *a = PHP_DECIMAL_MPD(op1);
    const mpd_t *b = PHP_DECIMAL_MPD(op2);

    int result = php_decimal_compare_mpd_to_mpd(a, b);

    /* */
    if (result == 0) {
        return php_decimal_compare_precision(op1, op2);
    }

    return result;
}

/**
 *
 */
static int php_decimal_compare_mpd_to_rational(const mpd_t *op1, const php_rational_t *op2)
{
    int result;
    PHP_DECIMAL_TEMP_MPD(tmp);

    uint32_t status = 0;

    const mpd_t *num = PHP_RATIONAL_NUM(op2);
    const mpd_t *den = PHP_RATIONAL_DEN(op2);

    /* */
    assert(!mpd_isnan(op1));
    if (UNEXPECTED(mpd_isnan(num))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    }

    PHP_DECIMAL_WITH_PRECISION(op1->digits, {
        mpd_qmul(&tmp, op1, den, SHARED_CONTEXT, &status);
    });

    result = php_decimal_compare_mpd_to_mpd(&tmp, num);

    mpd_del(&tmp);
    return result;
}

/**
 *
 */
static int php_decimal_compare_decimal_to_rational(const php_decimal_t *op1, const php_rational_t *op2)
{
    return php_decimal_compare_mpd_to_rational(PHP_DECIMAL_MPD(op1), op2);
}

/**
 *
 */
static int php_decimal_compare_decimal_to_number(const php_decimal_t *op1, const zval *op2)
{
    int result;
    PHP_DECIMAL_TEMP_MPD(tmp);

    php_decimal_number_to_mpd(&tmp, op2, php_decimal_get_prec(op1));
    result = php_decimal_compare_mpd_to_mpd(PHP_DECIMAL_MPD(op1), &tmp);
    
    mpd_del(&tmp);
    return result;
}

/**
 *
 */
static int php_decimal_compare_decimal_to_object(const php_decimal_t *op1, const zval *op2)
{
    if (UNEXPECTED(mpd_isnan(PHP_DECIMAL_MPD(op1)))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    }

    if (Z_OBJ_IS_DECIMAL_P(op2)) {
        return php_decimal_compare_decimal_to_decimal(op1, Z_DECIMAL_P(op2));
    }

    if (Z_OBJ_IS_RATIONAL_P(op2)) {
        return php_decimal_compare_decimal_to_rational(op1, Z_RATIONAL_P(op2));
    }

    if (Z_OBJ_IS_NUMBER_P(op2)) {
        return php_decimal_compare_decimal_to_number(op1, op2);
    }

    return PHP_DECIMAL_COMPARISON_FAILURE; // TODO we should throw here?
}


/**
 *
 */
static int php_decimal_compare_rational_to_mpd(const php_rational_t *op1, const mpd_t *op2)
{
    int result;
    PHP_DECIMAL_TEMP_MPD(tmp);

    uint32_t status = 0;

    const mpd_t *num = PHP_RATIONAL_NUM(op1);
    const mpd_t *den = PHP_RATIONAL_DEN(op1);

    /* */
    if (UNEXPECTED(mpd_isnan(num) || mpd_isnan(op2))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    }

    PHP_DECIMAL_WITH_PRECISION(op2->digits, {
        mpd_qmul(&tmp, op2, den, SHARED_CONTEXT, &status);
    });

    result = php_decimal_compare_mpd_to_mpd(num, &tmp);

    mpd_del(&tmp);
    return result;
}

/**
 *
 */
static int php_decimal_compare_rational_to_decimal(const php_rational_t *op1, const php_decimal_t *op2)
{
    return php_decimal_compare_rational_to_mpd(op1, PHP_DECIMAL_MPD(op2));
}

/**
 *
 */
static int php_decimal_compare_rational_to_rational(const php_rational_t *op1, const php_rational_t *op2)
{
    int result;

    PHP_DECIMAL_TEMP_MPD(a);
    PHP_DECIMAL_TEMP_MPD(b);

    const mpd_t *num1 = PHP_RATIONAL_NUM(op1);
    const mpd_t *den1 = PHP_RATIONAL_DEN(op1);
    const mpd_t *num2 = PHP_RATIONAL_NUM(op2);
    const mpd_t *den2 = PHP_RATIONAL_DEN(op2);

    uint32_t status = 0;

    /* */
    if (UNEXPECTED(mpd_isnan(num1) || mpd_isnan(num2))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    }

    mpd_qmul(&a, num1, den2, MAX_CONTEXT, &status);
    mpd_qmul(&b, num2, den1, MAX_CONTEXT, &status);

    result = php_decimal_compare_mpd_to_mpd(&a, &b);

    mpd_del(&a);
    mpd_del(&b);
    return result;
}

/**
 *
 */
static int php_decimal_compare_num_den(const mpd_t *num1, const mpd_t *den1, const mpd_t *num2, const mpd_t *den2)
{
    int result;
    PHP_DECIMAL_TEMP_MPD(a);
    PHP_DECIMAL_TEMP_MPD(b);

    uint32_t status = 0;

    mpd_qmul(&a, num1, den2, MAX_CONTEXT, &status);
    mpd_qmul(&b, num2, den1, MAX_CONTEXT, &status);

    result = php_decimal_compare_mpd_to_mpd(&a, &b);

    mpd_del(&a);
    mpd_del(&b);
    return result;
}

/**
 *
 */
static int php_decimal_compare_rational_to_number(const php_rational_t *op1, const zval *op2)
{
    int result;
    PHP_DECIMAL_TEMP_MPD(num);
    PHP_DECIMAL_TEMP_MPD(den);

    /* */
    assert(Z_IS_DECIMAL_NUMBER_P(op2));
    php_decimal_number_to_num_den(&num, &den, op2);

    /* */
    result = php_decimal_compare_num_den(
        PHP_RATIONAL_NUM(op1), 
        PHP_RATIONAL_DEN(op1), 
        &num, 
        &den
    );
    
    mpd_del(&num);
    mpd_del(&den);
    return result;
}

/**
 *
 */
static int php_decimal_compare_rational_to_object(const php_rational_t *op1, const zval *op2)
{
    if (Z_OBJ_IS_DECIMAL_P(op2)) {
        return php_decimal_compare_rational_to_decimal(op1, Z_DECIMAL_P(op2));
    }

    if (Z_OBJ_IS_RATIONAL_P(op2)) {
        return php_decimal_compare_rational_to_rational(op1, Z_RATIONAL_P(op2));
    }

    if (Z_OBJ_IS_NUMBER_P(op2)) {
        return php_decimal_compare_rational_to_number(op1, op2);
    }

    return PHP_DECIMAL_COMPARISON_FAILURE; // TODO we should throw here.
}

/**
 *
 */
static int php_decimal_compare_number_to_number(const zval *obj, const zval *op2)
{
    return php_decimal_compare_invoke(obj, op2);
}

/**
 *
 */
static int php_decimal_compare_number_to_object(const zval *obj, const zval *op2)
{
    if (UNEXPECTED(php_decimal_number_is_nan(obj))) {
        return PHP_DECIMAL_COMPARISON_UNDEFINED;
    }

    if (EXPECTED(Z_OBJ_IS_NUMBER_P(op2))) {
        return php_decimal_compare_number_to_number(obj, op2);
    }

    return PHP_DECIMAL_COMPARISON_FAILURE;
}




// /**
//  *
//  */
// static int php_decimal_compare_rational_to_double(const php_rational_t *op1, const double op2)
// {
//     int result;
//     PHP_DECIMAL_TEMP_MPD(num);
//     PHP_DECIMAL_TEMP_MPD(den);

//     const mpd_t *num1 = PHP_RATIONAL_NUM(op1);
//     const mpd_t *den1 = PHP_RATIONAL_DEN(op1);
//     const mpd_t *num2 = &num;
//     const mpd_t *den2 = &den;

//     php_decimal_mpd_set_double(&num, op2);
//     php_decimal_mpd_set_one(&den);
//     php_decimal_rational_normalize(&num, &den);

//     result = php_decimal_compare_rational_num_den(num1, den1, num2, den2);

//     mpd_del(&num);
//     mpd_del(&den);
//     return result;
// }




// static int php_decimal_compare_number_to_numeric(const zval *obj, const zval *op2)
// {
//     zval tmp;
//     zend_call_method_with_1_params(NULL, Z_OBJCE_P(obj), NULL, "valueof", &tmp, op2);

//     // int result;
//     // PHP_DECIMAL_TEMP_MPD(tmp);

//     // if (php_decimal_parse_scalar_quiet(&tmp, op2) == FAILURE) {
//     //     return PHP_DECIMAL_COMPARISON_FAILURE;
//     // }

//     // result = php_decimal_compare_mpd_to_mpd(op1, &tmp);

//     // mpd_del(&tmp);
//     // return result;
// }

// /**
//  *
//  */
// static int php_decimal_compare_number_to_scalar(const zval *obj, const zval *op2)
// {
//     switch (Z_TYPE_P(op2)) {
//         case IS_NULL:
//         case IS_FALSE:
//             return 1;

//         case IS_TRUE:
//             return 0;

//          Allow comparing to float. 
//         case IS_DOUBLE: 
//         case IS_LONG: 
//             return php_decimal_compare_number_to_numeric(obj, Z_LVAL_P(op2));

//         default: 
//             php_decimal_comparison_is_not_defined();
//             return PHP_DECIMAL_COMPARISON_UNDEFINED;

//     }
// }

/**
 *
 */
static zend_bool php_decimal_between_cmp(const int a, const int b, const zend_bool inclusive)
{
    /* */
    if (UNEXPECTED(
        a == PHP_DECIMAL_COMPARISON_UNDEFINED || 
        b == PHP_DECIMAL_COMPARISON_UNDEFINED))
        return false;
    

    return inclusive
        ? (a >= 0 && b <= 0) || (a <= 0 && b >= 0)
        : (a >  0 && b <  0) || (a <  0 && b >  0);
} 







int php_decimal_compare(const php_decimal_t *op1, const zval *op2)
{
    switch (Z_TYPE_P(op2)) {
        case IS_NULL:
        case IS_FALSE:
            return 1;
        case IS_TRUE:
            return 0;
        case IS_LONG:
            return php_decimal_compare_decimal_to_long(op1, Z_LVAL_P(op2));
        case IS_DOUBLE: 
            return php_decimal_compare_decimal_to_double(op1, Z_DVAL_P(op2));
        case IS_STRING: 
            return php_decimal_compare_decimal_to_string(op1, Z_STR_P(op2));
        case IS_OBJECT:
            return php_decimal_compare_decimal_to_object(op1, op2);
        default:
            return PHP_DECIMAL_COMPARISON_FAILURE;
    }
}

int php_decimal_rational_compare(const php_rational_t *op1, const zval *op2)
{
    switch (Z_TYPE_P(op2)) {
        case IS_NULL:
        case IS_FALSE:
            return 1;
        case IS_TRUE:
            return 0;
        case IS_LONG:
            return php_decimal_compare_rational_to_long(op1, Z_LVAL_P(op2));
        case IS_DOUBLE: 
            return php_decimal_compare_rational_to_double(op1, Z_DVAL_P(op2));
        case IS_STRING: 
            return php_decimal_compare_rational_to_string(op1, Z_STR_P(op2));
        case IS_OBJECT:
            return php_decimal_compare_rational_to_object(op1, op2);
        default:
            return PHP_DECIMAL_COMPARISON_FAILURE;
    }
}

int php_decimal_number_compare(const zval *obj, const zval *op2)
{
    switch (Z_TYPE_P(op2)) {
        case IS_NULL:
        case IS_FALSE:
            return 1;
        case IS_TRUE:
            return 0;
        case IS_LONG:
            return php_decimal_compare_number_to_long(obj, Z_LVAL_P(op2));
        case IS_DOUBLE: 
            return php_decimal_compare_number_to_double(obj, Z_DVAL_P(op2));
        case IS_STRING: 
            return php_decimal_compare_number_to_string(obj, Z_STR_P(op2));
        case IS_OBJECT:
            return php_decimal_compare_number_to_object(obj, op2);
        default:
            return PHP_DECIMAL_COMPARISON_FAILURE;
    }
}

    



    // // if (UNEXPECTED(result == PHP_DECIMAL_COMPARISON_UNDEFINED)) {
    // //     return result;
    // // }

    // // return invert ? result * -1 : result;


    // // int       result;
    // // zend_bool invert;



    // /* */
    // if (Z_IS_DECIMAL_P(op1)) {
    //     result = php_decimal_compare_decimal_to_zval(Z_DECIMAL_P(op1), op2);
    //     invert = false;

    // /* */
    // } else if (Z_IS_RATIONAL_P(op1)) {
    //     result = php_decimal_compare_rational_to_zval(Z_RATIONAL_P(op1), op2);
    //     invert = false;

    // /* */
    // } else if (Z_IS_DECIMAL_NUMBER_P(op1)) {
    //     result = php_decimal_number_compare_to_number(op1, op2);
    //     invert = false;

    // /* */
    // } else if (Z_IS_DECIMAL_P(op2)) {
    //     result = php_decimal_compare_decimal_to_zval(Z_DECIMAL_P(op2), op1);
    //     invert = true;

    // /* */
    // } else if (Z_IS_RATIONAL_P(op2)) {
    //     result = php_decimal_compare_rational_to_zval(Z_RATIONAL_P(op2), op1);
    //     invert = true;

    // /* */
    // } else {
    //     result = php_decimal_number_compare_to_number(op2, op1);
    //     invert = true;
    // }

    // /* */
    // if (UNEXPECTED(result == PHP_DECIMAL_COMPARISON_UNDEFINED)) {
    //     return result;
    // } else {
    //     return invert ? result * -1 : result;
    // }
// }


zend_bool php_decimal_between(const php_decimal_t *obj, const zval *op1, const zval *op2, const zend_bool inclusive)
{
    return php_decimal_between_cmp(
        php_decimal_compare(obj, op1),
        php_decimal_compare(obj, op2),
        inclusive
    );
}

zend_bool php_decimal_rational_between(const php_rational_t *obj, const zval *op1, const zval *op2, const zend_bool inclusive)
{
    return php_decimal_between_cmp(
        php_decimal_rational_compare(obj, op1),
        php_decimal_rational_compare(obj, op2),
        inclusive
    );
}

zend_bool php_decimal_number_between(const zval *obj, const zval *op1, const zval *op2, const zend_bool inclusive)
{
    if (Z_OBJ_IS_DECIMAL_P(obj)) {
        return php_decimal_between(Z_DECIMAL_P(obj), op1, op2, inclusive);
    }

    if (Z_OBJ_IS_RATIONAL_P(obj)) {
        return php_decimal_rational_between(Z_RATIONAL_P(obj), op1, op2, inclusive);
    }

    return php_decimal_between_cmp(
        php_decimal_number_compare(obj, op1),
        php_decimal_number_compare(obj, op2),
        inclusive
    );
}

zend_bool php_decimal_number_equals(const zval *op1, const zval *op2)
{
    if (Z_OBJ_IS_DECIMAL_P(op1)) {
        return php_decimal_compare(Z_DECIMAL_P(op1), op2) == 0;
    }

    if (Z_OBJ_IS_RATIONAL_P(op1)) {
        return php_decimal_rational_compare(Z_RATIONAL_P(op1), op2) == 0;
    }

    return php_decimal_number_compare(op1, op2) == 0;
}
