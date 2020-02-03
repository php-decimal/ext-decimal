--TEST--
Number::mod
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number mod scalar using operator */
print_r(Number::valueOf(9) % "6.5");

/* Scalar mod Number using operator */
print_r("6.5" % Number::valueOf(9));

/* Number mod Number using operator */
print_r(Number::valueOf(9) % Number::valueOf("6.5"));

/* Number mod Number using method */
print_r(Number::valueOf(9)->mod(Number::valueOf("6.5")));

/* Number mod Decimal using operator */
print_r(Number::valueOf(9) % Decimal::valueOf("6.5"));

/* Number mod Decimal using method */
print_r(Number::valueOf(9)->mod(Decimal::valueOf("6.5")));

/* Number mod Rational using operator */
print_r(Number::valueOf(9) % Rational::valueOf("6.5"));

/* Number mod Rational using method */
print_r(Number::valueOf(9)->mod(Rational::valueOf("6.5")));

/* Decimal mod Number using operator */
print_r(Decimal::valueOf(9) % Number::valueOf("6.5"));

/* Decimal mod Number using method */
print_r(Decimal::valueOf(9)->mod(Number::valueOf("6.5")));

/* Rational mod Number using operator */
print_r(Rational::valueOf(9) % Number::valueOf("6.5"));

/* Rational mod Number using method */
print_r(Rational::valueOf(9)->mod(Number::valueOf("6.5")));

?>
--EXPECT--
Number::mod
Number Object
(
    [value:protected] => 3
)
Number::mod
Number Object
(
    [value:protected] => 6
)
Number::mod
Number Object
(
    [value:protected] => 3
)
Number::mod
Number Object
(
    [value:protected] => 3
)
Number::mod
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 3
            [precision] => 34
        )

)
Number::mod
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 3
            [precision] => 34
        )

)
Number::mod
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 3
            [den] => 1
        )

)
Number::mod
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 3
            [den] => 1
        )

)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 3
    [precision] => 34
)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 3
    [precision] => 34
)
Number::toRational
Decimal\Rational Object
(
    [num] => 3
    [den] => 1
)
Number::toRational
Decimal\Rational Object
(
    [num] => 3
    [den] => 1
)
