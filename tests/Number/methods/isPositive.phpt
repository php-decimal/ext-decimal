--TEST--
Number::isPositive
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

/**
 * op1, expected result
 */
$tests = [
    [ "1E-50",  true],
    ["-1E-50",  false],

    [0,         false],
    [1,         true],
    [2,         true],
    [3,         true],

    ["0",       false],
    ["1",       true],
    ["2",       true],
    ["3",       true],

    ["-0",      false],
    ["-1",      false],
    ["-2",      false],
    ["-3",      false],

    ["0.4",     true],
    ["1.4",     true],
    ["2.4",     true],
    ["3.4",     true],

    [-INF,      false],
    [ INF,      true],
    [ NAN,      false],
];

foreach ($tests as $test) {
    list($number, $expect) = $test;

    $result = Number::valueOf($number)->isPositive();

    if ($result !== $expect) {
        var_dump(compact("number", "result", "expect"));
    }
    printf("\n");
}

/* Test that a negative zero rounding has a negative result. */
var_dump(Number::valueOf("-0.05")->trunc()->isPositive());

?>
--EXPECT--
Number::toDecimal
bool(false)