--TEST--
Decimal cast to string, bool, int, float
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
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

    [(int) Decimal::valueOf( NAN),               0],
    [(int) Decimal::valueOf( INF),               0],
    [(int) Decimal::valueOf(-INF),               0],

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
    printf("%s\n", $e->getMessage());
}

try {
    (float) Decimal::valueOf("1E-1000");
} catch (UnderflowException $e) {
    printf("%s\n", $e->getMessage());
}

try {
    (float) Decimal::valueOf("-1E-1000");
} catch (UnderflowException $e) {
    printf("%s\n", $e->getMessage());
}

?>
--EXPECT--
Integer overflow
Floating point underflow
Floating point underflow
