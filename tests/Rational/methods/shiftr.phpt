--TEST--
Rational::shiftr
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Rational;

$tests = [
    ["1",        3,      "1/1000"],
    ["1",        2,      "1/100"],
    ["1",        1,      "1/10"],
    ["1",        0,      "1"],
    ["1",       -1,      "10"],
    ["1",       -2,      "100"],
    ["1",       -3,      "1000"],

    ["0.5",      3,      "1/2000"],
    ["0.5",      2,      "1/200"],
    ["0.5",      1,      "1/20"],
    ["0.5",      0,      "1/2"],
    ["0.5",     -1,      "5"],
    ["0.5",     -2,      "50"],
    ["0.5",     -3,      "500"],

    ["1",        10000,  "1/1E+10000"],
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
        Rational::valueOf($number)->shiftr($places),
        Rational::valueOf($number) >>  $places,
        Rational::valueOf($number) << -$places,
    ];

    foreach ($results as $result) {
        if ((string) $result !== (string) $expect) {
            print_r(compact("number", "places", "result", "expect"));
            break;
        }
    }
}
?>
--EXPECT--
