--TEST--
Rational::rem
--FILE--
<?php
use Decimal\Rational;

/**
 * op1, op2, expected result
 */
$tests = [
    [Rational::valueOf("0"),        "1",    0 %  1],
    [Rational::valueOf("0"),       "-1",    0 % -1],

    [Rational::valueOf( "1"),       "3",    1 %  3],
    [Rational::valueOf( "1"),      "-3",    1 % -3],
    [Rational::valueOf("-1"),      "-3",   -1 % -3],
    [Rational::valueOf("-1"),      "-3",   -1 % -3],

    [Rational::valueOf( "7"),       "4",    7 %  4],
    [Rational::valueOf( "7"),      "-4",    7 % -4],
    [Rational::valueOf("-7"),       "4",   -7 %  4],
    [Rational::valueOf("-7"),      "-4",   -7 % -4],

    [Rational::valueOf("5.678"),   "1.234", "371/500"],
    [Rational::valueOf("5.678"),   "2.234", "121/100"],
    [Rational::valueOf("5.678"),   "3.234", "611/250"],

    [Rational::valueOf("5"),       "1.5",   "1/2"],
    [Rational::valueOf("5"),       "2.5",   "0"],
    [Rational::valueOf("5"),       "3.5",   "3/2"],

    [
        Rational::valueOf("5.678"),
        Rational::valueOf("3.234"),
                          "611/250",
    ],

    [Rational::valueOf( "NAN"),  "NAN",   "NAN"],
    [Rational::valueOf( "NAN"),  "INF",   "NAN"],
    [Rational::valueOf( "NAN"), "-INF",   "NAN"],
    [Rational::valueOf( "INF"),  "NAN",   "NAN"],
    [Rational::valueOf( "INF"),  "INF",   "NAN"], // <-- No exception, technically invalid.
    [Rational::valueOf( "INF"), "-INF",   "NAN"], // <-- No exception, technically invalid.
    [Rational::valueOf("-INF"),  "NAN",   "NAN"],
    [Rational::valueOf("-INF"),  "INF",   "NAN"], // <-- No exception, technically invalid.
    [Rational::valueOf("-INF"), "-INF",   "NAN"], // <-- No exception, technically invalid.
    [Rational::valueOf( "NAN"),  1,       "NAN"],
    [Rational::valueOf( "INF"),  1,       "NAN"], // <-- No exception, technically invalid.
    [Rational::valueOf("-INF"),  1,       "NAN"], // <-- No exception, technically invalid.
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->rem($op2),
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
$number->rem(5);

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

/* Check division by zero */
try {
    Rational::valueOf(0)->rem(0);
} catch (DivisionByZeroError $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    Rational::valueOf(1)->rem(0);
} catch (DivisionByZeroError $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    Rational::valueOf(NAN)->rem(0);
} catch (DivisionByZeroError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    Rational::valueOf(INF)->rem(0);
} catch (DivisionByZeroError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    Rational::valueOf(-INF)->rem(0);
} catch (DivisionByZeroError $e) {
    printf("E %s\n", $e->getMessage());
}

?>
--EXPECT--
A Division by zero
B Division by zero
C Division by zero
D Division by zero
E Division by zero
