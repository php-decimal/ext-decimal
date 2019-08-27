--TEST--
Number::abs
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

print_r(Number::valueOf("0")->abs());
print_r(Number::valueOf(0)->abs());

print_r(Number::valueOf( "5")->abs());
print_r(Number::valueOf("-5")->abs());

print_r(Number::valueOf( 5)->abs());
print_r(Number::valueOf(-5)->abs());

print_r(Number::valueOf( NAN)->abs());
print_r(Number::valueOf( INF)->abs());
print_r(Number::valueOf(-INF)->abs());

?>
--EXPECT--
Number Object
(
    [value:protected] => 0
)
Number Object
(
    [value:protected] => 0
)
Number Object
(
    [value:protected] => 5
)
Number::mul
Number Object
(
    [value:protected] => 5
)
Number Object
(
    [value:protected] => 5
)
Number::mul
Number Object
(
    [value:protected] => 5
)
Number Object
(
    [value:protected] => NAN
)
Number Object
(
    [value:protected] => INF
)
Number::mul
Number Object
(
    [value:protected] => INF
)