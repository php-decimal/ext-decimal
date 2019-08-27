--TEST--
Number::isInteger
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
    [1,         true],
    [2,         true],
    [3,         true],

    ["0",       true],
    ["1",       true],
    ["2",       true],
    ["3",       true],

    ["0.0",     true],
    ["1.0",     true],
    ["2.0",     true],
    ["3.0",     true],

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

    $result = Number::valueOf($number)->isInteger();

    if ($result !== $expect) {
        var_dump(compact("number", "result", "expect"));
    }
    printf("\n");
}
?>
--EXPECT--
Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo

Number::toDecimal
Number::compareTo