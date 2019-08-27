--TEST--
Decimal::log10
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, expected result, expected precision
 */
$tests = [
    [Decimal::valueOf("0.001"), "-3",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("0.01"),  "-2",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("0.1"),   "-1",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("1"),      "0",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("10"),     "1",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("100"),    "2",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("1000"),   "3",    Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("-0.001"), "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-0.01"),  "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-0.1"),   "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1"),     "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-10"),    "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-100"),   "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1000"),  "NAN",  Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("123.456"),            "2.091512201627771681069399777067906",                 Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("123.456", 50),        "2.0915122016277716810693997770679057946535841876887", 50],

    [Decimal::valueOf("0.000123456"),       "-3.908487798372228318930600222932094",                 Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("0.000123456", 50),   "-3.9084877983722283189306002229320942053464158123113", 50],

    [Decimal::valueOf(   "0"), (string) log10(   0), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf(  "-1"), (string) log10(  -1), Decimal::DEFAULT_PRECISION],
    
    [Decimal::valueOf( "NAN"), (string) log10( NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"), (string) log10( INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-INF"), (string) log10(-INF), Decimal::DEFAULT_PRECISION],
];

foreach ($tests as $test) {
    list($op1, $expect, $prec) = $test;

    $result = $op1->log10();

    if ((string) $result !== $expect || $result->precision() !== $prec) {
        print_r(compact("number", "result", "expect", "prec"));
    }
}

/* Test immutable */
$number = Decimal::valueOf("2");
$result = $number->log10();

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
