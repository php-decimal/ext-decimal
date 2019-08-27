--TEST--
Number::div
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number divided by scalar using operator */
print_r(Number::valueOf(4) / "2.5");

/* Scalar divided by Number using operator */
print_r("2.5" / Number::valueOf(4));

/* Number divided by Number using operator */
print_r(Number::valueOf(4) / Number::valueOf("2.5"));

/* Number divided by Number using method */
print_r(Number::valueOf(4)->div(Number::valueOf("2.5")));

/* Number divided by Decimal using operator */
print_r(Number::valueOf(4) / Decimal::valueOf("2.5"));

/* Number divided by Decimal using method */
print_r(Number::valueOf(4)->div(Decimal::valueOf("2.5")));

/* Number divided by Rational using operator */
print_r(Number::valueOf(4) / Rational::valueOf("2.5"));

/* Number divided by Rational using method */
print_r(Number::valueOf(4)->div(Rational::valueOf("2.5")));

/* Decimal divided by Number using operator */
print_r(Decimal::valueOf(4) / Number::valueOf("2.5"));

/* Decimal divided by Number using method */
print_r(Decimal::valueOf(4)->div(Number::valueOf("2.5")));

/* Rational divided by Number using operator */
print_r(Rational::valueOf(4) / Number::valueOf("2.5"));

/* Rational divided by Number using method */
print_r(Rational::valueOf(4)->div(Number::valueOf("2.5")));

?>
--EXPECT--
Number::div
Number Object
(
    [value:protected] => 1.6
)
Number::div
Number Object
(
    [value:protected] => 0.625
)
Number::div
Number Object
(
    [value:protected] => 1.6
)
Number::div
Number Object
(
    [value:protected] => 1.6
)
Number::div
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 1.6
            [precision] => 34
        )

)
Number::div
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 1.6
            [precision] => 34
        )

)
Number::div
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 8
            [den] => 5
        )

)
Number::div
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 8
            [den] => 5
        )

)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 1.6
    [precision] => 34
)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 1.6
    [precision] => 34
)
Number::toRational
Decimal\Rational Object
(
    [num] => 8
    [den] => 5
)
Number::toRational
Decimal\Rational Object
(
    [num] => 8
    [den] => 5
)
