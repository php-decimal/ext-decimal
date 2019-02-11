--TEST--
Rational::isZero
--FILE--
<?php
use Decimal\Rational;

/**
 * op1, expected result
 */
$tests = [
    [ "1E-50",  false],
    ["-1E-50",  false],

    [ "0E-50",  true],
    [ "0E+50",  true],
    ["-0E-50",  true],
    ["-0E+50",  true],

    [0,         true],
    [1,         false],
    [2,         false],
    [3,         false],

    [-1,        false],
    [-2,        false],
    [-3,        false],

    ["1.5",     false],
    ["2.5",     false],
    ["3.5",     false],

    [ "NAN",    false],
    [ "INF",    false],
    ["-INF",    false],
];

foreach ($tests as $pair) {
    $number = $pair[0];
    $expect = $pair[1];
    $result = Rational::valueOf($number)->isZero();

    if ($result !== $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECT--
