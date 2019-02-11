--TEST--
Decimal::trim
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, expected result
 */
$tests = [
   [ "0.000",  "0"],
   [ "1.000",  "1"],
   [ "1.230",  "1.23"],
   ["-0.000", "-0"],
   ["-1.000", "-1"],
   ["-1.230", "-1.23"],

   [ "0",  "0"],
   [ "1",  "1"],
   ["-0", "-0"],
   ["-1", "-1"],

   [ "NAN",  "NAN"],
   [ "INF",  "INF"],
   ["-INF", "-INF"],
];

foreach ($tests as $test) {
    list($number, $expect) = $test;

    $result = Decimal::valueOf($number)->trim();

    if ((string) $result !== $expect) {
        print_r(compact("number", "result", "expect"));
    }
}
?>
--EXPECT--
