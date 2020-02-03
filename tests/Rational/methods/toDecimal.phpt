--TEST--
Rational::toDecimal
--FILE--
<?php
use Decimal\Rational;

print_r(Rational::valueOf("0")->toDecimal(2));
print_r(Rational::valueOf("1.234")->toDecimal(2));
print_r(Rational::valueOf("1.000")->toDecimal(2));

print_r(Rational::valueOf("0")->toDecimal(50));
print_r(Rational::valueOf("1.234")->toDecimal(50));
print_r(Rational::valueOf("1.000")->toDecimal(50));

/* Test precision out of range */
try {
    Rational::valueOf("1")->toDecimal(-1);
} catch (\Exception $e) {
    printf("%s: %s\n", get_class($e), $e->getMessage());
}

/* Test precision out of range */
try {
    Rational::valueOf("1")->toDecimal(0);
} catch (\Exception $e) {
    printf("%s: %s\n", get_class($e), $e->getMessage());
}

/* Test with no precision given */
try {
    Rational::valueOf("1")->toDecimal();
} catch (\ArgumentCountError $e) {
    printf("%s: %s\n", get_class($e), $e->getMessage());
}

?>
--EXPECT--
Decimal\Decimal Object
(
    [value] => 0
    [precision] => 2
)
Decimal\Decimal Object
(
    [value] => 1.2
    [precision] => 2
)
Decimal\Decimal Object
(
    [value] => 1
    [precision] => 2
)
Decimal\Decimal Object
(
    [value] => 0
    [precision] => 50
)
Decimal\Decimal Object
(
    [value] => 1.234
    [precision] => 50
)
Decimal\Decimal Object
(
    [value] => 1
    [precision] => 50
)
OutOfRangeException: Decimal precision out of range
OutOfRangeException: Decimal precision out of range
ArgumentCountError: Decimal\Rational::toDecimal() expects exactly 1 parameter, 0 given