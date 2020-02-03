--TEST--
Decimal::rem
--FILE--
<?php
use Decimal\Decimal;

/**
 * OP1, OP2, Expected Result, Expected Precision
 */
$tests = [
    [Decimal::valueOf("0"),        "1",    0 %  1,    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("0"),       "-1",    0 % -1,    Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf( "1"),       "3",    1 %  3,    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "1"),      "-3",    1 % -3,    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1"),      "-3",   -1 % -3,    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1"),      "-3",   -1 % -3,    Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf( "7"),       "4",    7 %  4,    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "7"),      "-4",    7 % -4,    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-7"),       "4",   -7 %  4,    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-7"),      "-4",   -7 % -4,    Decimal::DEFAULT_PRECISION],

    /* rem is NOT an integer operation*/
    [Decimal::valueOf("5.678"),   "1.234",    "0.742",   Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("5.678"),   "2.234",    "1.210",   Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("5.678"),   "3.234",    "2.444",   Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("5"),       "1.5",      "0.5",     Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("5"),       "2.5",      "0.0",     Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("5"),       "3.5",      "1.5",     Decimal::DEFAULT_PRECISION],

    [
        Decimal::valueOf("5.678", 5),
        Decimal::valueOf("3.234", 5),
                         "2.444", 5
    ],

    [Decimal::valueOf( "NAN"),  "NAN",   "NAN", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"),  "INF",   "NAN", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"), "-INF",   "NAN", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "NAN",   "NAN", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "INF",   "NAN", Decimal::DEFAULT_PRECISION], // <-- No exception, technically invalid.
    [Decimal::valueOf( "INF"), "-INF",   "NAN", Decimal::DEFAULT_PRECISION], // <-- No exception, technically invalid.
    [Decimal::valueOf("-INF"),  "NAN",   "NAN", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),  "INF",   "NAN", Decimal::DEFAULT_PRECISION], // <-- No exception, technically invalid.
    [Decimal::valueOf("-INF"), "-INF",   "NAN", Decimal::DEFAULT_PRECISION], // <-- No exception, technically invalid.
    [Decimal::valueOf( "NAN"),  1,       "NAN", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  1,       "NAN", Decimal::DEFAULT_PRECISION], // <-- No exception, technically invalid.
    [Decimal::valueOf("-INF"),  1,       "NAN", Decimal::DEFAULT_PRECISION], // <-- No exception, technically invalid.
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $precision) = $test;

    $result = $op1->rem($op2);

    if ([(string) $result, $result->precision()] !== [(string) $expect, $precision]) {
        print_r(compact("index", "op1", "op2", "result", "expect", "precision"));
    }
}

/* Test immutable */
$number = Decimal::valueOf("2");
$number->rem(5);

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

/* Check division by zero */
try {
    Decimal::valueOf(0)->rem(0);
} catch (DivisionByZeroError $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(1)->rem(0);
} catch (DivisionByZeroError $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(NAN)->rem(0);
} catch (DivisionByZeroError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(INF)->rem(0);
} catch (DivisionByZeroError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(-INF)->rem(0);
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
