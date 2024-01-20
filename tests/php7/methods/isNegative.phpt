--TEST--
Decimal::isNegative
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

/**
 * Shortcut to construct a new decimal.
 */
function decimal(...$args) { return new Decimal(...$args); }

$tests = [
    /* Number, expected */

    [ "1E-50",  false],
    ["-1E-50",  true],

    ["0",       false],
    ["-0",      false],
    [1,         false],
    [2,         false],
    [3,         false],

    [-1,        true],
    [-2,        true],
    [-3,        true],

    ["1.5",     false],
    ["2.5",     false],
    ["3.5",     false],

    ["-1.5",    true],
    ["-2.5",    true],
    ["-3.5",    true],

    [ "NAN",    false],
    [ "INF",    false],
    ["-INF",    true],
];

foreach ($tests as $pair) {
    $number = $pair[0];
    $expect = $pair[1];
    $result = decimal($number)->isNegative();

    if ((string) $result !== (string) $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECT--
