--TEST--
Decimal::sub
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, op2, expected result, expected precision
 */
$tests = [
    [
        Decimal::valueOf("8E+90000"  /* default */),
        Decimal::valueOf("5E+90000"  /* default */),
                         "3E+90000", Decimal::DEFAULT_PRECISION,
    ],
    [
        Decimal::valueOf("4.3333", 1),
        Decimal::valueOf("3.3333", 2),
                         "1",      1,
    ],
    [
        Decimal::valueOf("0.3"  /* default */),
        Decimal::valueOf("0.1"  /* default */),
                         "0.2", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("-0.3"  /* default */),
        Decimal::valueOf( "0.1"  /* default */),
                         "-0.4", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf( "0.1"  /* default */),
        Decimal::valueOf("-0.1"  /* default */),
                          "0.2", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("-0.1"  /* default */),
        Decimal::valueOf("-0.1"  /* default */),
                          "0.0", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("0.33333333333333333333333333333", 20),
        Decimal::valueOf("0.22222222222222222222222222222", 12),
                         "0.111111111111",                  12,
    ],
    [
        Decimal::valueOf("0.33333333333333333333333333333",  2),
        Decimal::valueOf("0.22222222222222222222222222222", 16),
                         "0.11",                             2,
    ],
    [
        Decimal::valueOf("15"), 10, "5", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("15", 2), 10, "5", 2
    ],
    [
        Decimal::valueOf("15", 50), 10, "5", 50
    ],
    [
        $a = Decimal::valueOf("7", 50), $a, "0", 50
    ],

    /* Special numbers */
    [Decimal::valueOf( "NAN"),  "NAN", (string)  (NAN -  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"),  "INF", (string)  (NAN -  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"), "-INF", (string)  (NAN - -INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "NAN", (string)  (INF -  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "INF", (string)  (INF -  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"), "-INF", (string)  (INF - -INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),  "NAN", (string) (-INF -  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),  "INF", (string) (-INF -  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"), "-INF", (string) (-INF - -INF), Decimal::DEFAULT_PRECISION],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $prec) = $test;

    $results = [
        $op1->sub($op2),
        $op1 - $op2,
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
$number->sub(5);
$number - 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
