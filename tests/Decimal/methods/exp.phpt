--TEST--
Decimal::exp
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, expected result, expected precision
 */
$tests = [
    [Decimal::valueOf("-0.1", 50), "0.90483741803595957316424905944643662119470536098040", 50],
    [Decimal::valueOf( "0.1", 50), "1.1051709180756476248117078264902466682245471947375", 50],

    [Decimal::valueOf(0), "1", Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf( "NAN"), "NAN", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"), "INF", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"), "0",   Decimal::DEFAULT_PRECISION],
];

foreach ($tests as $test) {
    list($op1, $expect, $prec) = $test;

    $result = $op1->exp();

    if ((string) $result !== $expect || $result->precision() !== $prec) {
        print_r(compact("number", "result", "expect", "prec"));
    }
}

/* Test immutable */
$number = Decimal::valueOf("2");
$result = $number->exp();

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
