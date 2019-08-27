--TEST--
Decimal::round
--FILE--
<?php
use Decimal\Decimal;

$tests = [
    /* number, decimal places, rounding mode(s), expected result */

    /* HALF UP, .5 AWAY FROM ZERO */
    [ "1.1", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1"],
    [ "1.5", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "2"],
    [ "1.9", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "2"],
    ["-1.1", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1"],
    ["-1.5", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-2"],
    ["-1.9", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1.235"],
    [ "1.2349", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1.235"],
    ["-1.2349", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-1.235"],

    [ "0", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "0"],
    [ "3", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "3"],
    [ "4", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "4"],
    ["-3", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-3"],
    ["-4", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-4"],

    [ "NAN", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP],  "INF"],
    ["-INF", 0, [Decimal::ROUND_HALF_UP, PHP_ROUND_HALF_UP], "-INF"],


    /* HALF DOWN, .5 TOWARDS ZERO */
    [ "1.1", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1"],
    [ "1.5", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1"],
    [ "1.9", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "2"],
    ["-1.1", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1"],
    ["-1.5", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1"],
    ["-1.9", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-1.235"],

    [ "0", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "0"],
    [ "3", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "3"],
    [ "4", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "4"],
    ["-3", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-3"],
    ["-4", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-4"],

    [ "NAN", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN],  "INF"],
    ["-INF", 0, [Decimal::ROUND_HALF_DOWN, PHP_ROUND_HALF_DOWN], "-INF"],


    /* HALF EVEN, .5 TO NEAREST EVEN NUMBER */
    [ "1.1", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1"],
    [ "1.5", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "2"],
    [ "1.9", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "2"],
    ["-1.1", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1"],
    ["-1.5", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-2"],
    ["-1.9", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-1.235"],

    [ "0", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "0"],
    [ "3", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "3"],
    [ "4", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "4"],
    ["-3", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-3"],
    ["-4", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-4"],

    [ "NAN", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN],  "INF"],
    ["-INF", 0, [Decimal::ROUND_HALF_EVEN, PHP_ROUND_HALF_EVEN], "-INF"],


    /* HALF ODD, .5 TO NEAREST ODD NUMBER */
    [ "1.1", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1"],
    [ "1.5", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1"],
    [ "1.9", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "2"],
    ["-1.1", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1"],
    ["-1.5", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1"],
    ["-1.9", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1.235"],
    [ "1.2349", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1.235"],
    ["-1.2349", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-1.235"],

    [ "0", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "0"],
    [ "3", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "3"],
    [ "4", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "4"],
    ["-3", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-3"],
    ["-4", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-4"],

    [ "NAN", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD],  "INF"],
    ["-INF", 0, [Decimal::ROUND_HALF_ODD, PHP_ROUND_HALF_ODD], "-INF"],


    /* UP, AWAY FROM ZERO */
    [ "1.1", 0, [Decimal::ROUND_UP],  "2"],
    [ "1.5", 0, [Decimal::ROUND_UP],  "2"],
    [ "1.9", 0, [Decimal::ROUND_UP],  "2"],
    ["-1.1", 0, [Decimal::ROUND_UP], "-2"],
    ["-1.5", 0, [Decimal::ROUND_UP], "-2"],
    ["-1.9", 0, [Decimal::ROUND_UP], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_UP],  "1.235"],
    [ "1.2345", 3, [Decimal::ROUND_UP],  "1.235"],
    [ "1.2349", 3, [Decimal::ROUND_UP],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_UP], "-1.235"],
    ["-1.2345", 3, [Decimal::ROUND_UP], "-1.235"],
    ["-1.2349", 3, [Decimal::ROUND_UP], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_UP],  "1.235"],
    ["-1.23455", 3, [Decimal::ROUND_UP], "-1.235"],

    [ "0", 0, [Decimal::ROUND_UP],  "0"],
    [ "3", 0, [Decimal::ROUND_UP],  "3"],
    [ "4", 0, [Decimal::ROUND_UP],  "4"],
    ["-3", 0, [Decimal::ROUND_UP], "-3"],
    ["-4", 0, [Decimal::ROUND_UP], "-4"],

    [ "NAN", 0, [Decimal::ROUND_UP],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_UP],  "INF"],
    ["-INF", 0, [Decimal::ROUND_UP], "-INF"],


    // /* DOWN, TOWARDS ZERO */
    [ "1.1", 0, [Decimal::ROUND_DOWN],  "1"],
    [ "1.5", 0, [Decimal::ROUND_DOWN],  "1"],
    [ "1.9", 0, [Decimal::ROUND_DOWN],  "1"],
    ["-1.1", 0, [Decimal::ROUND_DOWN], "-1"],
    ["-1.5", 0, [Decimal::ROUND_DOWN], "-1"],
    ["-1.9", 0, [Decimal::ROUND_DOWN], "-1"],

    [ "1.2341", 3, [Decimal::ROUND_DOWN],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_DOWN],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_DOWN],  "1.234"],
    ["-1.2341", 3, [Decimal::ROUND_DOWN], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_DOWN], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_DOWN], "-1.234"],

    [ "1.23455", 3, [Decimal::ROUND_DOWN],  "1.234"],
    ["-1.23455", 3, [Decimal::ROUND_DOWN], "-1.234"],

    [ "0", 0, [Decimal::ROUND_DOWN],  "0"],
    [ "3", 0, [Decimal::ROUND_DOWN],  "3"],
    [ "4", 0, [Decimal::ROUND_DOWN],  "4"],
    ["-3", 0, [Decimal::ROUND_DOWN], "-3"],
    ["-4", 0, [Decimal::ROUND_DOWN], "-4"],

    [ "NAN", 0, [Decimal::ROUND_DOWN],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_DOWN],  "INF"],
    ["-INF", 0, [Decimal::ROUND_DOWN], "-INF"],


    /* CEILING, TOWARDS +INF */
    [ "1.1", 0, [Decimal::ROUND_CEILING],  "2"],
    [ "1.5", 0, [Decimal::ROUND_CEILING],  "2"],
    [ "1.9", 0, [Decimal::ROUND_CEILING],  "2"],
    ["-1.1", 0, [Decimal::ROUND_CEILING], "-1"],
    ["-1.5", 0, [Decimal::ROUND_CEILING], "-1"],
    ["-1.9", 0, [Decimal::ROUND_CEILING], "-1"],

    [ "1.2341", 3, [Decimal::ROUND_CEILING],  "1.235"],
    [ "1.2345", 3, [Decimal::ROUND_CEILING],  "1.235"],
    [ "1.2349", 3, [Decimal::ROUND_CEILING],  "1.235"],
    ["-1.2341", 3, [Decimal::ROUND_CEILING], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_CEILING], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_CEILING], "-1.234"],

    [ "1.23455", 3, [Decimal::ROUND_DOWN],  "1.234"],
    ["-1.23455", 3, [Decimal::ROUND_DOWN], "-1.234"],

    [ "0", 0, [Decimal::ROUND_CEILING],  "0"],
    [ "3", 0, [Decimal::ROUND_CEILING],  "3"],
    [ "4", 0, [Decimal::ROUND_CEILING],  "4"],
    ["-3", 0, [Decimal::ROUND_CEILING], "-3"],
    ["-4", 0, [Decimal::ROUND_CEILING], "-4"],

    [ "NAN", 0, [Decimal::ROUND_CEILING],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_CEILING],  "INF"],
    ["-INF", 0, [Decimal::ROUND_CEILING], "-INF"],


    /* FLOOR, TOWARDS -INF */
    [ "1.1", 0, [Decimal::ROUND_FLOOR],  "1"],
    [ "1.5", 0, [Decimal::ROUND_FLOOR],  "1"],
    [ "1.9", 0, [Decimal::ROUND_FLOOR],  "1"],
    ["-1.1", 0, [Decimal::ROUND_FLOOR], "-2"],
    ["-1.5", 0, [Decimal::ROUND_FLOOR], "-2"],
    ["-1.9", 0, [Decimal::ROUND_FLOOR], "-2"],

    [ "1.2341", 3, [Decimal::ROUND_FLOOR],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_FLOOR],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_FLOOR],  "1.234"],
    ["-1.2341", 3, [Decimal::ROUND_FLOOR], "-1.235"],
    ["-1.2345", 3, [Decimal::ROUND_FLOOR], "-1.235"],
    ["-1.2349", 3, [Decimal::ROUND_FLOOR], "-1.235"],

    [ "1.23455", 3, [Decimal::ROUND_DOWN],  "1.234"],
    ["-1.23455", 3, [Decimal::ROUND_DOWN], "-1.234"],

    [ "0", 0, [Decimal::ROUND_FLOOR],  "0"],
    [ "3", 0, [Decimal::ROUND_FLOOR],  "3"],
    [ "4", 0, [Decimal::ROUND_FLOOR],  "4"],
    ["-3", 0, [Decimal::ROUND_FLOOR], "-3"],
    ["-4", 0, [Decimal::ROUND_FLOOR], "-4"],

    [ "NAN", 0, [Decimal::ROUND_FLOOR],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_FLOOR],  "INF"],
    ["-INF", 0, [Decimal::ROUND_FLOOR], "-INF"],


    /* TRUNCATE */
    [ "1.1", 0, [Decimal::ROUND_TRUNCATE],  "1"],
    [ "1.5", 0, [Decimal::ROUND_TRUNCATE],  "1"],
    [ "1.9", 0, [Decimal::ROUND_TRUNCATE],  "1"],
    ["-1.1", 0, [Decimal::ROUND_TRUNCATE], "-1"],
    ["-1.5", 0, [Decimal::ROUND_TRUNCATE], "-1"],
    ["-1.9", 0, [Decimal::ROUND_TRUNCATE], "-1"],

    [ "1.2341", 3, [Decimal::ROUND_TRUNCATE],  "1.234"],
    [ "1.2345", 3, [Decimal::ROUND_TRUNCATE],  "1.234"],
    [ "1.2349", 3, [Decimal::ROUND_TRUNCATE],  "1.234"],
    ["-1.2341", 3, [Decimal::ROUND_TRUNCATE], "-1.234"],
    ["-1.2345", 3, [Decimal::ROUND_TRUNCATE], "-1.234"],
    ["-1.2349", 3, [Decimal::ROUND_TRUNCATE], "-1.234"],

    [ "1.23455", 3, [Decimal::ROUND_TRUNCATE],  "1.234"],
    ["-1.23455", 3, [Decimal::ROUND_TRUNCATE], "-1.234"],

    [ "0", 0, [Decimal::ROUND_TRUNCATE],  "0"],
    [ "3", 0, [Decimal::ROUND_TRUNCATE],  "3"],
    [ "4", 0, [Decimal::ROUND_TRUNCATE],  "4"],
    ["-3", 0, [Decimal::ROUND_TRUNCATE], "-3"],
    ["-4", 0, [Decimal::ROUND_TRUNCATE], "-4"],

    [ "NAN", 0, [Decimal::ROUND_TRUNCATE],  "NAN"],
    [ "INF", 0, [Decimal::ROUND_TRUNCATE],  "INF"],
    ["-INF", 0, [Decimal::ROUND_TRUNCATE], "-INF"],

    /* ROUND NEGATIVE FIGURES */
    ["1.2345", -3, [Decimal::ROUND_HALF_UP], "0"],
    ["12345",  -3, [Decimal::ROUND_HALF_UP], "12000"],
    ["12345",  -9, [Decimal::ROUND_HALF_UP], "0"],

    /* ROUND TO CURRENT SIGNIFICANT FIGURES */
    ["0.12345", 5, [Decimal::ROUND_HALF_UP], "0.12345"],
    ["1.2345",  5, [Decimal::ROUND_HALF_UP], "1.23450"],
    ["12.345",  5, [Decimal::ROUND_HALF_UP], "12.34500"],
    ["123.45",  5, [Decimal::ROUND_HALF_UP], "123.45000"],
    ["1234.5",  5, [Decimal::ROUND_HALF_UP], "1234.50000"],
    ["12345",   5, [Decimal::ROUND_HALF_UP], "12345.00000"],

    /* ROUND BEYOND CURRENT SIGNIFICANT FIGURES */
    ["0.12345", 10, [Decimal::ROUND_HALF_UP], "0.1234500000"],
    ["1.2345",  10, [Decimal::ROUND_HALF_UP], "1.2345000000"],
    ["12.345",  10, [Decimal::ROUND_HALF_UP], "12.3450000000"],
    ["123.45",  10, [Decimal::ROUND_HALF_UP], "123.4500000000"],
    ["1234.5",  10, [Decimal::ROUND_HALF_UP], "1234.5000000000"],
    ["12345",   10, [Decimal::ROUND_HALF_UP], "12345.0000000000"],

    /**
     * Test rounding unreduced decimal values.
     */
    ["0.0500", 0, [Decimal::ROUND_HALF_UP], "0"],
    ["0.0500", 1, [Decimal::ROUND_HALF_UP], "0.1"],
    ["0.0500", 2, [Decimal::ROUND_HALF_UP], "0.05"],
    ["0.0500", 3, [Decimal::ROUND_HALF_UP], "0.050"],

    ["5.0000", 0, [Decimal::ROUND_HALF_UP], "5"],
    ["5.0000", 1, [Decimal::ROUND_HALF_UP], "5.0"],
    ["5.0000", 2, [Decimal::ROUND_HALF_UP], "5.00"],
    ["5.0000", 3, [Decimal::ROUND_HALF_UP], "5.000"],
];

foreach ($tests as $test) {
    list($number, $places, $modes, $expect) = $test;

    /* Check to see if there is an equivalent PHP constant */
    foreach ($modes as $mode) {
        $result = (string) (Decimal::valueOf($number))->round($places, $mode);

        if ($result !== $expect) {
            print_r(compact("number", "places", "mode", "result", "expect"));
        }
    }
}

/* Test immutable */
$number = Decimal::valueOf("1.2345");
$result = $number->round();

if ((string) $number !== "1.2345") {
    var_dump("Mutated!", compact("number"));
}

/**
 * Test that rounding mode fails even if already rounded.
 */
try {
    Decimal::valueOf("0.1234")->round(2, -1);
} catch (InvalidArgumentException $e) {
    printf("A %s\n", $e->getMessage());
}

/**
 * Test rounding a value beyond E_MAX
 */
try {
    Decimal::valueOf("0.1234")->round(PHP_INT_MAX);
} catch (\Exception $e) {
    printf("B %s\n", $e->getMessage());
}

/**
 * Test invalid rounding decimal places type
 */
try {
    Decimal::valueOf("0.1234")->round("a");
} catch (TypeError $e) {
    printf("C %s\n", get_class($e));
}

/**
 * Test invalid rounding mode type
 */
try {
    Decimal::valueOf("0.1234")->round(2, "a");
} catch (TypeError $e) {
    printf("D %s\n", get_class($e));
}


?>
--EXPECTF--
A Unsupported rounding mode
B Failed to round decimal - exponent out of range
C TypeError
D TypeError
