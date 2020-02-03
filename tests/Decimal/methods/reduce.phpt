--TEST--
Decimal::reduce
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, precision, expected result
 */
$tests = [
   [ "0.123",    1,   "0.1"],
   [ "12300",    1,   "10000"],

   [ "-0.123",   1,   "-0.1"],
   [ "-12300",   1,   "-10000"],

   [ "0.123",    2,   "0.12"],
   [ "12300",    2,   "12000"],

   [ "-0.123",   2,   "-0.12"],
   [ "-12300",   2,   "-12000"],

   [ "0.123",   20,   "0.123"],
   [ "12300",   20,   "12300"],

   [ "-0.123",  20,   "-0.123"],
   [ "-12300",  20,   "-12300"],

   /* Test that reduce does not trim. */
   [ "1.2000",   3,   "1.20"],
   [ "1.2000",  20,   "1.2000"],

   /* Special numbers should not be affected. */
   [ "NAN",      1,   "NAN"],
   [ "INF",      1,   "INF"],
   ["-INF",      1,  "-INF"],
];

foreach ($tests as $test) {
    list($number, $prec, $expect) = $test;

    try {
      $result = Decimal::valueOf($number, $prec)->reduce();
    } catch (\Throwable $e) {
      printf("%s: %s\n", get_class($e), $e->getMessage());
      continue;
    }

    if ((string) $result !== $expect) {
        print_r(compact("number", "prec", "result", "expect"));
    }
}
?>
--EXPECT--
