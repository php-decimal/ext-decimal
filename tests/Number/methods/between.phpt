--TEST--
Number::between
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

/**
 * obj, op1, op2, expected result
 */
$tests = [

    /**
     *
     */
    [
        [Number::valueOf(0), 0, 0, $inclusive = true,  $expect = true],
        [Number::valueOf(0), 0, 0, $inclusive = true,  $expect = true],
        [Number::valueOf(0), 0, 0, $inclusive = false, $expect = false],
        [Number::valueOf(0), 0, 0, $inclusive = false, $expect = false],

        [Number::valueOf(5), 4, 6, $inclusive = true,  $expect = true],
        [Number::valueOf(5), 6, 4, $inclusive = true,  $expect = true],
        [Number::valueOf(5), 4, 5, $inclusive = true,  $expect = true],
        [Number::valueOf(5), 5, 4, $inclusive = true,  $expect = true],
        [Number::valueOf(5), 5, 5, $inclusive = true,  $expect = true],

        [Number::valueOf(5), 3, 4, $inclusive = true,  $expect = false],
        [Number::valueOf(5), 4, 3, $inclusive = true,  $expect = false],
        [Number::valueOf(5), 7, 6, $inclusive = true,  $expect = false],
        [Number::valueOf(5), 6, 7, $inclusive = true,  $expect = false],

        [Number::valueOf(5), 4, 5, $inclusive = false, $expect = false],
        [Number::valueOf(5), 5, 4, $inclusive = false, $expect = false],
        [Number::valueOf(5), 5, 5, $inclusive = false, $expect = false],
    ],

    /**
     *
     */
    [
        [Number::valueOf(5), "4.99", "5.01", $inclusive = true,  $expect = true],
        [Number::valueOf(5), "4.99", "5.01", $inclusive = false, $expect = true],

        [Number::valueOf(5), "4.99", "5.00", $inclusive = true,  $expect = true],
        [Number::valueOf(5), "4.99", "5.00", $inclusive = false, $expect = false],
    ],

    /**
     *
     */
    [
        [Number::valueOf( "INF"),    NAN,  NAN,  $inclusive = true,   $expect = false],
        [Number::valueOf( "INF"),    INF,  NAN,  $inclusive = true,   $expect = false],
        [Number::valueOf( "INF"),   -INF,  NAN,  $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),    NAN,  NAN,  $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),    INF,  NAN,  $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),   -INF,  NAN,  $inclusive = true,   $expect = false],
        [Number::valueOf( "NAN"),    NAN,  NAN,  $inclusive = true,   $expect = false],
        [Number::valueOf( "NAN"),    INF,  NAN,  $inclusive = true,   $expect = false],
        [Number::valueOf( "NAN"),   -INF,  NAN,  $inclusive = true,   $expect = false],

        [Number::valueOf( "INF"),    NAN,  INF,  $inclusive = true,   $expect = false],
        [Number::valueOf( "INF"),    INF,  INF,  $inclusive = true,   $expect = true],
        [Number::valueOf( "INF"),   -INF,  INF,  $inclusive = true,   $expect = true],
        [Number::valueOf("-INF"),    NAN,  INF,  $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),    INF,  INF,  $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),   -INF,  INF,  $inclusive = true,   $expect = true],
        [Number::valueOf( "NAN"),    NAN,  INF,  $inclusive = true,   $expect = false],
        [Number::valueOf( "NAN"),    INF,  INF,  $inclusive = true,   $expect = false],
        [Number::valueOf( "NAN"),   -INF,  INF,  $inclusive = true,   $expect = false],

        [Number::valueOf( "INF"),    NAN, -INF,  $inclusive = true,   $expect = false],
        [Number::valueOf( "INF"),    INF, -INF,  $inclusive = true,   $expect = true],
        [Number::valueOf( "INF"),   -INF, -INF,  $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),    NAN, -INF,  $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),    INF, -INF,  $inclusive = true,   $expect = true],
        [Number::valueOf("-INF"),   -INF, -INF,  $inclusive = true,   $expect = true],
        [Number::valueOf( "NAN"),    NAN, -INF,  $inclusive = true,   $expect = false],
        [Number::valueOf( "NAN"),    INF, -INF,  $inclusive = true,   $expect = false],
        [Number::valueOf( "NAN"),   -INF, -INF,  $inclusive = true,   $expect = false],
    ],

    /**
     *
     */
    [
        [Number::valueOf( "INF"),    NAN,  NAN,  $inclusive = false,  $expect = false],
        [Number::valueOf( "INF"),    INF,  NAN,  $inclusive = false,  $expect = false],
        [Number::valueOf( "INF"),   -INF,  NAN,  $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),    NAN,  NAN,  $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),    INF,  NAN,  $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),   -INF,  NAN,  $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),    NAN,  NAN,  $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),    INF,  NAN,  $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),   -INF,  NAN,  $inclusive = false,  $expect = false],

        [Number::valueOf( "INF"),    NAN,  INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "INF"),    INF,  INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "INF"),   -INF,  INF,  $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),    NAN,  INF,  $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),    INF,  INF,  $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),   -INF,  INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),    NAN,  INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),    INF,  INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),   -INF,  INF,  $inclusive = false,  $expect = false],

        [Number::valueOf( "INF"),    NAN, -INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "INF"),    INF, -INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "INF"),   -INF, -INF,  $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),    NAN, -INF,  $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),    INF, -INF,  $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),   -INF, -INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),    NAN, -INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),    INF, -INF,  $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),   -INF, -INF,  $inclusive = false,  $expect = false],
    ],

    /**
     *
     */
    [
        [Number::valueOf( "INF"),    NAN,  0,    $inclusive = true,   $expect = false],
        [Number::valueOf( "INF"),    INF,  0,    $inclusive = true,   $expect = true],
        [Number::valueOf( "INF"),   -INF,  0,    $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),    NAN,  0,    $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),    INF,  0,    $inclusive = true,   $expect = false],
        [Number::valueOf("-INF"),   -INF,  0,    $inclusive = true,   $expect = true],
        [Number::valueOf( "NAN"),    NAN,  0,    $inclusive = true,   $expect = false],
        [Number::valueOf( "NAN"),    INF,  0,    $inclusive = true,   $expect = false],
        [Number::valueOf( "NAN"),   -INF,  0,    $inclusive = true,   $expect = false],

        [Number::valueOf( "INF"),    0, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf( "INF"),    0, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf( "INF"),    0, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),    0, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),    0, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf("-INF"),    0, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),    0, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),    0, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf( "NAN"),    0, -INF,    $inclusive = false,  $expect = false],
    ],

    /**
     *
     */
    [
        [Number::valueOf(0),       NAN,  INF,    $inclusive = false,  $expect = false],
        [Number::valueOf(0),       INF,  INF,    $inclusive = false,  $expect = false],
        [Number::valueOf(0),      -INF,  INF,    $inclusive = false,  $expect = true],
        [Number::valueOf(0),       NAN, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf(0),       INF, -INF,    $inclusive = false,  $expect = true],
        [Number::valueOf(0),      -INF, -INF,    $inclusive = false,  $expect = false],
        [Number::valueOf(0),       NAN,  NAN,    $inclusive = false,  $expect = false],
        [Number::valueOf(0),       INF,  NAN,    $inclusive = false,  $expect = false],
        [Number::valueOf(0),      -INF,  NAN,    $inclusive = false,  $expect = false],
    ]
];

$index = 0;

foreach ($tests as $group) {
    foreach ($group as $test) {
        list($obj, $op1, $op2, $inc, $expect) = $test;

        $result = $obj->between($op1, $op2, $inc);

        if ($result !== $expect) {
            var_dump(compact("group", "index", "obj", "op1", "op2", "inc", "result", "expect"));
        }

        $index++;
    }
    printf("\n");
}

/* Check default inclusivity */
var_dump(Number::valueOf(0)->between(0, 1)); // true
var_dump(Number::valueOf(0)->between(1, 0)); // true
var_dump(Number::valueOf(1)->between(0, 1)); // true
var_dump(Number::valueOf(1)->between(1, 0)); // true
printf("\n");

var_dump(Number::valueOf(0)->between(1, 2)); // false
var_dump(Number::valueOf(0)->between(2, 1)); // false
var_dump(Number::valueOf(3)->between(1, 2)); // false
var_dump(Number::valueOf(3)->between(2, 1)); // false
printf("\n");

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)

bool(false)
bool(false)
bool(false)
bool(false)