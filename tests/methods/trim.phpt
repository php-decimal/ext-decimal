--TEST--
Decimal::trim
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
    /* Input    Expected */
    ["0",       "0"],
    ["0.00",    "0"],
    ["0.010",   "0.01"],

    [ "NAN",     "NAN"],
    [ "INF",     "INF"],
    ["-INF",    "-INF"],
];

foreach ($tests as $test) {
    list($number, $expect) = $test;

    $result = decimal($number)->trim();

    if ((string) $result !== $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECT--
