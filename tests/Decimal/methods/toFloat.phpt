--TEST--
Decimal::toFloat
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;


$tests = [
    ["0",        0.0],

    ["-0.1",    -0.1],
    [ "0.1",     0.1],

    ["-2.4",    -2.4],
    ["-2.5",    -2.5],
    ["-2.6",    -2.6],

    [ "2.4",     2.4],
    [ "2.5",     2.5],
    [ "2.6",     2.6],

    [ "1E-50",   1E-50],
    ["-1E-50",  -1E-50],

    ["-1E-1000", 0.0], // Warning

 /* [ "NAN",     NAN], Can"t test NAN because it does not equal itself */
    [ "INF",     INF],
    ["-INF",    -INF],
];

foreach ($tests as $test) {
    $number = $test[0];
    $expect = $test[1];

    try {
        $result = Decimal::valueOf($number)->toFloat();
    } catch (Throwable $e) {
        printf("%s: %s\n", get_class($e), $e->getMessage());
        continue;
    }

    if ($result !== $expect) {
        print_r(compact("number", "result", "expect"));
    }
}

/* Test immutable */
$number = Decimal::valueOf("2.5");
$result = $number->toFloat();

if ((string) $number !== "2.5") {
    var_dump("Mutated!", compact("number"));
}

/* Test NAN */
var_dump(Decimal::valueOf("NAN")->toFloat());

?>
--EXPECT--
UnderflowException: Floating point underflow
float(NAN)
