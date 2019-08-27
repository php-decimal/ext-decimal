--TEST--
Number::isZero
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

/**
 * op1, expected result
 */
$tests = [
    [ "1E-50",  false],
    ["-1E-50",  false],

    [0,         true],
    [1,         false],
    [2,         false],
    [3,         false],

    ["0",       true],
    ["1",       false],
    ["2",       false],
    ["3",       false],

    ["0.0",     true],
    ["1.0",     false],
    ["2.0",     false],
    ["3.0",     false],

    ["0.4",     false],
    ["1.4",     false],
    ["2.4",     false],
    ["3.4",     false],

    [-INF,      false],
    [ INF,      false],
    [ NAN,      false],
];

foreach ($tests as $test) {
    list($number, $expect) = $test;

    $result = Number::valueOf($number)->isZero();

    if ($result !== $expect) {
        var_dump(compact("number", "result", "expect"));
    }
    printf("\n");
}

/* Test that a negative zero rounding is still zero. */
var_dump(Number::valueOf("-0.05")->trunc()->isZero());

?>
--EXPECT--
Number::toDecimal
bool(true)