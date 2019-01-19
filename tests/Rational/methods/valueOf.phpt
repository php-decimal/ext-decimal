--TEST--
Rational::valueOf
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Rational;

$tests = [

    /**
     * value, expected result
     */
    // [[123,                         /* 34 */ ],    34,     "123"],
    // [["0.2000",                    /* 34 */ ],    34,     "0.2000"],
    // [["1E+900",                    /* 34 */ ],    34,     "1E+900"],
    // [[Rational::valueOf("0.1"),     /* 34 */ ],    34,     "0.1"],
    // [[Rational::valueOf("0.1", 30), /* 34 */ ],    30,     "0.1"],

    // [[123,                                10],    10,     "123"],
    // [["0.2000",                           10],    10,     "0.2000"],
    // [["1E+900",                           10],    10,     "1E+900"],
    // [[Rational::valueOf("0.1"),            10],    10,     "0.1"],
    // [[Rational::valueOf("0.1", 30),        10],    10,     "0.1"],

    // [[150,                                 1],     1,     "150"],
    // [["0.2000",                            1],     1,     "0.2000"],
    // [["0.20001",                           1],     1,     "0.20001"],
    // [["1E+900",                            1],     1,     "1E+900"],
    // [[Rational::valueOf("0.1"),             1],     1,     "0.1"],
    // [[Rational::valueOf("0.1", 30),         1],     1,     "0.1"],

    // /* Precision does not affect parsed scalar. */
    // [["0.123456", 4], 4, "0.123456"],

    // /* Precision is not preserved if a specific precision is given. */
    // [[Rational::valueOf("0.12345", 5), 1], 1, "0.12345"],

    // /* Precision is preserved if a specific precision is not given. */
    // [[Rational::valueOf("0.12345", 50), /* 34 */], 50, "0.12345"],

    // /* Use given decimal precision if greater or smaller than default. */
    // [[Rational::valueOf("0.1234", 50), /* 34 */], 50, "0.1234"],
    // [[Rational::valueOf("0.1234", 10), /* 34 */], 10, "0.1234"],

    // [[ "nan"],  34,  "NAN"],
    // [[ "inf"],  34,  "INF"],
    // [["-inf"],  34, "-INF"],

    // /* While float is not actually supported, these constants are an exception
    //    for the sake of developer sanity and convenience. */
    // [[ NAN],  34,  "NAN"],
    // [[ INF],  34,  "INF"],
    // [[-INF],  34, "-INF"],

    // /* Check precision bounds */
    // [[0, Decimal::MAX_PRECISION], Decimal::MAX_PRECISION, "0"],
    // [[0, Decimal::MIN_PRECISION], Decimal::MIN_PRECISION, "0"],
];

foreach ($tests as $index => $test) {
    list($number, $expect) = $test;

    $result = Rational::valueOf($number);

    if ((string) $result !== (string) $expect) {
        var_dump(compact("index", "number", "result", "expect"));
    }
}

try {
    Rational::valueOf(" 1");
} catch (DomainException $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    Rational::valueOf("1 ");
} catch (DomainException $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    Rational::valueOf(1.5);
} catch (TypeError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    Rational::valueOf(null);
} catch (TypeError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    (Rational::valueOf(0))->__construct();
} catch (Error $e) {
    printf("E %s\n", $e->getMessage());
}

try {
    Rational::valueOf();
} catch (Error $e) {
    printf("F %s\n", $e->getMessage());
}

?>
--EXPECTF--
A Failed to parse string as decimal: " 1"
B Failed to parse string as decimal: "1 "
C Decimal\Rational::valueOf() expected parameter 1 to be a decimal number, string, or integer
D Decimal\Rational::valueOf() expected parameter 1 to be a decimal number, string, or integer
E Call to private method Decimal\Rational::__construct() from context ''
F Decimal\Rational::valueOf() expects exactly 1 parameter, 0 given
