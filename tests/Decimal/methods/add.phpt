--TEST--
Decimal::add
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, op2, expected result, expected precision
 */
$tests = [
    [
        Decimal::valueOf("9.0E+9"),
        Decimal::valueOf("9.0E+9"),
                         "18000000000", Decimal::DEFAULT_PRECISION,
    ],
    [
        Decimal::valueOf("1.1111", 1),
        Decimal::valueOf("3.3333", 2),
                         "4",      1,
    ],
    [
        Decimal::valueOf("0.1"),
        Decimal::valueOf("0.1"),
                         "0.2", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("-0.1"),
        Decimal::valueOf( "0.1"),
                          "0.0", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf( "0.1"),
        Decimal::valueOf("-0.1"),
                          "0.0", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("-0.1"),
        Decimal::valueOf("-0.1"),
                         "-0.2", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("0.11111111111111111111111111111", 20),
        Decimal::valueOf("0.22222222222222222222222222222", 12),
                         "0.333333333333",                  12,
    ],
    [
        Decimal::valueOf("0.11111111111111111111111111111",  2),
        Decimal::valueOf("0.22222222222222222222222222222", 16),
                         "0.33",                             2,
    ],
    [
        Decimal::valueOf("5"), 10, "15", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("5", 2), 10, "15", 2
    ],
    [
        Decimal::valueOf("5", 50), 10, "15", 50
    ],
    [
        $a = Decimal::valueOf("7", 50), $a, "14", 50
    ],
    [
        Decimal::valueOf(PHP_INT_MAX), Decimal::valueOf(PHP_INT_MIN), "-1", Decimal::DEFAULT_PRECISION,
    ],

    /* Special numbers */
    [Decimal::valueOf( "NAN"),  "NAN", (string)  (NAN +  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"),  "INF", (string)  (NAN +  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"), "-INF", (string)  (NAN + -INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "NAN", (string)  (INF +  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "INF", (string)  (INF +  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"), "-INF", (string)  (INF + -INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),  "NAN", (string) (-INF +  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),  "INF", (string) (-INF +  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"), "-INF", (string) (-INF + -INF), Decimal::DEFAULT_PRECISION],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $prec) = $test;

    $results = [
        $op1->add($op2),

        $op1 + $op2,
        $op2 + $op1,
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
$number->add(5);
$number + 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

?>
--EXPECT--
