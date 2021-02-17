# PHP Decimal Extension

[![Build Status](https://travis-ci.org/php-decimal/ext-decimal.svg?branch=master)](https://travis-ci.org/php-decimal/ext-decimal)
[![Build status](https://ci.appveyor.com/api/projects/status/lg5nw5tqgpmv1c33?svg=true)](https://ci.appveyor.com/project/rtheunissen/php-decimal)
[![PECL](https://img.shields.io/badge/PECL-1.4.0-blue.svg)](https://pecl.php.net/package/decimal)

Correctly-rounded, arbitrary-precision decimal arithmetic for PHP

## Documentation

See http://php-decimal.io

## Dependencies

- PHP 7
- [mpdecimal](http://www.bytereef.org/mpdecimal/download.html)

## Install

```
pecl install decimal
```

Windows users can find *.dll* files under [releases](https://github.com/php-decimal/ext-decimal/releases).

## Enable

You can do this temporarily using `php -dextension=decimal.so` or by adding `extension=decimal.so` to your INI. If you manage PHP with [phpbrew](https://github.com/phpbrew/phpbrew), you can use `phpbrew ext enable decimal`.

## Tests

```
php run-tests.php -P -q
```


