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
#ifndef HAVE_PHP_DECIMAL_ARGINFO_H
#define HAVE_PHP_DECIMAL_ARGINFO_H

#include <php.h>
#include "number.h"
#include "decimal.h"
#include "rational.h"

/**
 *
 */
#define PHP_DECIMAL_ARGINFO_NAME(cls, name) php_decimal_arginfo_##cls##_##name

/**
 * Arginfo
 */
#define PHP_DECIMAL_ARGINFO(cls, name, required_num_args) \
    ZEND_BEGIN_ARG_INFO_EX(PHP_DECIMAL_ARGINFO_NAME(cls, name), 0, 0, required_num_args)

#if PHP_VERSION_ID >= 70200
#define PHP_DECIMAL_ARGINFO_RETURN_NUMBER(cls, name, required_num_args) \
    static const zend_internal_arg_info PHP_DECIMAL_ARGINFO_NAME(cls, name)[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), ZEND_TYPE_ENCODE_CLASS_CONST(PHP_DECIMAL_NUMBER_FQCN, 0), 0, 0},
#else
#define PHP_DECIMAL_ARGINFO_RETURN_NUMBER(cls, name, required_num_args) \
    static const zend_internal_arg_info PHP_DECIMAL_ARGINFO_NAME(cls, name)[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), PHP_DECIMAL_NUMBER_FQCN, IS_OBJECT, 0, 0, 0},
#endif

#if PHP_VERSION_ID >= 70200
#define PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(cls, name, required_num_args) \
    const zend_internal_arg_info PHP_DECIMAL_ARGINFO_NAME(cls, name)[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), ZEND_TYPE_ENCODE_CLASS_CONST(PHP_DECIMAL_DECIMAL_FQCN, 0), 0, 0},
#else
#define PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(cls, name, required_num_args) \
    const zend_internal_arg_info PHP_DECIMAL_ARGINFO_NAME(cls, name)[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), PHP_DECIMAL_DECIMAL_FQCN, IS_OBJECT, 0, 0, 0},
#endif

#if PHP_VERSION_ID >= 70200
#define PHP_DECIMAL_ARGINFO_RETURN_RATIONAL(cls, name, required_num_args) \
    const zend_internal_arg_info PHP_DECIMAL_ARGINFO_NAME(cls, name)[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), ZEND_TYPE_ENCODE_CLASS_CONST(PHP_DECIMAL_RATIONAL_FQCN, 0), 0, 0},
#else
#define PHP_DECIMAL_ARGINFO_RETURN_RATIONAL(cls, name, required_num_args) \
    const zend_internal_arg_info PHP_DECIMAL_ARGINFO_NAME(cls, name)[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), PHP_DECIMAL_RATIONAL_FQCN, IS_OBJECT, 0, 0, 0},
#endif
/*
#if PHP_VERSION_ID >= 70200
#if PHP_VERSION_ID >= 70200
#define PHP_DECIMAL_ARGINFO_RETURN_NUMBER(cls, name, required_num_args) \
    static const zend_internal_arg_info PHP_DECIMAL_ARGINFO_NAME(cls, name)[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), ZEND_TYPE_ENCODE_CLASS_CONST(PHP_DECIMAL_RATIONAL_FQCN, 0), 0, 0},
#else
#define PHP_DECIMAL_ARGINFO_RETURN_NUMBER(cls, name, required_num_args) \
    static const zend_internal_arg_info PHP_DECIMAL_ARGINFO_NAME(cls, name)[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), PHP_DECIMAL_RATIONAL_FQCN, IS_OBJECT, 0, 0, 0},
#endif
*/
#if PHP_VERSION_ID >= 70200
#define PHP_DECIMAL_ARGINFO_RETURN_TYPE(cls, name, type, required_num_args) \
    ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(PHP_DECIMAL_ARGINFO_NAME(cls, name), 0, required_num_args, type, 0)
#else
#define PHP_DECIMAL_ARGINFO_RETURN_TYPE(cls, name, type, required_num_args) \
    ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(PHP_DECIMAL_ARGINFO_NAME(cls, name), 0, required_num_args, type, 0, 0)
#endif



/*
#define PHP_DECIMAL_ARGINFO_RETURN_TYPE(cls, name, type, required_num_args) \
    PHP_DECIMAL_ARGINFO_RETURN_TYPE_EX(cls, name, type, required_num_args)

#define PHP_DECIMAL_FUNCTION_ARGINFO(name, required_num_args) \
    PHP_DECIMAL_ARGINFO_EX(function, name, required_num_args)

#define PHP_DECIMAL_FUNCTION_ARGINFO_RETURN_DECIMAL(name, required_num_args) \
    PHP_DECIMAL_ARGINFO_RETURN_NUMBER(function, name, required_num_args)

#define PHP_DECIMAL_FUNCTION_ARGINFO_RETURN_RATIONAL(name, required_num_args) \
    PHP_DECIMAL_ARGINFO_RETURN_NUMBER(function, name, required_num_args)

#define PHP_DECIMAL_FUNCTION_ARGINFO_RETURN_TYPE(name, type, required_num_args) \
    PHP_DECIMAL_ARGINFO_RETURN_TYPE_EX(function, name, type, required_num_args)
*/


#define PHP_DECIMAL_ARGINFO_ZVAL(name) \
    ZEND_ARG_INFO(0, name)

#define PHP_DECIMAL_ARGINFO_OPTIONAL_ZVAL(name) \
    ZEND_ARG_TYPE_INFO(0, name, 0, 1) \

#define PHP_DECIMAL_ARGINFO_LONG(name) \
    ZEND_ARG_TYPE_INFO(0, name, IS_LONG, 0)

#define PHP_DECIMAL_ARGINFO_FLOAT(name) \
    ZEND_ARG_TYPE_INFO(0, name, IS_DOUBLE, 0)

#define PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(name) \
    ZEND_ARG_TYPE_INFO(0, name, IS_LONG, 1)

#define PHP_DECIMAL_ARGINFO_OPTIONAL_BOOL(name) \
    ZEND_ARG_TYPE_INFO(0, name, _IS_BOOL, 1)

#define PHP_DECIMAL_ARGINFO_NUMBER(name) \
    ZEND_ARG_OBJ_INFO(0, name, Decimal\\Number, 0)

#define PHP_DECIMAL_ARGINFO_END() \
    ZEND_END_ARG_INFO()

#endif
