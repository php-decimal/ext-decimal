--TEST--
Number::mul
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number multiplied by scalar using operator */
print_r(Number::valueOf(4) * "2.5");

/* Scalar multiplied by Number using operator */
print_r("2.5" * Number::valueOf(4));

/* Number multiplied by Number using operator */
print_r(Number::valueOf(4) * Number::valueOf("2.5"));

/* Number multiplied by Number using method */
print_r(Number::valueOf(4)->mul(Number::valueOf("2.5")));

/* Number multiplied by Decimal using operator */
print_r(Number::valueOf(4) * Decimal::valueOf("2.5"));

/* Number multiplied by Decimal using method */
print_r(Number::valueOf(4)->mul(Decimal::valueOf("2.5")));

/* Number multiplied by Rational using operator */
print_r(Number::valueOf(4) * Rational::valueOf("2.5"));

/* Number multiplied by Rational using method */
print_r(Number::valueOf(4)->mul(Rational::valueOf("2.5")));

/* Decimal multiplied by Number using operator */
print_r(Decimal::valueOf(4) * Number::valueOf("2.5"));

/* Decimal multiplied by Number using method */
print_r(Decimal::valueOf(4)->mul(Number::valueOf("2.5")));

/* Rational multiplied by Number using operator */
print_r(Rational::valueOf(4) * Number::valueOf("2.5"));

/* Rational multiplied by Number using method */
print_r(Rational::valueOf(4)->mul(Number::valueOf("2.5")));

?>
--EXPECTF--
Number::mul
Number Object
(
    [value:protected] => 10
)
Number::mul
Number Object
(
    [value:protected] => 10
)
Number::mul
Number Object
(
    [value:protected] => 10
)
Number::mul
Number Object
(
    [value:protected] => 10
)
Number::mul
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 10.0
            [precision] => 34
        )

)
Number::mul
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 10.0
            [precision] => 34
        )

)
Number::mul
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 10
            [den] => 1
        )

)
Number::mul
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 10
            [den] => 1
        )

)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 10.0
    [precision] => 34
)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 10.0
    [precision] => 34
)
Number::toRational
Decimal\Rational Object
(
    [num] => 10
    [den] => 1
)
Number::toRational
Decimal\Rational Object
(
    [num] => 10
    [den] => 1
)
