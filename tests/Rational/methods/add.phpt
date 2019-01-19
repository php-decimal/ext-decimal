--TEST--
Rational::add
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Rational;

/**
 * op1, op2, expected result
 */
$tests = [
    [Rational::valueOf(0),    0,     "0"],
    [Rational::valueOf(0),    1,     "1"],
    [Rational::valueOf(0),   -1,    "-1"],

    [Rational::valueOf(1),    0,     "1"],
    [Rational::valueOf(1),    1,     "2"],
    [Rational::valueOf(1),   -1,     "0"],

    [Rational::valueOf(-1),   0,    "-1"],
    [Rational::valueOf(-1),   1,     "0"],
    [Rational::valueOf(-1),  -1,    "-2"],

    [Rational::valueOf(0),   "0.5",  "1/2"],
    [Rational::valueOf(0),   "1.5",  "3/2"],
    [Rational::valueOf(0),  "-1.5", "-3/2"],

    [Rational::valueOf(1),   "0.5",  "3/2"],
    [Rational::valueOf(1),   "1.5",  "5/2"],
    [Rational::valueOf(1),  "-1.5", "-1/2"],

    [Rational::valueOf(-1),  "0.5", "-1/2"],
    [Rational::valueOf(-1),  "1.5",  "1/2"],
    [Rational::valueOf(-1), "-1.5", "-5/2"],

    /* Special numbers */
    [Rational::valueOf( "NAN"),  "NAN", (string)  (NAN +  NAN)],
    [Rational::valueOf( "NAN"),  "INF", (string)  (NAN +  INF)],
    [Rational::valueOf( "NAN"), "-INF", (string)  (NAN + -INF)],
    [Rational::valueOf( "INF"),  "NAN", (string)  (INF +  NAN)],
    [Rational::valueOf( "INF"),  "INF", (string)  (INF +  INF)],
    [Rational::valueOf( "INF"), "-INF", (string)  (INF + -INF)],
    [Rational::valueOf("-INF"),  "NAN", (string) (-INF +  NAN)],
    [Rational::valueOf("-INF"),  "INF", (string) (-INF +  INF)],
    [Rational::valueOf("-INF"), "-INF", (string) (-INF + -INF)],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->add($op2),

        $op1 + $op2,
        $op2 + $op1,
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
$number->add(5);
$number + 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

?>
--EXPECT--
