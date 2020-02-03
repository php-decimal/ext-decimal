--TEST--
Rational::toFixed
--FILE--
<?php
use Decimal\Rational;

$tests = [
    /* decimal places, grouped, rounding mode, expected */

    ["0",           5,     false,   Rational::ROUND_HALF_UP,      "0.00000"],

    ["1.23456",     3,     false,   Rational::ROUND_UP,           "1.235"],
    ["1.23456",     3,     false,   Rational::ROUND_DOWN,         "1.234"],
    ["1.23456",     3,     false,   Rational::ROUND_CEILING,      "1.235"],

    ["1.23456",     3,     false,   Rational::ROUND_FLOOR,        "1.234"],
    ["1.23456",     3,     false,   Rational::ROUND_HALF_DOWN,    "1.235"],
    ["1.23456",     3,     false,   Rational::ROUND_HALF_EVEN,    "1.235"],
    ["1.23456",     3,     false,   Rational::ROUND_HALF_UP,      "1.235"],

    ["1000000",     0,     true,    Rational::ROUND_UP,   "1,000,000"],
    ["1000000",     2,     true,    Rational::ROUND_UP,   "1,000,000.00"],
    ["1000000",     0,     false,   Rational::ROUND_UP,   "1000000"],
    ["1000000",     2,     false,   Rational::ROUND_UP,   "1000000.00"],

    [ "NAN",        2,     false,   Rational::ROUND_UP,   "NAN"],
    [ "INF",        2,     false,   Rational::ROUND_UP,   "INF"],
    ["-INF",        2,     false,   Rational::ROUND_UP,  "-INF"],
];

foreach ($tests as $test) {
    list($number, $places, $grouped, $rounding, $expect) = $test;

    $result = Rational::valueOf($number)->toFixed($places, $grouped, $rounding);

    if ((string) $result !== (string) $expect) {
        print_r(compact("number", "places", "rounding", "result", "expect"));
    }
}

/**
 * Test default rounding.
 */
var_dump(Rational::valueOf("1.125")->toFixed(2));

/**
 * Test default comma separation.
 */
var_dump(Rational::valueOf("1125")->toFixed(2, true));
var_dump(Rational::valueOf("1125")->toFixed(2));

/**
 * Test default places.
 */
var_dump(Rational::valueOf("1.125")->toFixed());
?>
--EXPECT--
string(4) "1.12"
string(8) "1,125.00"
string(7) "1125.00"
string(1) "1"
