--TEST--
Number::isNegative
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

/**
 * op1, expected result
 */
$tests = [
    [ "1E-50",  false],
    ["-1E-50",  true],

    [0,         false],
    [1,         false],
    [2,         false],
    [3,         false],

    ["0",       false],
    ["1",       false],
    ["2",       false],
    ["3",       false],

    ["-0",      true],
    ["-1",      true],
    ["-2",      true],
    ["-3",      true],

    ["0.4",     false],
    ["1.4",     false],
    ["2.4",     false],
    ["3.4",     false],

    [-INF,      true],
    [ INF,      false],
    [ NAN,      false],
];

foreach ($tests as $test) {
    list($number, $expect) = $test;

    $result = Number::valueOf($number)->isNegative();

    if ($result !== $expect) {
        var_dump(compact("number", "result", "expect"));
    }
    printf("\n");
}

/* Test that a negative zero rounding has a negative result. */
var_dump(Number::valueOf("-0.05")->trunc()->isNegative());

?>
--EXPECT--
Number::toDecimal
bool(true)