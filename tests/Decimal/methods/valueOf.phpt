--TEST--
Decimal::valueOf
--FILE--
<?php
use Decimal\Decimal;

$tests = [

    /**
     * Decimal::valueOf(value [, precision])
     */
    [[123,                         /* 34 */ ],    34,     "123"],
    [["0.2000",                    /* 34 */ ],    34,     "0.2000"],
    [["1E+900",                    /* 34 */ ],    34,     "1E+900"],
    [[Decimal::valueOf("0.1"),     /* 34 */ ],    34,     "0.1"],
    [[Decimal::valueOf("0.1", 30), /* 34 */ ],    30,     "0.1"],

    [[123,                                10],    10,     "123"],
    [["0.2000",                           10],    10,     "0.2000"],
    [["1E+900",                           10],    10,     "1E+900"],
    [[Decimal::valueOf("0.1"),            10],    10,     "0.1"],
    [[Decimal::valueOf("0.1", 30),        10],    10,     "0.1"],

    [[150,                                 1],     1,     "150"],
    [["0.2000",                            1],     1,     "0.2000"],
    [["0.20001",                           1],     1,     "0.20001"],
    [["1E+900",                            1],     1,     "1E+900"],
    [[Decimal::valueOf("0.1"),             1],     1,     "0.1"],
    [[Decimal::valueOf("0.1", 30),         1],     1,     "0.1"],

    /* Precision does not affect parsed scalar. */
    [["0.123456", 4], 4, "0.123456"],

    /* Precision is not preserved if a specific precision is given. */
    [[Decimal::valueOf("0.12345", 5), 1], 1, "0.12345"],

    /* Precision is preserved if a specific precision is not given. */
    [[Decimal::valueOf("0.12345", 50), /* 34 */], 50, "0.12345"],

    /* Use given decimal precision if greater or smaller than default. */
    [[Decimal::valueOf("0.1234", 50), /* 34 */], 50, "0.1234"],
    [[Decimal::valueOf("0.1234", 10), /* 34 */], 10, "0.1234"],

    [[ "nan"],  34,  "NAN"],
    [[ "inf"],  34,  "INF"],
    [["-inf"],  34, "-INF"],

    /* While float is not actually supported, these constants are an exception
       for the sake of developer sanity and convenience. */
    [[ NAN],  34,  "NAN"],
    [[ INF],  34,  "INF"],
    [[-INF],  34, "-INF"],

    /* Check precision bounds */
    [[0, Decimal::MAX_PRECISION], Decimal::MAX_PRECISION, "0"],
    [[0, Decimal::MIN_PRECISION], Decimal::MIN_PRECISION, "0"],
];

foreach ($tests as $index => $test) {
    list($args, $precision, $expect) = $test;

    $result = Decimal::valueOf(...$args);

    if ($result->precision() !== $precision || (string) $result !== $expect) {
        var_dump(compact("index", "args", "result", "precision", "expect"));
    }
}


try {
    Decimal::valueOf(" 1");
} catch (DomainException $e) {
    printf("A %s\n", $e->getMessage());
}

try {
    Decimal::valueOf("1 ");
} catch (DomainException $e) {
    printf("B %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(1.5);
} catch (TypeError $e) {
    printf("C %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(null);
} catch (TypeError $e) {
    printf("D %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(0, "b");
} catch (TypeError $e) {
    printf("E %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(0, null);
} catch (OutOfRangeException $e) {
    printf("F %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(0, 0);
} catch (OutOfRangeException $e) {
    printf("G %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(0, -1);
} catch (OutOfRangeException $e) {
    printf("H %s\n", $e->getMessage());
}

try {
    Decimal::valueOf(0, Decimal::MAX_PRECISION + 1);
} catch (OutOfRangeException $e) {
    printf("I %s\n", $e->getMessage());
}

try {
    (Decimal::valueOf(0))->__construct();
} catch (Error $e) {
    printf("J %s\n", $e->getMessage());
}

try {
    Decimal::valueOf();
} catch (Error $e) {
    printf("K %s\n", $e->getMessage());
}

?>
--EXPECTF--
A Failed to parse string as decimal: " 1"
B Failed to parse string as decimal: "1 "
C Decimal\Decimal::valueOf() expected parameter 1 to be a decimal number, string, or integer
D Decimal\Decimal::valueOf() expected parameter 1 to be a decimal number, string, or integer
E Argument 2 passed to Decimal\Decimal::valueOf() must be of the type in%s or null, string given
F Decimal precision out of range
G Decimal precision out of range
H Decimal precision out of range
I Decimal precision out of range
J Call to private method Decimal\Decimal::__construct() from context ''
K Decimal\Decimal::valueOf() expects at least 1 parameter, 0 given
