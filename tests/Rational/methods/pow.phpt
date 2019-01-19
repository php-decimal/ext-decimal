--TEST--
Rational::pow
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Rational;
use Decimal\Decimal;

$tests = [

    // Alternating signs with integer exponent
    [Rational::valueOf( "2.5"),   2, "25/4"],
    [Rational::valueOf( "2.5"),  -2, "4/25"],
    [Rational::valueOf("-2.5"),   2, "25/4"],
    [Rational::valueOf("-2.5"),  -2, "4/25"],

    [Rational::valueOf( "2.0"),   2, "4"],
    [Rational::valueOf( "2.0"),  -2, "1/4"],
    [Rational::valueOf("-2.0"),   2, "4"],
    [Rational::valueOf("-2.0"),  -2, "1/4"],

    // Power of 0 should always be 1
    [Rational::valueOf( "0.1"),   0, "1"],
    [Rational::valueOf("-0.1"),   0, "1"],
    [Rational::valueOf( "0"),     0, "1"],
    [Rational::valueOf( "1"),     0, "1"],
    [Rational::valueOf("-1"),     0, "1"],

    [Rational::valueOf( NAN), 0, "1"],
    [Rational::valueOf( INF), 0, "1"],
    [Rational::valueOf(-INF), 0, "1"],

    // Rational raised to another Rational::valueOf (truncate op2)
    [
        Rational::valueOf(13)->div(7),
        Rational::valueOf(8)->div(3),
        "169/49"
    ],

    // Rational raised to decimal (truncate op2)
    [
        Rational::valueOf(13)->div(7),
        "2.3",
        "169/49",
    ],

    // Rational raised to itself.
    [
        $a = Rational::valueOf("2.5"),
        $a,
        "25/4",
    ],

    // Decimal instance exponent
    [
        Rational::valueOf("2.5"),
        Decimal::valueOf("2.5"),
        "25/4",
    ],

    // Rational instance exponent
    [
        Rational::valueOf("2.5"),
        Rational::valueOf("2.5"),
        "25/4",
    ],


    // Match PHP where op1 is NAN
    [Rational::valueOf(NAN),      2,     (NAN ** 2)],
    [Rational::valueOf(NAN),     -2,     (NAN ** -2)],
    [Rational::valueOf(NAN),     "2.5",  (NAN ** 2.5)],
    [Rational::valueOf(NAN),     "-2.5", (NAN ** -2.5)],
    [Rational::valueOf(NAN),     "0.5",  (NAN ** 0.5)],
    [Rational::valueOf(NAN),     "-0.5", (NAN ** -0.5)],
    [Rational::valueOf(NAN),     NAN,    (NAN ** NAN)],
    [Rational::valueOf(NAN),     INF,    (NAN ** INF)],
    [Rational::valueOf(NAN),    -INF,    (NAN ** (-INF))],

    // Match PHP where op1 is INF
    [Rational::valueOf(INF),     2,      (INF ** 2)],
    [Rational::valueOf(INF),     -2,     (INF ** -2)],
    [Rational::valueOf(INF),     "2.5",  (INF ** 2.5)],
    [Rational::valueOf(INF),     "-2.5", (INF ** -2.5)],
    [Rational::valueOf(INF),     "0.5",  (INF ** 0.5)],
    [Rational::valueOf(INF),     "-0.5", (INF ** -0.5)],
    [Rational::valueOf(INF),     NAN,    (INF ** NAN)],
    [Rational::valueOf(INF),     INF,    (INF ** INF)],
    [Rational::valueOf(INF),    -INF,    (INF ** (-INF))],

    // Match PHP where op1 is -INF
    [Rational::valueOf(-INF),    2,      ((-INF) ** 2)],
    [Rational::valueOf(-INF),    -2,     ((-INF) ** -2)],
    [Rational::valueOf(-INF),    "2.5",  ((-INF) ** 2.5)],
    [Rational::valueOf(-INF),    "-2.5", ((-INF) ** -2.5)],
    [Rational::valueOf(-INF),    "0.5",  ((-INF) ** 0.5)],
    [Rational::valueOf(-INF),    "-0.5", ((-INF) ** -0.5)],
    [Rational::valueOf(-INF),    NAN,    ((-INF) ** NAN)],
    [Rational::valueOf(-INF),    INF,    ((-INF) ** INF)],
    [Rational::valueOf(-INF),   -INF,    ((-INF) ** (-INF))],
];

foreach ($tests as $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        Rational::valueOf($op1)->pow($op2),
    ];

    foreach ($results as $result) {
        if ((string) $result !== (string) $expect) {
            print_r(compact("op1", "op2", "result", "expect"));
            break;
        }
    }
}
?>
--EXPECTF--
Warning: Rational arithmetic operand truncated to integer in %s on line %d

Warning: Rational arithmetic operand truncated to integer in %s on line %d

Warning: Rational arithmetic operand truncated to integer in %s on line %d

Warning: Rational arithmetic operand truncated to integer in %s on line %d

Warning: Rational arithmetic operand truncated to integer in %s on line %d
