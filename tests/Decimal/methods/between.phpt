--TEST--
Decimal::between
--FILE--
<?php
use Decimal\Decimal;

/**
 * obj, op1, op2, inclusive?, expected result
 */
$tests = [
    [Decimal::valueOf(0), 0, 0, $inclusive = true,  $expect = true],
    [Decimal::valueOf(0), 0, 0, $inclusive = true,  $expect = true],
    [Decimal::valueOf(0), 0, 0, $inclusive = false, $expect = false],
    [Decimal::valueOf(0), 0, 0, $inclusive = false, $expect = false],

    [Decimal::valueOf(5), 4, 6, $inclusive = true,  $expect = true],
    [Decimal::valueOf(5), 6, 4, $inclusive = true,  $expect = true],
    [Decimal::valueOf(5), 4, 5, $inclusive = true,  $expect = true],
    [Decimal::valueOf(5), 5, 4, $inclusive = true,  $expect = true],
    [Decimal::valueOf(5), 5, 5, $inclusive = true,  $expect = true],

    [Decimal::valueOf(5), 3, 4, $inclusive = true,  $expect = false],
    [Decimal::valueOf(5), 4, 3, $inclusive = true,  $expect = false],
    [Decimal::valueOf(5), 7, 6, $inclusive = true,  $expect = false],
    [Decimal::valueOf(5), 6, 7, $inclusive = true,  $expect = false],

    [Decimal::valueOf(5), 4, 5, $inclusive = false, $expect = false],
    [Decimal::valueOf(5), 5, 4, $inclusive = false, $expect = false],
    [Decimal::valueOf(5), 5, 5, $inclusive = false, $expect = false],

    [Decimal::valueOf(5), "4.99", "5.01", $inclusive = true,  $expect = true],
    [Decimal::valueOf(5), "4.99", "5.01", $inclusive = false, $expect = true],

    [Decimal::valueOf(5), "4.99", "5.00", $inclusive = true,  $expect = true],
    [Decimal::valueOf(5), "4.99", "5.00", $inclusive = false, $expect = false],

    [Decimal::valueOf( "INF"),    NAN,  NAN,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "INF"),    INF,  NAN,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "INF"),   -INF,  NAN,  $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),    NAN,  NAN,  $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),    INF,  NAN,  $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),   -INF,  NAN,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "NAN"),    NAN,  NAN,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "NAN"),    INF,  NAN,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "NAN"),   -INF,  NAN,  $inclusive = true,   $expect = false],

    [Decimal::valueOf( "INF"),    NAN,  INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "INF"),    INF,  INF,  $inclusive = true,   $expect = true],
    [Decimal::valueOf( "INF"),   -INF,  INF,  $inclusive = true,   $expect = true],
    [Decimal::valueOf("-INF"),    NAN,  INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),    INF,  INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),   -INF,  INF,  $inclusive = true,   $expect = true],
    [Decimal::valueOf( "NAN"),    NAN,  INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "NAN"),    INF,  INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "NAN"),   -INF,  INF,  $inclusive = true,   $expect = false],

    [Decimal::valueOf( "INF"),    NAN, -INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "INF"),    INF, -INF,  $inclusive = true,   $expect = true],
    [Decimal::valueOf( "INF"),   -INF, -INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),    NAN, -INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),    INF, -INF,  $inclusive = true,   $expect = true],
    [Decimal::valueOf("-INF"),   -INF, -INF,  $inclusive = true,   $expect = true],
    [Decimal::valueOf( "NAN"),    NAN, -INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "NAN"),    INF, -INF,  $inclusive = true,   $expect = false],
    [Decimal::valueOf( "NAN"),   -INF, -INF,  $inclusive = true,   $expect = false],

    [Decimal::valueOf( "INF"),    NAN,  NAN,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "INF"),    INF,  NAN,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "INF"),   -INF,  NAN,  $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),    NAN,  NAN,  $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),    INF,  NAN,  $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),   -INF,  NAN,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),    NAN,  NAN,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),    INF,  NAN,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),   -INF,  NAN,  $inclusive = false,  $expect = false],

    [Decimal::valueOf( "INF"),    NAN,  INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "INF"),    INF,  INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "INF"),   -INF,  INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),    NAN,  INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),    INF,  INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),   -INF,  INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),    NAN,  INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),    INF,  INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),   -INF,  INF,  $inclusive = false,  $expect = false],

    [Decimal::valueOf( "INF"),    NAN, -INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "INF"),    INF, -INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "INF"),   -INF, -INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),    NAN, -INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),    INF, -INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),   -INF, -INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),    NAN, -INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),    INF, -INF,  $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),   -INF, -INF,  $inclusive = false,  $expect = false],

    [Decimal::valueOf( "INF"),    NAN,  0,    $inclusive = true,   $expect = false],
    [Decimal::valueOf( "INF"),    INF,  0,    $inclusive = true,   $expect = true],
    [Decimal::valueOf( "INF"),   -INF,  0,    $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),    NAN,  0,    $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),    INF,  0,    $inclusive = true,   $expect = false],
    [Decimal::valueOf("-INF"),   -INF,  0,    $inclusive = true,   $expect = true],
    [Decimal::valueOf( "NAN"),    NAN,  0,    $inclusive = true,   $expect = false],
    [Decimal::valueOf( "NAN"),    INF,  0,    $inclusive = true,   $expect = false],
    [Decimal::valueOf( "NAN"),   -INF,  0,    $inclusive = true,   $expect = false],

    [Decimal::valueOf( "INF"),    0, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf( "INF"),    0, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf( "INF"),    0, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),    0, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),    0, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf("-INF"),    0, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),    0, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),    0, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf( "NAN"),    0, -INF,    $inclusive = false,  $expect = false],

    [Decimal::valueOf(0),       NAN,  INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf(0),       INF,  INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf(0),      -INF,  INF,    $inclusive = false,  $expect = true],
    [Decimal::valueOf(0),       NAN, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf(0),       INF, -INF,    $inclusive = false,  $expect = true],
    [Decimal::valueOf(0),      -INF, -INF,    $inclusive = false,  $expect = false],
    [Decimal::valueOf(0),       NAN,  NAN,    $inclusive = false,  $expect = false],
    [Decimal::valueOf(0),       INF,  NAN,    $inclusive = false,  $expect = false],
    [Decimal::valueOf(0),      -INF,  NAN,    $inclusive = false,  $expect = false],
];

foreach ($tests as $index => $test) {
    list($obj, $op1, $op2, $inc, $expect) = $test;

    $result = $obj->between($op1, $op2, $inc);

    if ($result !== $expect) {
        var_dump(compact("index", "obj", "op1", "op2", "inc", "result", "expect"));
    }
}

/* Check default inclusivity */
var_dump(Decimal::valueOf(0)->between(0, 1)); // true
var_dump(Decimal::valueOf(0)->between(1, 0)); // true
var_dump(Decimal::valueOf(1)->between(0, 1)); // true
var_dump(Decimal::valueOf(1)->between(1, 0)); // true

var_dump(Decimal::valueOf(0)->between(1, 2)); // false
var_dump(Decimal::valueOf(0)->between(2, 1)); // false
var_dump(Decimal::valueOf(3)->between(1, 2)); // false
var_dump(Decimal::valueOf(3)->between(2, 1)); // false

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
