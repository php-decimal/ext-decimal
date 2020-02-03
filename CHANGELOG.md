# Change Log
All notable changes to this project will be documented in this file.
This project follows [Semantic Versioning](http://semver.org/).

## [2.0.0] - 2020-02-03
- Default decimal precision changed from 28 to 34 to match [Decimal128](https://en.wikipedia.org/wiki/Decimal128_floating-point_format).
- Decimal operations now use the **minimum** precision of all operands, rather than the maximum.
- Remove `Decimal\Decimal::equals` method.
- Added `Decimal\Rational` implementation for rational numbers (fractions).
- Added `Decimal\Number` abstract number class.
- Added `Decimal\Precision` interface.
- Added `Decimal\Decimal::reduce` method.
- Better consistency around scientific notation when converting to string or JSON.
- Precision is now applied on operations only (input is preserved).
- Decimal constructors are now private - must use `\Decimal\Number::valueOf`.

## [1.3.0] - 2019-02-11
- Fixed sqrt of -INF returning -INF rather than NAN. #13
- Disable opcache pass 2 due to numeric string to float casts. Thanks @krakjoe

## [1.2.0] - 2019-02-09
- Change signum of NAN returning 1, now throws RuntimeException. #10
- Change toInt of NAN and INF returning 0, now throws RuntimeException. #11
- Fix pthreads incompatibility (properly). #12

## [1.1.2] - 2019-02-07
- Fix pthreads incompatibility. #12

## [1.1.1] - 2019-01-24
- Fix `signum` returning 1 for zero, instead of 0. #9

## [1.1.0] - 2018-11-20
- Added `trim` method to Decimal to trim trailing zeroes.

## [1.0.1] - 2018-10-29
- Added docs and tests to package.xml @remicollet
- Smarter libmpdec lib check in config.m4 @remicollet

## [1.0.0] - 2018-10-28
- Initial public release, request for comments.
