--TEST--
Rational cast to string, bool, int, float
--FILE--
<?php
use Decimal\Rational;

$tests = [

    /**
     * STRING
     */
    [(string) Rational::valueOf(0),         "0"],
    [(string) Rational::valueOf("-0"),     "-0"],
    [(string) Rational::valueOf("5.2"),     "26/5"],

    [(string) Rational::valueOf( "NAN"),    "NAN"],
    [(string) Rational::valueOf( "INF"),    "INF"],
    [(string) Rational::valueOf("-INF"),   "-INF"],

    /**
     * BOOL
     */
    [(bool) Rational::valueOf(0),           true],
    [(bool) Rational::valueOf( "0.1"),      true],
    [(bool) Rational::valueOf("-0.1"),      true],

    [(bool) Rational::valueOf( "NAN"),      true],
    [(bool) Rational::valueOf( "INF"),      true],
    [(bool) Rational::valueOf("-INF"),      true],

    [(bool) Rational::valueOf( "1E-1000"),  true],
    [(bool) Rational::valueOf("-1E-1000"),  true],

    /**
     * INT
     */
    [(int) Rational::valueOf(0),            0],
    [(int) Rational::valueOf( "1E-1000"),   0],
    [(int) Rational::valueOf("-1E-1000"),   0],

    [(int) Rational::valueOf(PHP_INT_MAX),  PHP_INT_MAX],
    [(int) Rational::valueOf(PHP_INT_MIN),  PHP_INT_MIN],

    /**
     * FLOAT
     */
    [(float) Rational::valueOf(0),          0.0],
    [(float) Rational::valueOf("-0"),      -0.0],
    [(float) Rational::valueOf("-0.0"),    -0.0],
    [(float) Rational::valueOf( "0.1"),     0.1],
    [(float) Rational::valueOf( "0.5"),     0.5],

    [(float) Rational::valueOf("0.11111111111111111111"), (float) "0.11111111111111111111"],

    [(float) Rational::valueOf("1234.5678E+9"),     1.2345678E+12],
    [(float) Rational::valueOf("1234.5678E+90"),    1.2345678E+93],

    [(string) (float) Rational::valueOf( "NAN"),    "NAN"],
    [(string) (float) Rational::valueOf( "INF"),    "INF"],
    [(string) (float) Rational::valueOf("-INF"),   "-INF"],

    [(float) Rational::valueOf( INF),  INF],
    [(float) Rational::valueOf(-INF), -INF],

    /**
     * ARRAY
     */
    [(array) Rational::valueOf(0), []],
];

foreach ($tests as $test) {
    list($result, $expect) = $test;

    if ($result !== $expect) {
        print_r(compact("result", "expect"));
    }
}

try {
    (int) Rational::valueOf("1E+1000");
} catch (OverflowException $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    (float) Rational::valueOf("1E-1000");
} catch (UnderflowException $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    (float) Rational::valueOf("-1E-1000");
} catch (UnderflowException $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    (int) Rational::valueOf(NAN);
} catch (RuntimeException $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    (int) Rational::valueOf(INF);
} catch (RuntimeException $e) {
    printf("E %s\n", $e->getMessage());
}

try {
    (int) Rational::valueOf(-INF);
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
