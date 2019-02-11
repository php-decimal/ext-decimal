--TEST--
Decimal::shiftr
--FILE--
<?php
use Decimal\Decimal;

$tests = [
    ["1",        3,      "0.001"],
    ["1",        2,      "0.01"],
    ["1",        1,      "0.1"],
    ["1",        0,      "1"],
    ["1",       -1,      "10"],
    ["1",       -2,      "100"],
    ["1",       -3,      "1000"],

    ["0.5",      3,      "0.0005"],
    ["0.5",      2,      "0.005"],
    ["0.5",      1,      "0.05"],
    ["0.5",      0,      "0.5"],
    ["0.5",     -1,      "5"],
    ["0.5",     -2,      "50"],
    ["0.5",     -3,      "500"],

    ["1",        10000,  "1E-10000"],
    ["1",       -10000,  "1E+10000"],

    [ "NAN",     2,      "NAN"],
    [ "INF",     2,      "INF"],
    ["-INF",     2,     "-INF"],

    [ "NAN",    -2,      "NAN"],
    [ "INF",    -2,      "INF"],
    ["-INF",    -2,     "-INF"],
];

foreach ($tests as $test) {
    list($number, $places, $expect) = $test;

    $results = [
        Decimal::valueOf($number)->shiftr($places),
        Decimal::valueOf($number) >>  $places,
        Decimal::valueOf($number) << -$places,
    ];

    foreach ($results as $result) {
        if ((string) $result !== $expect || $result->precision() !== 34) {
            print_r(compact("number", "places", "result", "expect"));
            break;
        }
    }
}
?>
--EXPECT--
