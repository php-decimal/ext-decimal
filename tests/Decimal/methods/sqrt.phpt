--TEST--
Decimal::sqrt
--FILE--
<?php
use Decimal\Decimal;

$tests = [
    [Decimal::valueOf(0), "0", Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("0.1", 5),    "0.31623",                                              5],
    [Decimal::valueOf("0.1", 50),   "0.31622776601683793319988935444327185337195551393252", 50],
    [Decimal::valueOf("0.1"),       "0.3162277660168379331998893544432719",                 Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("145", 5),    "12.042",                                               5],
    [Decimal::valueOf("145", 50),   "12.041594578792295480128241030378608052425352405054",  50],
    [Decimal::valueOf("145"),       "12.04159457879229548012824103037861",                  Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("144", 5),    "12",       5],
    [Decimal::valueOf("144", 50),   "12",       50],
    [Decimal::valueOf("144"),       "12",       Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf( "1E-10"),    "0.00001",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1E-10"),    "NAN",      Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf( "NAN"),      "NAN",      Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"),      "INF",      Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"),      "NAN",      Decimal::DEFAULT_PRECISION],
];

foreach ($tests as $test) {
    list($op1, $expect, $prec) = $test;

    $result = $op1->sqrt();

    if ((string) $result !== (string) $expect || $result->precision() !== $prec) {
        print_r(compact("op1", "result", "expect", "prec"));
    }
}

/* Test immutable */
$number = Decimal::valueOf("1.5");
$result = $number->sqrt();

if ((string) $number !== "1.5") {
    var_dump("Mutated!", compact("number"));
}

?>
--EXPECT--
