--TEST--
Decimal::exp
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, expected result, expected precision
 */
$tests = [
    [Decimal::valueOf("-0.1", 50), "0.90483741803595957316424905944643662119470536098040", 50],
    [Decimal::valueOf( "0.1", 50), "1.1051709180756476248117078264902466682245471947375", 50],

    [Decimal::valueOf(0), "1", 34],

    [Decimal::valueOf("-INF"), "0",   34],
    [Decimal::valueOf( "NAN"), "NAN", 34],
    [Decimal::valueOf( "INF"), "INF", 34],
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
