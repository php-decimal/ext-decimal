--TEST--
Number::sub
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number minus scalar by operator */
print_r(Number::valueOf(4) - "2.5");

/* Scalar minus Number by operator */
print_r("2.5" - Number::valueOf(4));

/* Number minus Number by operator */
print_r(Number::valueOf(4) - Number::valueOf("2.5"));

/* Number minus Number by method */
print_r(Number::valueOf(4)->sub(Number::valueOf("2.5")));

/* Number minus Decimal by operator */
print_r(Number::valueOf(4) - Decimal::valueOf("2.5"));

/* Number minus Decimal by method */
print_r(Number::valueOf(4)->sub(Decimal::valueOf("2.5")));

/* Number minus Rational by operator */
print_r(Number::valueOf(4) - Rational::valueOf("2.5"));

/* Number minus Rational by method */
print_r(Number::valueOf(4)->sub(Rational::valueOf("2.5")));

/* Decimal minus Number by operator */
print_r(Decimal::valueOf(4) - Number::valueOf("2.5"));

/* Decimal minus Number by method */
print_r(Decimal::valueOf(4)->sub(Number::valueOf("2.5")));

/* Rational minus Number by operator */
print_r(Rational::valueOf(4) - Number::valueOf("2.5"));

/* Rational minus Number by method */
print_r(Rational::valueOf(4)->sub(Number::valueOf("2.5")));

?>
--EXPECT--
Number::sub
Number Object
(
    [value:protected] => 1.5
)
Number::sub
Number Object
(
    [value:protected] => -1.5
)
Number::sub
Number Object
(
    [value:protected] => 1.5
)
Number::sub
Number Object
(
    [value:protected] => 1.5
)
Number::sub
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 1.5
            [precision] => 34
        )

)
Number::sub
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 1.5
            [precision] => 34
        )

)
Number::sub
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 3
            [den] => 2
        )

)
Number::sub
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 3
            [den] => 2
        )

)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 1.5
    [precision] => 34
)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 1.5
    [precision] => 34
)
Number::toRational
Decimal\Rational Object
(
    [num] => 3
    [den] => 2
)
Number::toRational
Decimal\Rational Object
(
    [num] => 3
    [den] => 2
)
