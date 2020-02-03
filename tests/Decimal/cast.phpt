--TEST--
Decimal cast to string, bool, int, float
--FILE--
<?php
use Decimal\Decimal;

$tests = [

    /**
     * STRING
     */
    [(string) Decimal::valueOf(0),               "0"],
    [(string) Decimal::valueOf("-0"),           "-0"],
    [(string) Decimal::valueOf("5.2"),           "5.2"],

    [(string) Decimal::valueOf( "NAN"),          "NAN"],
    [(string) Decimal::valueOf( "INF"),          "INF"],
    [(string) Decimal::valueOf("-INF"),         "-INF"],

    /**
     * BOOL
     */
    [(bool) Decimal::valueOf(0),                 true],
    [(bool) Decimal::valueOf( "0.1"),            true],
    [(bool) Decimal::valueOf("-0.1"),            true],

    [(bool) (Decimal::valueOf( "NAN")),          true],
    [(bool) (Decimal::valueOf( "INF")),          true],
    [(bool) (Decimal::valueOf("-INF")),          true],

    [(bool) Decimal::valueOf( "1E-1000"),        true],
    [(bool) Decimal::valueOf("-1E-1000"),        true],

    /**
     * INT
     */
    [(int) Decimal::valueOf(0),                  0],
    [(int) Decimal::valueOf( "1E-1000"),         0],
    [(int) Decimal::valueOf("-1E-1000"),         0],

    [(int) Decimal::valueOf(PHP_INT_MAX),        PHP_INT_MAX],
    [(int) Decimal::valueOf(PHP_INT_MIN),        PHP_INT_MIN],

    /**
     * FLOAT
     */
    [(float) Decimal::valueOf(0),                        0.0],
    [(float) Decimal::valueOf("-0"),                    -0.0],
    [(float) Decimal::valueOf("-0.0"),                  -0.0],
    [(float) Decimal::valueOf( "0.1"),                   0.1],
    [(float) Decimal::valueOf( "0.5"),                   0.5],

    [(float) Decimal::valueOf("0.11111111111111111111"), (float) "0.11111111111111111111"],

    [(float) Decimal::valueOf("1234.5678E+9"),           1.2345678E+12],
    [(float) Decimal::valueOf("1234.5678E+90"),          1.2345678E+93],

    [(string) (float) Decimal::valueOf( "NAN"),          "NAN"],
    [(string) (float) Decimal::valueOf( "INF"),          "INF"],
    [(string) (float) Decimal::valueOf("-INF"),         "-INF"],

    [(float) Decimal::valueOf( INF),  INF],
    [(float) Decimal::valueOf(-INF), -INF],

    /**
     * ARRAY
     */
    [(array) Decimal::valueOf(0), []],
];

foreach ($tests as $test) {
    list($result, $expect) = $test;

    if ($result !== $expect) {
        print_r(compact("result", "expect"));
    }
}

try {
    (int) Decimal::valueOf("1E+1000");
} catch (OverflowException $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    (float) Decimal::valueOf("1E-1000");
} catch (UnderflowException $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    (float) Decimal::valueOf("-1E-1000");
} catch (UnderflowException $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    (int) Decimal::valueOf(NAN);
} catch (RuntimeException $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    (int) Decimal::valueOf(INF);
} catch (RuntimeException $e) {
    printf("E %s\n", $e->getMessage());
}

try {
    (int) Decimal::valueOf(-INF);
} catch (RuntimeException $e) {
    printf("F %s\n", $e->getMessage());
}

?>
--EXPECT--
A Integer overflow
B Floating point underflow
C Floating point underflow
D Converting NaN or Infinity to integer is undefined
E Converting NaN or Infinity to integer is undefined
F Converting NaN or Infinity to integer is undefined
