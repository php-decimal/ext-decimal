--TEST--
Decimal::compareTo
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

/**
 * op1, op2, expected result, expected precision
 */
$tests = [
    [$decimal = Decimal::valueOf("0.1"), $decimal, 0],

    [$decimal, &$decimal, 0],

    [Decimal::valueOf(0),       0,      0],
    [Decimal::valueOf(0),       null,   1],
    [Decimal::valueOf(0),       false,  1],
    [Decimal::valueOf(0),       0.0,    0],

    [Decimal::valueOf(1),       0,      1],
    [Decimal::valueOf(1),       null,   1],
    [Decimal::valueOf(1),       false,  1],
    [Decimal::valueOf(1),       0.0,    1],

    [Decimal::valueOf(NAN),     0,      1],
    [Decimal::valueOf(NAN),     null,   1],
    [Decimal::valueOf(NAN),     false,  1],
    [Decimal::valueOf(NAN),     0.0,    1],

    [Decimal::valueOf(0),       1,     -1],
    [Decimal::valueOf(0),       true,   0], // Objects are always equal to true.
    [Decimal::valueOf(0),       1.0,   -1],
    [Decimal::valueOf(0),       "1",   -1],

    [Decimal::valueOf(1),       1,      0],
    [Decimal::valueOf(1),       true,   0], // Objects are always equal to true.
    [Decimal::valueOf(1),       1.0,    0],
    [Decimal::valueOf(1),       "1",    0],

    [Decimal::valueOf(NAN),     1,      1],
    [Decimal::valueOf(NAN),     true,   0], // Objects are always equal to true.
    [Decimal::valueOf(NAN),     1.0,    1],
    [Decimal::valueOf(NAN),     "1",    1],

    [Decimal::valueOf(0),       NAN,    1],
    [Decimal::valueOf(1),       NAN,    1],

    [Decimal::valueOf("0.1"),   0.1,    0],
    [Decimal::valueOf("0.2"),   0.2,    0],
    [Decimal::valueOf("0.200"), 0.2,    0],
    [Decimal::valueOf("0.200"), 0.200,  0],
    [Decimal::valueOf("1E-10"), 1E-10,  0],
    [Decimal::valueOf("1E+10"), 1E+10,  0],

    [Decimal::valueOf("0.123", 3), "0.123",   0],
    [Decimal::valueOf("0.123", 3), "0.1234", -1], // Precision of op1 not used
    [Decimal::valueOf("0.123", 3), "0.12  ",  1],

    [Decimal::valueOf("0.1000000000000000000001"), 0.1,   1],
    [Decimal::valueOf("0.2000000000000000000001"), 0.2,   1],

    [Decimal::valueOf("0.0000000000000000000001"), 0.1,  -1],
    [Decimal::valueOf("0.0000000000000000000001"), 0.0,   1],

    [Decimal::valueOf("0.123", 3), Decimal::valueOf("0.1234", 3), -1],
    [Decimal::valueOf("0.123", 3), Decimal::valueOf("0.1234", 4), -1],

    /* Parse beyond the default precision. Should use max prec, not default.
       0.123
       0.12300...001
                  ^default prec cutoff
    */
    [Decimal::valueOf("0.123", 3), "0.123" . str_repeat("0", Decimal::DEFAULT_PRECISION) . "1", -1],

    [Decimal::valueOf("-0.123", 3), "-0.122", -1],
    [Decimal::valueOf("-0.123", 3), "-0.123",  0],
    [Decimal::valueOf("-0.123", 3), "-0.124",  1], // Numeric value, not lexicographic

    [Decimal::valueOf("-0.1"),     -0.1, 0],
    [Decimal::valueOf("-0.2"),     -0.2, 0],
    [Decimal::valueOf("-1E-10"), -1E-10, 0],
    [Decimal::valueOf("-1E+10"), -1E+10, 0],

    [Decimal::valueOf("0.1"),     "0.01",      1],
    [Decimal::valueOf("0.1"),     "0.1",       0],
    [Decimal::valueOf("0.1"),     "0.2",      -1],
    [Decimal::valueOf("0.2"),     "0.1",       1],
    [Decimal::valueOf("0.01"),    "0.1",      -1],

    [Decimal::valueOf("0.1"),     Decimal::valueOf("0.01"),      1],
    [Decimal::valueOf("0.1"),     Decimal::valueOf("0.1"),       0],
    [Decimal::valueOf("0.1"),     Decimal::valueOf("0.2"),      -1],
    [Decimal::valueOf("0.2"),     Decimal::valueOf("0.1"),       1],
    [Decimal::valueOf("0.01"),    Decimal::valueOf("0.1"),      -1],

    [Decimal::valueOf("0.1", 2),  Decimal::valueOf("0.1", 1),    1],
    [Decimal::valueOf("0.1", 2),  Decimal::valueOf("0.1", 2),    0],
    [Decimal::valueOf("0.1", 2),  Decimal::valueOf("0.1", 3),   -1],

    [Decimal::valueOf( "INF"),    Decimal::valueOf( "NAN"),      INF <=>  NAN],
    [Decimal::valueOf( "INF"),    Decimal::valueOf( "INF"),      INF <=>  INF],
    [Decimal::valueOf( "INF"),    Decimal::valueOf("-INF"),      INF <=> -INF],
    [Decimal::valueOf("-INF"),    Decimal::valueOf( "NAN"),     -INF <=>  NAN],
    [Decimal::valueOf("-INF"),    Decimal::valueOf( "INF"),     -INF <=>  INF],
    [Decimal::valueOf("-INF"),    Decimal::valueOf("-INF"),     -INF <=> -INF],
    [Decimal::valueOf( "NAN"),    Decimal::valueOf( "NAN"),      NAN <=>  NAN],
    [Decimal::valueOf( "NAN"),    Decimal::valueOf( "INF"),      NAN <=>  INF],
    [Decimal::valueOf( "NAN"),    Decimal::valueOf("-INF"),      NAN <=> -INF],

    [Decimal::valueOf( "INF"),    NAN,                           INF <=>  NAN],
    [Decimal::valueOf( "INF"),    INF,                           INF <=>  INF],
    [Decimal::valueOf( "INF"),   -INF,                           INF <=> -INF],
    [Decimal::valueOf("-INF"),    NAN,                          -INF <=>  NAN],
    [Decimal::valueOf("-INF"),    INF,                          -INF <=>  INF],
    [Decimal::valueOf("-INF"),   -INF,                          -INF <=> -INF],
    [Decimal::valueOf( "NAN"),    NAN,                           NAN <=>  NAN],
    [Decimal::valueOf( "NAN"),    INF,                           NAN <=>  INF],
    [Decimal::valueOf( "NAN"),   -INF,                           NAN <=> -INF],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        // $op1->compareTo($op2),
        $op1 <=> $op2,
    ];

    /**
     * Check compareTo results.
     */
    foreach ($results as $result) {
        if ($result !== $expect) {
            var_dump(compact("index", "op1", "op2", "result", "expect"));
            break;
        }
    }

    /* OPERATORS */

    /* All objects are considered equal to true. */
    if (is_bool($op2)) {
        $expected = [
            "<"  => false,
            ">"  => $op2 ? false : true,
            "<=" => $op2 ? true  : false,
            ">=" => true,
            "==" => $op2 ? true  : false,
        ];

    /* NULL is a special case because objects are always greater than NULL. */
    } else if (is_null($op2)) {
        $expected = [
            "<"  => false,
            ">"  => true,
            "<=" => false,
            ">=" => true,
            "==" => false,
        ];

    /* NAN is a special case because nothing equals NAN, and comparison is undefined. */
    } else if ($op1->isNan() || ($op2 instanceof Decimal && $op2->isNan()) || (is_float($op2) && is_nan($op2))) {
        $expected = [
            "<"  => false,
            ">"  => false,
            "<=" => false,
            ">=" => false,
            "==" => false,
        ];

    /* Match whatever compareTo returns, assuming previous tests are correct. */
    } else {
        $expected = [
            "<"  => $op1->compareTo($op2) == -1,
            ">"  => $op1->compareTo($op2) == 1,
            "<=" => $op1->compareTo($op2) <= 0,
            ">=" => $op1->compareTo($op2) >= 0,
            "==" => $op1->compareTo($op2) == 0,
        ];
    }

    /* Test each operator's result and its symmetrical equivalent. */
    $results = [
        "<"  => [$op1 <  $op2, $op2 >  $op1],
        ">"  => [$op1 >  $op2, $op2 <  $op1],
        "<=" => [$op1 <= $op2, $op2 >= $op1],
        ">=" => [$op1 >= $op2, $op2 <= $op1],
        "==" => [$op1 == $op2, $op2 == $op1],
    ];

    foreach ($results as $op => $result) {
        foreach ($result as $actual) {
            $expect = $expected[$op];

            if ($actual !== $expect) {
                var_dump(compact("op", "op1", "op2", "expect", "actual"));
                break 2;
            }
        }
    }
}

/* Check compared to non-numeric valid type. */
var_dump(Decimal::valueOf(0) <=> "abc");
?>
--EXPECT--
int(1)
