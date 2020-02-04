--TEST--
Number::pow
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number to the power of scalar using operator */
print_r(Number::valueOf(4) ** "2.5");

/* Scalar to the power of Number using operator */
print_r("2.5" ** Number::valueOf(4));

/* Number to the power of Number using operator */
print_r(Number::valueOf(4) ** Number::valueOf("2.5"));

/* Number to the power of Number using method */
print_r(Number::valueOf(4)->pow(Number::valueOf("2.5")));

/* Number to the power of Decimal using operator */
print_r(Number::valueOf(4) ** Decimal::valueOf("2.5"));

/* Number to the power of Decimal using method */
print_r(Number::valueOf(4)->pow(Decimal::valueOf("2.5")));

/* Number to the power of Rational using operator */
print_r(Number::valueOf(4) ** Rational::valueOf("2.5"));

/* Number to the power of Rational using method */
print_r(Number::valueOf(4)->pow(Rational::valueOf("2.5")));

/* Decimal to the power of Number using operator */
print_r(Decimal::valueOf(4) ** Number::valueOf("2.5"));

/* Decimal to the power of Number using method */
print_r(Decimal::valueOf(4)->pow(Number::valueOf("2.5")));

/* Rational to the power of Number using operator */
print_r(Rational::valueOf(4) ** Number::valueOf("2.5"));

/* Rational to the power of Number using method */
print_r(Rational::valueOf(4)->pow(Number::valueOf("2.5")));

?>
--EXPECTF--
Number::pow
Number Object
(
    [value:protected] => 32
)
Number::pow
Number Object
(
    [value:protected] => 39.0625
)
Number::pow
Number Object
(
    [value:protected] => 32
)
Number::pow
Number Object
(
    [value:protected] => 32
)
Number::pow
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 32.00000000000000000000000000000000
            [precision] => 34
        )

)
Number::pow
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 32.00000000000000000000000000000000
            [precision] => 34
        )

)
Number::pow

Warning: Decimal arithmetic operand truncated to integer in %sNumber.php on line 63
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 16
            [den] => 1
        )

)
Number::pow

Warning: Decimal arithmetic operand truncated to integer in %sNumber.php on line 63
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 16
            [den] => 1
        )

)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 32.00000000000000000000000000000000
    [precision] => 34
)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 32.00000000000000000000000000000000
    [precision] => 34
)
Number::toRational

Warning: Decimal arithmetic operand truncated to integer in %spow.php on line 38
Decimal\Rational Object
(
    [num] => 16
    [den] => 1
)
Number::toRational

Warning: Decimal arithmetic operand truncated to integer in %spow.php on line 41
Decimal\Rational Object
(
    [num] => 16
    [den] => 1
)
