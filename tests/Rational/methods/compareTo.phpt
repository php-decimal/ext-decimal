--TEST--
Rational::compareTo
--FILE--
<?php
use Decimal\Rational;

/**
 * op1, op2, expected result, expected precision
 */
$tests = [
    [$rational = Rational::valueOf("0.1"), $rational, 0],

    [Rational::valueOf(0),       0,      0],
    [Rational::valueOf(0),       null,   1],
    [Rational::valueOf(0),       false,  1],
    [Rational::valueOf(0),       0.0,    0],

    [Rational::valueOf(1),       0,      1],
    [Rational::valueOf(1),       null,   1],
    [Rational::valueOf(1),       false,  1],
    [Rational::valueOf(1),       0.0,    1],

    [Rational::valueOf(NAN),     0,      1],
    [Rational::valueOf(NAN),     null,   1],
    [Rational::valueOf(NAN),     false,  1],
    [Rational::valueOf(NAN),     0.0,    1],

    [Rational::valueOf(0),       1,     -1],
    [Rational::valueOf(0),       true,   0], // Objects are always equal to true.
    [Rational::valueOf(0),       1.0,   -1],
    [Rational::valueOf(0),       "1",   -1],

    [Rational::valueOf(1),       1,      0],
    [Rational::valueOf(1),       true,   0], // Objects are always equal to true.
    [Rational::valueOf(1),       1.0,    0],
    [Rational::valueOf(1),       "1",    0],

    [Rational::valueOf(NAN),     1,      1],
    [Rational::valueOf(NAN),     true,   0], // Objects are always equal to true.
    [Rational::valueOf(NAN),     1.0,    1],
    [Rational::valueOf(NAN),     "1",    1],

    [Rational::valueOf(0),       NAN,    1],
    [Rational::valueOf(1),       NAN,    1],

    [Rational::valueOf("0.1"),   0.1,    0],
    [Rational::valueOf("0.2"),   0.2,    0],
    [Rational::valueOf("0.200"), 0.2,    0],
    [Rational::valueOf("0.200"), 0.200,  0],
    [Rational::valueOf("1E-10"), 1E-10,  0],
    [Rational::valueOf("1E+10"), 1E+10,  0],

    [Rational::valueOf("0.1000000000000000000001"), 0.1,   0],
    [Rational::valueOf("0.2000000000000000000001"), 0.2,   0],

    [Rational::valueOf("0.0000000000000000000001"), 0.1,  -1],
    [Rational::valueOf("0.0000000000000000000001"), 0.0,   1],

    [Rational::valueOf("0.123"), Rational::valueOf("0.1234"), -1],

    [Rational::valueOf("-0.1"),     -0.1, 0],
    [Rational::valueOf("-0.2"),     -0.2, 0],
    [Rational::valueOf("-1E-10"), -1E-10, 0],
    [Rational::valueOf("-1E+10"), -1E+10, 0],

    [Rational::valueOf("0.1"),     "0.01",      1],
    [Rational::valueOf("0.1"),     "0.1",       0],
    [Rational::valueOf("0.1"),     "0.2",      -1],
    [Rational::valueOf("0.2"),     "0.1",       1],
    [Rational::valueOf("0.01"),    "0.1",      -1],

    [Rational::valueOf("0.1"),     Rational::valueOf("0.01"),      1],
    [Rational::valueOf("0.1"),     Rational::valueOf("0.1"),       0],
    [Rational::valueOf("0.1"),     Rational::valueOf("0.2"),      -1],
    [Rational::valueOf("0.2"),     Rational::valueOf("0.1"),       1],
    [Rational::valueOf("0.01"),    Rational::valueOf("0.1"),      -1],

    [Rational::valueOf( "INF"),    NAN,                            INF <=>  NAN],
    [Rational::valueOf( "INF"),    INF,                            INF <=>  INF],
    [Rational::valueOf( "INF"),   -INF,                            INF <=> -INF],
    [Rational::valueOf( "INF"),    Rational::valueOf( "NAN"),      INF <=>  NAN],
    [Rational::valueOf( "INF"),    Rational::valueOf( "INF"),      INF <=>  INF],
    [Rational::valueOf( "INF"),    Rational::valueOf("-INF"),      INF <=> -INF],
    [Rational::valueOf( "INF"),    0,                              INF <=>  0],
    [Rational::valueOf( "INF"),    1,                              INF <=>  1],
    [Rational::valueOf( "INF"),   -1,                              INF <=> -1],

    [Rational::valueOf("-INF"),    NAN,                           -INF <=>  NAN],
    [Rational::valueOf("-INF"),    INF,                           -INF <=>  INF],
    [Rational::valueOf("-INF"),   -INF,                           -INF <=> -INF],
    [Rational::valueOf("-INF"),    Rational::valueOf( "NAN"),     -INF <=>  NAN],
    [Rational::valueOf("-INF"),    Rational::valueOf( "INF"),     -INF <=>  INF],
    [Rational::valueOf("-INF"),    Rational::valueOf("-INF"),     -INF <=> -INF],
    [Rational::valueOf("-INF"),    0,                             -INF <=>  0],
    [Rational::valueOf("-INF"),    1,                             -INF <=>  1],
    [Rational::valueOf("-INF"),   -1,                             -INF <=> -1],

    [Rational::valueOf( "NAN"),    NAN,                            NAN <=>  NAN],
    [Rational::valueOf( "NAN"),    INF,                            NAN <=>  INF],
    [Rational::valueOf( "NAN"),   -INF,                            NAN <=> -INF],
    [Rational::valueOf( "NAN"),    Rational::valueOf( "NAN"),      NAN <=>  NAN],
    [Rational::valueOf( "NAN"),    Rational::valueOf( "INF"),      NAN <=>  INF],
    [Rational::valueOf( "NAN"),    Rational::valueOf("-INF"),      NAN <=> -INF],
    [Rational::valueOf( "NAN"),    0,                              NAN <=>  0],
    [Rational::valueOf( "NAN"),    1,                              NAN <=>  1],
    [Rational::valueOf( "NAN"),   -1,                              NAN <=> -1],
];

// TODO test comparing against Number
// TODO test comparing against Decimal
// TODO test the equivalent in both the others.
// TODO aside: test all operations against the other types.

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->compareTo($op2),
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
            "!=" => $op2 ? false : true,
            "==" => $op2 ? true  : false,
        ];

    /* NULL is a special case because objects are always greater than NULL. */
    } else if (is_null($op2)) {
        $expected = [
            "<"  => false,
            ">"  => true,
            "<=" => false,
            ">=" => true,
            "!=" => true,
            "==" => false,
        ];

    /* NAN is a special case because nothing equals NAN, and comparison is undefined. */
    } else if ($op1->isNan() || ($op2 instanceof Rational && $op2->isNan()) || (is_float($op2) && is_nan($op2))) {
        $expected = [
            "<"  => false,
            ">"  => false,
            "<=" => false,
            ">=" => false,
            "!=" => true,
            "==" => false,
        ];

    /* Match whatever compareTo returns, assuming previous tests are correct. */
    } else {
        $expected = [
            "<"  => $op1->compareTo($op2) == -1,
            ">"  => $op1->compareTo($op2) == 1,
            "<=" => $op1->compareTo($op2) <= 0,
            ">=" => $op1->compareTo($op2) >= 0,
            "!=" => $op1->compareTo($op2) != 0,
            "==" => $op1->compareTo($op2) == 0,
        ];
    }

    /* Test each operator's result and its symmetrical equivalent. */
    $results = [
        "<"  => [$op1 <  $op2, $op2 >  $op1],
        ">"  => [$op1 >  $op2, $op2 <  $op1],
        "<=" => [$op1 <= $op2, $op2 >= $op1],
        ">=" => [$op1 >= $op2, $op2 <= $op1],
        "!=" => [$op1 != $op2, $op2 != $op1],
        "==" => [$op1 == $op2, $op2 == $op1, $op1->equals($op2)],
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
var_dump(Rational::valueOf(0) <=> "abc");
?>
--EXPECT--
int(1)
