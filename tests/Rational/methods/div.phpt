--TEST--
Rational::div
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
    [
        Rational::valueOf("1.2345678901234567890123456789"),
        Rational::valueOf("9.8765432109876543210987654321"),
                           "13717421" . "/" .
                          "109739369"
    ],
    [
        Rational::valueOf("1.1111"),
        Rational::valueOf("3.3333"),
                          "1" . "/" .
                          "3"
    ],
    [
        Rational::valueOf("0.3"),
        Rational::valueOf("0.1"),
                          "3"
    ],
    [
        Rational::valueOf("-0.3"),
        Rational::valueOf( "0.1"),
                          "-3",
    ],
    [
        $a = Rational::valueOf("7"), $a, "1"
    ],

    /* Special numbers */
    [Rational::valueOf(0),       "NAN", (string)  (0   /  NAN)],
    [Rational::valueOf(0),       "INF", (string)  (0   /  INF)],
    [Rational::valueOf(0),      "-INF", (string)  (0   / -INF)],
    [Rational::valueOf( "NAN"),  "NAN", (string)  (NAN /  NAN)],
    [Rational::valueOf( "NAN"),  "INF", (string)  (NAN /  INF)],
    [Rational::valueOf( "NAN"), "-INF", (string)  (NAN / -INF)],
    [Rational::valueOf( "INF"),  "NAN", (string)  (INF /  NAN)],
    [Rational::valueOf( "INF"),  "INF", (string)  (INF /  INF)],
    [Rational::valueOf( "INF"), "-INF", (string)  (INF / -INF)],
    [Rational::valueOf("-INF"),  "NAN", (string) (-INF /  NAN)],
    [Rational::valueOf("-INF"),  "INF", (string) (-INF /  INF)],
    [Rational::valueOf("-INF"), "-INF", (string) (-INF / -INF)],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->div($op2),
        $op1 / $op2,
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
$number->div(5);
$number / 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

/* Test division by zero */
try {
    Rational::valueOf(0) / 0;
} catch (DivisionByZeroError $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    Rational::valueOf(1) / 0;
} catch (DivisionByZeroError $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    Rational::valueOf(NAN) / 0;
} catch (DivisionByZeroError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    Rational::valueOf(INF) / 0;
} catch (DivisionByZeroError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    Rational::valueOf(-INF) / 0;
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
