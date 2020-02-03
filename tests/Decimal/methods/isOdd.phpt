--TEST--
Decimal::isOdd
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, expected result
 */
$tests = [
    [ "1E-50",  false],
    ["-1E-50",  false],

    [ "3.01",   false],
    ["-3.01",   false],

    [ "3.00",   true],
    ["-3.00",   true],

    [0,         false],
    [1,         true],
    [2,         false],
    [3,         true],

    [-1,        true],
    [-2,        false],
    [-3,        true],

    ["1.5",     false],
    ["2.5",     false],
    ["3.5",     false],

    [ "NAN",    false],
    [ "INF",    false],
    ["-INF",    false],
];

foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];
    $result = Decimal::valueOf($number)->isOdd();

    if ($result !== $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECT--
