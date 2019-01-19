--TEST--
Rational::abs
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Rational;

/**
 * op1, expected result, expected precision
 */
$tests = [
    [Rational::valueOf( "0"),        "0"],
    [Rational::valueOf("+0"),        "0"],
    [Rational::valueOf("-0"),        "0"],

    [Rational::valueOf("-0.1"),      "1/10"],
    [Rational::valueOf("+0.1"),      "1/10"],
    [Rational::valueOf( "0.1"),      "1/10"],

    [Rational::valueOf("-1"),        "1"],
    [Rational::valueOf( "1"),        "1"],

    [Rational::valueOf("-INF"),      "INF"],
    [Rational::valueOf( "INF"),      "INF"],
    [Rational::valueOf( "NAN"),      "NAN"],
];

/* Test expected output */
foreach ($tests as $test) {
    list($op1, $expect) = $test;

    $result = $op1->abs();

    if ((string) $result !== $expect) {
        print_r(compact("op1", "result", "expect"));
    }
}

/* Test immutable */
$number = Rational::valueOf("-1");
$result = $number->abs();

if ((string) $number !== "-1") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
