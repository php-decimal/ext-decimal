--TEST--
Number::add
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

use \Decimal\Decimal;
use \Decimal\Rational;

/* Number added to scalar by operator */
print_r(Number::valueOf(4) + "2.5");

/* Scalar added to Number by operator */
print_r("2.5" + Number::valueOf(4));

/* Number added to Number by operator */
print_r(Number::valueOf(4) + Number::valueOf("2.5"));

/* Number added to Number by method */
print_r(Number::valueOf(4)->add(Number::valueOf("2.5")));

/* Number added to Decimal by operator */
print_r(Number::valueOf(4) + Decimal::valueOf("2.5"));

/* Number added to Decimal by method */
print_r(Number::valueOf(4)->add(Decimal::valueOf("2.5")));

/* Number added to Rational by operator */
print_r(Number::valueOf(4) + Rational::valueOf("2.5"));

/* Number added to Rational by method */
print_r(Number::valueOf(4)->add(Rational::valueOf("2.5")));

/* Decimal added to Number by operator */
print_r(Decimal::valueOf(4) + Number::valueOf("2.5"));

/* Decimal added to Number by method */
print_r(Decimal::valueOf(4)->add(Number::valueOf("2.5")));

// /* Rational added to Number by operator */
print_r(Rational::valueOf(4) + Number::valueOf("2.5"));

// /* Rational added to Number by method */
print_r(Rational::valueOf(4)->add(Number::valueOf("2.5")));

?>
--EXPECT--
Number::add
Number Object
(
    [value:protected] => 6.5
)
Number::add
Number Object
(
    [value:protected] => 6.5
)
Number::add
Number Object
(
    [value:protected] => 6.5
)
Number::add
Number Object
(
    [value:protected] => 6.5
)
Number::add
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 6.5
            [precision] => 34
        )

)
Number::add
Number Object
(
    [value:protected] => Decimal\Decimal Object
        (
            [value] => 6.5
            [precision] => 34
        )

)
Number::add
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 13
            [den] => 2
        )

)
Number::add
Number Object
(
    [value:protected] => Decimal\Rational Object
        (
            [num] => 13
            [den] => 2
        )

)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 6.5
    [precision] => 34
)
Number::toDecimal
Decimal\Decimal Object
(
    [value] => 6.5
    [precision] => 34
)
Number::toRational
Decimal\Rational Object
(
    [num] => 13
    [den] => 2
)
Number::toRational
Decimal\Rational Object
(
    [num] => 13
    [den] => 2
)
