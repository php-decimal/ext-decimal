# PHP Decimal Extension


[![PECL](https://img.shields.io/static/v1?message=1.3.1&label=PECL&color=blue&style=flat&logo=php)](https://pecl.php.net/package/decimal/1.3.1)
[![Build Status](https://travis-ci.org/php-decimal/ext-decimal.svg?branch=1.3)](https://travis-ci.org/php-decimal/ext-decimal)

[![PECL](https://img.shields.io/static/v1?message=2.0.0-alpha&label=PECL&color=blue&style=flat&logo=php)](https://pecl.php.net/package/decimal/2.0.0)
[![Build Status](https://travis-ci.org/php-decimal/ext-decimal.svg?branch=2.0&style=flat)](https://travis-ci.org/php-decimal/ext-decimal)
[![Build status](https://ci.appveyor.com/api/projects/status/lg5nw5tqgpmv1c33?svg=true)](https://ci.appveyor.com/project/rtheunissen/php-decimal)

Correctly-rounded, arbitrary-precision decimal arithmetic for PHP

## Documentation

See http://php-decimal.io

## Requirements

- PHP >= 7.2
- [libmpdec >= 2.4](http://www.bytereef.org/mpdecimal/download.html)

## Install

```
pecl install decimal
```

**Alpha**:

```
pecl install decimal-alpha
```

Windows users can find *.dll* files under [releases](https://github.com/php-decimal/ext-decimal/releases).

## Enable

You can do this temporarily using `php -dextension=decimal.so` or by adding `extension=decimal.so` to your INI. If you manage PHP with [phpbrew](https://github.com/phpbrew/phpbrew), you can use `phpbrew ext enable decimal`.

## Tests

```
php run-tests.php -P -q
```
