--TEST--
Decimal::div
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, op2, expected result, expected precision
 */
$tests = [
    [
        Decimal::valueOf("1.2345678901234567890123456789" /* default */),
        Decimal::valueOf("9.8765432109876543210987654321" /* default */),
                         "0.1249999988609375000142382812498220", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("1.2345678901234567890123456789", 20),
        Decimal::valueOf("9.8765432109876543210987654321", 12),
                         "0.124999998861",                 12,
    ],
    [
        Decimal::valueOf("1.2345678901234567890123456789",  5),
        Decimal::valueOf("9.8765432109876543210987654321", 20),
                         "0.12500",                         5,
    ],
    [
        Decimal::valueOf("1.1111", 1),
        Decimal::valueOf("3.3333", 2),
                         "0.3",    1,
    ],
    [
        Decimal::valueOf("0.3" /* default */),
        Decimal::valueOf("0.1" /* default */),
                         "3", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("-0.3" /* default */),
        Decimal::valueOf( "0.1" /* default */),
                         "-3", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("15"), 10, "1.5", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("15", 2), 10, "1.5", 2
    ],
    [
        Decimal::valueOf("15", 50), 10, "1.5", 50
    ],
    [
        $a = Decimal::valueOf("7", 50), $a, "1", 50
    ],

    /* Special numbers */
    [Decimal::valueOf(0),       "NAN", (string)  (  0 /  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf(0),       "INF", (string)  (  0 /  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf(0),      "-INF", (string)  (  0 / -INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"),  "NAN", (string)  (NAN /  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"),  "INF", (string)  (NAN /  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"), "-INF", (string)  (NAN / -INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "NAN", (string)  (INF /  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "INF", (string)  (INF /  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"), "-INF", (string)  (INF / -INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),  "NAN", (string) (-INF /  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),  "INF", (string) (-INF /  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"), "-INF", (string) (-INF / -INF), Decimal::DEFAULT_PRECISION],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $prec) = $test;

    $results = [
        $op1->div($op2),
        $op1 / $op2,
    ];

    foreach ($results as $result) {
        if ([(string) $result, $result->precision()] !== [$expect, $prec]) {
            print_r(compact("index", "op1", "op2", "result", "expect", "prec"));
            break;
        }
    }
}

/* Test immutable */
$number = Decimal::valueOf("2");
$number->div(5);
$number / 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

/* Test division by zero */
try {
    Decimal::valueOf(0) / 0;
} catch (DivisionByZeroError $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(1) / 0;
} catch (DivisionByZeroError $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(NAN) / 0;
} catch (DivisionByZeroError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(INF) / 0;
} catch (DivisionByZeroError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(-INF) / 0;
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
