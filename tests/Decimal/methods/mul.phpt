--TEST--
Decimal::mul
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
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
                        "12.19326311370217952261850327336229", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("1.2345678901234567890123456789", 20),
        Decimal::valueOf("9.8765432109876543210987654321", 12),
                        "12.1932631137",                   12
    ],
    [
        Decimal::valueOf("1.2345678901234567890123456789",  5),
        Decimal::valueOf("9.8765432109876543210987654321", 20),
                        "12.193",                           5
    ],
    [
        Decimal::valueOf("1.3333", 1),
        Decimal::valueOf("3.3333", 2),
                         "4",      1,
    ],
    [
        Decimal::valueOf("0.3" /* default */),
        Decimal::valueOf("0.1" /* default */),
                         "0.03", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("-0.3" /* default */),
        Decimal::valueOf( "0.1" /* default */),
                         "-0.03", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("15"), 10, "150", Decimal::DEFAULT_PRECISION
    ],
    [
        Decimal::valueOf("15", 2), 10, "150", 2
    ],
    [
        Decimal::valueOf("15", 50), 10, "150", 50
    ],
    [
        Decimal::valueOf("0.619207", 6), 10000000, "6192070", 6
    ],
    [
        Decimal::valueOf("0.619207"), 10000000, "6192070.000000", Decimal::DEFAULT_PRECISION
    ],
    [
        $a = Decimal::valueOf("7", 50), $a, "49", 50
    ],

    /* Special numbers */
    [Decimal::valueOf( "NAN"),  "NAN", (string)  (NAN *  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"),  "INF", (string)  (NAN *  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"), "-INF", (string)  (NAN * -INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "NAN", (string)  (INF *  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),  "INF", (string)  (INF *  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"), "-INF", (string)  (INF * -INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),  "NAN", (string) (-INF *  NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),  "INF", (string) (-INF *  INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"), "-INF", (string) (-INF * -INF), Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf( "NAN"), 0, "NAN", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"), 0, "NAN", Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"), 0, "NAN", Decimal::DEFAULT_PRECISION],

    /* Test that scalar operands are parsed at max precision. */
    [Decimal::valueOf("0.5", 9), "0.1111111", "0.05555555", 9],
    [Decimal::valueOf("0.5", 2), "0.1111111", "0.056",      2],
                                /*   ^ would be parsed as 0.11 otherwise,
                                       producing an incorrect result of 0.055 */
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect, $prec) = $test;

    $results = [
        $op1->mul($op2),

        $op1 * $op2,
        $op2 * $op1,
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
$number->mul(5);
$number * 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

?>
--EXPECT--
