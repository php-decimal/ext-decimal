--TEST--
Rational::isNaN
--FILE--
<?php
use Decimal\Rational;

/**
 * op1, expected result
 */
$tests = [
    [ "1E-50",  false],
    ["-1E-50",  false],

    [0,         false],
    [1,         false],
    [2,         false],
    [3,         false],

    [-1,        false],
    [-2,        false],
    [-3,        false],

    ["1.5",     false],
    ["2.5",     false],
    ["3.5",     false],

    ["NAN",     true],
    ["-INF",    false],
    ["INF",     false],
];

foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];
    $result = Rational::valueOf($number)->isNan();

    if ($result !== $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECT--
