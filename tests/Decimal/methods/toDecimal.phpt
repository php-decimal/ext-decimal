--TEST--
Decimal::toDecimal
--FILE--
<?php
use Decimal\Decimal;

print_r(Decimal::valueOf(0)->toDecimal(Decimal::DEFAULT_PRECISION));
print_r(Decimal::valueOf("1.234")->toDecimal(Decimal::DEFAULT_PRECISION));
print_r(Decimal::valueOf("1.000")->toDecimal(Decimal::DEFAULT_PRECISION));

print_r(Decimal::valueOf(0, 2)->toDecimal(Decimal::DEFAULT_PRECISION));
print_r(Decimal::valueOf("1.234", 2)->toDecimal(Decimal::DEFAULT_PRECISION));
print_r(Decimal::valueOf("1.000", 2)->toDecimal(Decimal::DEFAULT_PRECISION));

print_r(Decimal::valueOf(0, 10)->toDecimal(2));
print_r(Decimal::valueOf("1.234", 10)->toDecimal(2));
print_r(Decimal::valueOf("1.000", 10)->toDecimal(2));

print_r(Decimal::valueOf(0, 10)->toDecimal(50));
print_r(Decimal::valueOf("1.234", 10)->toDecimal(50));
print_r(Decimal::valueOf("1.000", 10)->toDecimal(50));

/* Test precision out of range */
try {
    Decimal::valueOf("1")->toDecimal(-1);
} catch (\Exception $e) {
    printf("%s: %s\n", get_class($e), $e->getMessage());
}

?>
--EXPECT--
Decimal\Decimal Object
(
    [value] => 0
    [precision] => 34
)
Decimal\Decimal Object
(
    [value] => 1.234
    [precision] => 34
)
Decimal\Decimal Object
(
    [value] => 1.000
    [precision] => 34
)
Decimal\Decimal Object
(
    [value] => 0
    [precision] => 34
)
Decimal\Decimal Object
(
    [value] => 1.234
    [precision] => 34
)
Decimal\Decimal Object
(
    [value] => 1.000
    [precision] => 34
)
Decimal\Decimal Object
(
    [value] => 0
    [precision] => 2
)
Decimal\Decimal Object
(
    [value] => 1.234
    [precision] => 2
)
Decimal\Decimal Object
(
    [value] => 1.000
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
    [value] => 1.000
    [precision] => 50
)
OutOfRangeException: Decimal precision out of range
