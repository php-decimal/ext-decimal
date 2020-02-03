--TEST--
Number::toRational
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

/**
 * op1, expected result
 */
$tests = [
    [ "1E-3",    "1/1000"],
    ["-1E-3",   "-1/1000"],

    [0,          "0"], // There is no -0 in PHP, so userland implementation can't support it.
    [1,          "1"],
    [2,          "2"],
    [3,          "3"],

    ["-0.5",    "-1/2"],
    ["-1.5",    "-3/2"],
    ["-2.5",    "-5/2"],
    ["-3.5",    "-7/2"],

    [-INF,      "-INF"],
    [ INF,       "INF"],
    [ NAN,       "NAN"],
];

foreach ($tests as $test) {
    list($number, $expect) = $test;

    $result = Number::valueOf($number)->toRational();

    assert($result instanceof \Decimal\Rational);

    if (($result = $result->toString()) !== $expect) {
        var_dump(compact("number", "result", "expect"));
    }

    printf("\n");
}

?>
--EXPECT--
Number::toRational

Number::toRational

Number::toRational

Number::toRational

Number::toRational

Number::toRational

Number::toRational

Number::toRational

Number::toRational

Number::toRational

Number::toRational

Number::toRational

Number::toRational