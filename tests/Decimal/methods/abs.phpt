--TEST--
Decimal::abs
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, expected result, expected precision
 */
$tests = [
    [Decimal::valueOf( "0"),        "0",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("+0"),        "0",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-0"),        "0",    Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("-0.1"),      "0.1",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("+0.1"),      "0.1",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "0.1"),      "0.1",  Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("-1"),        "1",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "1"),        "1",    Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("-INF"),      "INF",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),      "INF",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"),      "NAN",  Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf( "0", 10),    "0",    10],
    [Decimal::valueOf("+0", 10),    "0",    10],
    [Decimal::valueOf("-0", 10),    "0",    10],

    [Decimal::valueOf("-0.1", 10),  "0.1",  10],
    [Decimal::valueOf("+0.1", 10),  "0.1",  10],
    [Decimal::valueOf( "0.1", 10),  "0.1",  10],

    [Decimal::valueOf("-1", 10),    "1",    10],
    [Decimal::valueOf( "1", 10),    "1",    10],

    [Decimal::valueOf( "NAN", 10),  "NAN",  10],
    [Decimal::valueOf( "INF", 10),  "INF",  10],
    [Decimal::valueOf("-INF", 10),  "INF",  10],
];

/* Test expected output */
foreach ($tests as $test) {
    list($op1, $expect, $prec) = $test;

    $result = $op1->abs();

    if ([(string) $result, $result->precision()] !== [$expect, $prec]) {
        print_r(compact("op1", "result", "expect", "prec"));
    }
}

/* Test immutable */
$number = Decimal::valueOf("-1");
$result = $number->abs();

if ((string) $number !== "-1") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
