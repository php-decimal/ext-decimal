--TEST--
Decimal::ln
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, expected result, expected precision
 */
$tests = [
    [Decimal::valueOf("0.001"), "-6.907755278982137052053974364053093",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("0.01"),  "-4.605170185988091368035982909368728",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("0.1"),   "-2.302585092994045684017991454684364",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("1"),      "0",                                      Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("10"),     "2.302585092994045684017991454684364",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("100"),    "4.605170185988091368035982909368728",    Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("1000"),   "6.907755278982137052053974364053093",    Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("1E-1000"), "-2302.585092994045684017991454684364",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("1E-2000"), "-4605.170185988091368035982909368728",  Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("-0.001"), "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-0.01"),  "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-0.1"),   "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1"),     "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-10"),    "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-100"),   "NAN",  Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1000"),  "NAN",  Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf("123.456"),          "4.815884817283263883109232105166526",                 Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("123.456", 50),      "4.8158848172832638831092321051665255771721581355055", 50],

    [Decimal::valueOf("0.000123456"),     "-8.999625740681010220998716622939660",                 Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("0.000123456", 50), "-8.9996257406810102209987166229396596684344507962672", 50],

    /* Match PHP on special values */
    [Decimal::valueOf("-INF"), (string) log(-INF), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "NAN"), (string) log( NAN), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf( "INF"), (string) log( INF), Decimal::DEFAULT_PRECISION],

    [Decimal::valueOf( "0"), (string) log( 0), Decimal::DEFAULT_PRECISION],
    [Decimal::valueOf("-1"), (string) log(-1), Decimal::DEFAULT_PRECISION],
];

foreach ($tests as $test) {
    list($op1, $expect, $prec) = $test;

    $result = $op1->ln();

    if ((string) $result !== $expect || $result->precision() !== $prec) {
        print_r(compact("number", "result", "expect", "prec"));
    }
}

/* Test immutable */
$number = Decimal::valueOf("2");
$result = $number->ln();

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}
?>
--EXPECT--
