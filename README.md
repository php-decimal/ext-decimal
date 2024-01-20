# PHP Decimal Extension

[![PECL](https://img.shields.io/badge/PECL-1.5.0-blue.svg)](https://pecl.php.net/package/decimal)

Correctly-rounded, arbitrary-precision decimal arithmetic for PHP

## Documentation

See https://php-decimal.github.io/

## Dependencies

- PHP 7 or 8
- [mpdecimal](http://www.bytereef.org/mpdecimal/download.html)

## Install

```
pecl install decimal
```

Windows users can find *.dll* files under [releases](https://github.com/php-decimal/ext-decimal/releases).

## Enable

You can do this temporarily using `php -dextension=decimal.so` or by adding `extension=decimal.so` to your INI. If you manage PHP with [phpbrew](https://github.com/phpbrew/phpbrew), you can use `phpbrew ext enable decimal`.

## Local development

Docker: `./develop.sh`

## Tests

```
php run-tests.php -P -q
```


