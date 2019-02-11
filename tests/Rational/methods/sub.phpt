--TEST--
Rational::sub
--FILE--
<?php
use Decimal\Rational;

/**
 * op1, op2, expected result
 */
$tests = [
    [Rational::valueOf(0),    0,     "0"],
    [Rational::valueOf(0),    1,    "-1"],
    [Rational::valueOf(0),   -1,     "1"],

    [Rational::valueOf(1),    0,     "1"],
    [Rational::valueOf(1),    1,     "0"],
    [Rational::valueOf(1),   -1,     "2"],

    [Rational::valueOf(-1),   0,    "-1"],
    [Rational::valueOf(-1),   1,    "-2"],
    [Rational::valueOf(-1),  -1,     "0"],

    [Rational::valueOf(0),   "0.5", "-1/2"],
    [Rational::valueOf(0),   "1.5", "-3/2"],
    [Rational::valueOf(0),  "-1.5",  "3/2"],

    [Rational::valueOf(1),   "0.5",  "1/2"],
    [Rational::valueOf(1),   "1.5", "-1/2"],
    [Rational::valueOf(1),  "-1.5",  "5/2"],

    [Rational::valueOf(-1),  "0.5", "-3/2"],
    [Rational::valueOf(-1),  "1.5", "-5/2"],
    [Rational::valueOf(-1), "-1.5",  "1/2"],

    /* Special numbers */
    [Rational::valueOf( "NAN"),  "NAN",  NAN -  NAN],
    [Rational::valueOf( "NAN"),  "INF",  NAN -  INF],
    [Rational::valueOf( "NAN"), "-INF",  NAN - -INF],
    [Rational::valueOf( "INF"),  "NAN",  INF -  NAN],
    [Rational::valueOf( "INF"),  "INF",  INF -  INF],
    [Rational::valueOf( "INF"), "-INF",  INF - -INF],
    [Rational::valueOf("-INF"),  "NAN", -INF -  NAN],
    [Rational::valueOf("-INF"),  "INF", -INF -  INF],
    [Rational::valueOf("-INF"), "-INF", -INF - -INF],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->sub($op2),
        $op1 - $op2,
    ];

    foreach ($results as $result) {
        if ((string) $result !== (string) $expect) {
            print_r(compact("index", "op1", "op2", "result", "expect"));
            break;
        }
    }
}

/* Test immutable */
$number = Rational::valueOf("2");
$number->sub(5);
$number - 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

?>
--EXPECT--
