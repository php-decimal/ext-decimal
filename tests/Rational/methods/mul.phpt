--TEST--
Rational::mul
--FILE--
<?php
use Decimal\Rational;

/**
 * op1, op2, expected result
 */
$tests = [
    [Rational::valueOf( "0"), "-0", "-0"],
    [Rational::valueOf( "0"), "-0", "-0"],
    [Rational::valueOf( "0"),  "0",  "0"],
    [Rational::valueOf("-0"), "-0",  "0"],
    [Rational::valueOf("-0"),  "0", "-0"],

    [Rational::valueOf( "1"), "-0", "-0"],
    [Rational::valueOf( "1"), "-0", "-0"],
    [Rational::valueOf( "1"),  "0",  "0"],
    [Rational::valueOf("-1"), "-0",  "0"],
    [Rational::valueOf("-1"),  "0", "-0"],

    [Rational::valueOf( "0"), "-1", "-0"],
    [Rational::valueOf( "0"), "-1", "-0"],
    [Rational::valueOf( "0"),  "1",  "0"],
    [Rational::valueOf("-0"), "-1",  "0"],
    [Rational::valueOf("-0"),  "1", "-0"],

    [Rational::valueOf( "1.5"),  "2.4",  "18/5"],
    [Rational::valueOf("-1.5"),  "2.4", "-18/5"],
    [Rational::valueOf( "1.5"), "-2.4", "-18/5"],
    [Rational::valueOf("-1.5"), "-2.4",  "18/5"],

    /* Special numbers */
    [Rational::valueOf( "NAN"),  "NAN", (string)  (NAN *  NAN)],
    [Rational::valueOf( "NAN"),  "INF", (string)  (NAN *  INF)],
    [Rational::valueOf( "NAN"), "-INF", (string)  (NAN * -INF)],
    [Rational::valueOf( "INF"),  "NAN", (string)  (INF *  NAN)],
    [Rational::valueOf( "INF"),  "INF", (string)  (INF *  INF)],
    [Rational::valueOf( "INF"), "-INF", (string)  (INF * -INF)],
    [Rational::valueOf("-INF"),  "NAN", (string) (-INF *  NAN)],
    [Rational::valueOf("-INF"),  "INF", (string) (-INF *  INF)],
    [Rational::valueOf("-INF"), "-INF", (string) (-INF * -INF)],

    [Rational::valueOf( "NAN"), 0, "NAN"],
    [Rational::valueOf( "INF"), 0, "NAN"],
    [Rational::valueOf("-INF"), 0, "NAN"],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->mul($op2),

        $op1 * $op2,
        $op2 * $op1,
    ];

    foreach ($results as $result) {
        if ((string) $result !== $expect) {
            print_r(compact("index", "op1", "op2", "result", "expect"));
            break;
        }
    }
}

/* Test immutable */
$number = Rational::valueOf("2");
$number->mul(5);
$number * 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

?>
--EXPECT--
