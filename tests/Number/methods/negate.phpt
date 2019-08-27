--TEST--
Number::negate
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

/**
 * op1, expected result
 */
$tests = [
    [ "1E-3",   "-0.001"],
    ["-1E-3",    "0.001"],

    [0,          "0"], // There is no -0 in PHP, so userland implementation can't support it.
    [1,         "-1"],
    [2,         "-2"],
    [3,         "-3"],

    ["-0",       "0"],
    ["-1",       "1"],
    ["-2",       "2"],
    ["-3",       "3"],

    [-INF,       "INF"],
    [ INF,      "-INF"],
    [ NAN,       "NAN"],
];

foreach ($tests as $test) {
    list($number, $expect) = $test;

    $result = Number::valueOf($number)->negate()->toString();

    if ($result !== $expect) {
        var_dump(compact("number", "result", "expect"));
    }
    printf("\n");
}

?>
--EXPECT--
Number::mul

Number::mul

Number::mul

Number::mul

Number::mul

Number::mul

Number::mul

Number::mul

Number::mul

Number::mul

Number::mul

Number::mul

Number::mul