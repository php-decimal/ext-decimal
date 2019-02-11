--TEST--
Decimal::toFixed
--FILE--
<?php
use Decimal\Decimal;

$tests = [
    /* decimal places, grouped, rounding mode, expected */

    ["0",           5,     false,   Decimal::ROUND_HALF_UP,      "0.00000"],

    ["1.23456",     3,     false,   Decimal::ROUND_UP,           "1.235"],
    ["1.23456",     3,     false,   Decimal::ROUND_DOWN,         "1.234"],
    ["1.23456",     3,     false,   Decimal::ROUND_CEILING,      "1.235"],

    ["1.23456",     3,     false,   Decimal::ROUND_FLOOR,        "1.234"],
    ["1.23456",     3,     false,   Decimal::ROUND_HALF_DOWN,    "1.235"],
    ["1.23456",     3,     false,   Decimal::ROUND_HALF_EVEN,    "1.235"],
    ["1.23456",     3,     false,   Decimal::ROUND_HALF_UP,      "1.235"],

    ["1000000",     0,     true,    Decimal::ROUND_UP,   "1,000,000"],
    ["1000000",     2,     true,    Decimal::ROUND_UP,   "1,000,000.00"],
    ["1000000",     0,     false,   Decimal::ROUND_UP,   "1000000"],
    ["1000000",     2,     false,   Decimal::ROUND_UP,   "1000000.00"],

    [ "NAN",        2,     false,   Decimal::ROUND_UP,   "NAN"],
    [ "INF",        2,     false,   Decimal::ROUND_UP,   "INF"],
    ["-INF",        2,     false,   Decimal::ROUND_UP,  "-INF"],
];

foreach ($tests as $test) {
    list($number, $places, $grouped, $rounding, $expect) = $test;

    $result = (string) (Decimal::valueOf($number))->toFixed($places, $grouped, $rounding);

    if ($result !== $expect) {
        print_r(compact("number", "places", "rounding", "result", "expect"));
    }
}

/**
 * Test default rounding.
 */
var_dump((Decimal::valueOf("1.125"))->toFixed(2));

/**
 * Test default comma separation.
 */
var_dump((Decimal::valueOf("1125"))->toFixed(2, true));
var_dump((Decimal::valueOf("1125"))->toFixed(2));

/**
 * Test default places.
 */
var_dump((Decimal::valueOf("1.125"))->toFixed());
?>
--EXPECT--
string(4) "1.12"
string(8) "1,125.00"
string(7) "1125.00"
string(1) "1"
