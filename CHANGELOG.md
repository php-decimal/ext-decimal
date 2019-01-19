# Change Log
All notable changes to this project will be documented in this file.
This project follows [Semantic Versioning](http://semver.org/).

## [2.0.0] - 2019-01-XX
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

## [1.0.1] - 2018-10-29
- Added docs and tests to package.xml @remicollet
- Smarter libmpdec lib check in config.m4 @remicollet

## [1.0.0] - 2018-10-28
- Initial public release, request for comments.
