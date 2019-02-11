--TEST--
Decimal::mod
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, op2, expected result, expected precision
 */
$tests = [
    [Decimal::valueOf("0"),        "1",    0 %  1,    34],
    [Decimal::valueOf("0"),       "-1",    0 % -1,    34],

    [Decimal::valueOf( "1"),       "3",    1 %  3,    34],
    [Decimal::valueOf( "1"),      "-3",    1 % -3,    34],
    [Decimal::valueOf("-1"),      "-3",   -1 % -3,    34],
    [Decimal::valueOf("-1"),      "-3",   -1 % -3,    34],

    [Decimal::valueOf( "7"),       "4",    7 %  4,    34],
    [Decimal::valueOf( "7"),      "-4",    7 % -4,    34],
    [Decimal::valueOf("-7"),       "4",   -7 %  4,    34],
    [Decimal::valueOf("-7"),      "-4",   -7 % -4,    34],

    /* mod is an integer operation*/
    [Decimal::valueOf("5.678"),   "1.234",    "0",    34],
    [Decimal::valueOf("5.678"),   "2.234",    "1",    34],
    [Decimal::valueOf("5.678"),   "3.234",    "2",    34],

    [Decimal::valueOf("5"),       "1.5",      "0",    34], // 5 % 1
    [Decimal::valueOf("5"),       "2.5",      "1",    34], // 5 % 2
    [Decimal::valueOf("5"),       "3.5",      "2",    34], // 5 % 3

    [
        Decimal::valueOf("5.678", 5),
        Decimal::valueOf("3.234", 5),
                         "2",     5
    ],

    [Decimal::valueOf( "NAN"),  "NAN",   "NAN", 34],
    [Decimal::valueOf( "NAN"),  "INF",   "NAN", 34],
    [Decimal::valueOf( "NAN"), "-INF",   "NAN", 34],
    [Decimal::valueOf( "INF"),  "NAN",   "NAN", 34],
    [Decimal::valueOf( "INF"),  "INF",   "NAN", 34], // <-- No exception, technically invalid.
    [Decimal::valueOf( "INF"), "-INF",   "NAN", 34], // <-- No exception, technically invalid.
    [Decimal::valueOf("-INF"),  "NAN",   "NAN", 34],
    [Decimal::valueOf("-INF"),  "INF",   "NAN", 34], // <-- No exception, technically invalid.
    [Decimal::valueOf("-INF"), "-INF",   "NAN", 34], // <-- No exception, technically invalid.

    [Decimal::valueOf( "NAN"),  1,       "NAN", 34],
    [Decimal::valueOf( "INF"),  1,       "NAN", 34], // <-- No exception, technically invalid.
    [Decimal::valueOf("-INF"),  1,       "NAN", 34], // <-- No exception, technically invalid.
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $prec) = $test;

    $results = [
        $op1->mod($op2),
        $op1 % $op2,
    ];

    foreach ($results as $result) {
        if ([(string) $result, $result->precision()] !== [(string) $expect, $prec]) {
            print_r(compact("index", "op1", "op2", "result", "expect", "prec"));
            break;
        }
    }
}

/* Test immutable */
$number = Decimal::valueOf("2");
$number->mod(5);
$number % 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

/* Check division by zero */
try {
    Decimal::valueOf(0) % 0;
} catch (DivisionByZeroError $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(1) % 0;
} catch (DivisionByZeroError $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(NAN) % 0;
} catch (DivisionByZeroError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(INF) % 0;
} catch (DivisionByZeroError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(-INF) % 0;
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
