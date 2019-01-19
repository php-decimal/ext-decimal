--TEST--
Rational::mul
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
    [Rational::valueOf( "0"), "-0", "-0"],
    [Rational::valueOf( "0"), "-0", "-0"],
    [Rational::valueOf( "0"),  "0",  "0"],
    [Rational::valueOf("-0"), "-0",  "0"],
    [Rational::valueOf("-0"),  "0", "-0"],

    [Rational::valueOf( "1"), "-0", "-0"],
    [Rational::valueOf( "1"), "-0", "-0"],
    [Rational::valueOf( "1"),  "0",  "0"],
    [Rational::valueOf("-1"), "-0",  "0"],
    [Rational::valueOf("-1"),  "0", "-0"],

    [Rational::valueOf( "0"), "-1", "-0"],
    [Rational::valueOf( "0"), "-1", "-0"],
    [Rational::valueOf( "0"),  "1",  "0"],
    [Rational::valueOf("-0"), "-1",  "0"],
    [Rational::valueOf("-0"),  "1", "-0"],

    [Rational::valueOf( "1.5"),  "2.4",  "18/5"],
    [Rational::valueOf("-1.5"),  "2.4", "-18/5"],
    [Rational::valueOf( "1.5"), "-2.4", "-18/5"],
    [Rational::valueOf("-1.5"), "-2.4",  "18/5"],

    /* Special numbers */
    [Rational::valueOf( "NAN"),  "NAN", (string)  (NAN *  NAN)],
    [Rational::valueOf( "NAN"),  "INF", (string)  (NAN *  INF)],
    [Rational::valueOf( "NAN"), "-INF", (string)  (NAN * -INF)],
    [Rational::valueOf( "INF"),  "NAN", (string)  (INF *  NAN)],
    [Rational::valueOf( "INF"),  "INF", (string)  (INF *  INF)],
    [Rational::valueOf( "INF"), "-INF", (string)  (INF * -INF)],
    [Rational::valueOf("-INF"),  "NAN", (string) (-INF *  NAN)],
    [Rational::valueOf("-INF"),  "INF", (string) (-INF *  INF)],
    [Rational::valueOf("-INF"), "-INF", (string) (-INF * -INF)],

    [Rational::valueOf( "NAN"), 0, "NAN"],
    [Rational::valueOf( "INF"), 0, "NAN"],
    [Rational::valueOf("-INF"), 0, "NAN"],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->mul($op2),

        $op1 * $op2,
        $op2 * $op1,
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
$number->mul(5);
$number * 5;

if ((string) $number !== "2") {
    var_dump("Mutated!", compact("number"));
}

/**
 * This is a test that came up as a result of the bencharking code.
 */
$number = Rational::valueOf(1);
$number = $number *  "0.1";
$number = $number *  "0.0000123456789";
$number = $number *  "12345.6789";
$number = $number * "-98765.4321";
$number = $number *  "9.87654321";
$number = $number * "-1.23456789";
$number = $number *  "0.00000000123456789";
$number = $number * "-0.00000000123456789";
$number = $number *  "12345.6789";
$number = $number * "-12345.6789";

var_dump($number);

?>
--EXPECT--
object(Decimal\Rational)#36 (2) {
  ["num"]=>
  string(75) "426397624393172957537541485031321559583108704527981359123508334688202542589"
  ["den"]=>
  string(5) "1E+80"
}
