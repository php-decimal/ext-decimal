--TEST--
Rational::toInt
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Rational;

$tests = [
    ["0",  0],

    ["-0.1",  0],
    [ "0.1",  0],

    ["-2.4", -2],
    ["-2.5", -2],
    ["-2.6", -2],

    [ "2.4",  2],
    [ "2.5",  2],
    [ "2.6",  2],

    [ "1E-50",  0],
    ["-1E-50",  0],

    [ "NAN",  0],
    [ "INF",  0],
    ["-INF",  0],

    ["1E+1000", PHP_INT_MAX], // Exception

    [PHP_INT_MAX, PHP_INT_MAX],
    [PHP_INT_MIN, PHP_INT_MIN],

    [(string) PHP_INT_MAX, PHP_INT_MAX],
    [(string) PHP_INT_MIN, PHP_INT_MIN],

    [PHP_INT_MAX + 1, null], // Exception
    [PHP_INT_MIN - 1, null], // Exception
];

foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];

    try {
        $result = Rational::valueOf($number)->toInt();
    } catch (Throwable $e) {
        printf("%s: %s\n", get_class($e), $e->getMessage());
        continue;
    }

    if ($result !== $expect) {
        print_r(compact("number", "result", "expect"));
    }
}

/* Test immutable */
$number = Rational::valueOf("2.5");
$result = $number->toInt();

if ((string) $number !== "5/2") {
    var_dump("Mutated!", compact("number"));
}

?>
--EXPECT--
RuntimeException: Converting NaN or Inf to integer is not defined
RuntimeException: Converting NaN or Inf to integer is not defined
RuntimeException: Converting NaN or Inf to integer is not defined
OverflowException: Integer overflow
TypeError: Decimal\Rational::valueOf() expected parameter 1 to be a decimal number, string, or integer
TypeError: Decimal\Rational::valueOf() expected parameter 1 to be a decimal number, string, or integer
