--TEST--
Number::floor
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

print_r(Number::valueOf(0)->floor());
print_r(Number::valueOf("0")->floor());

print_r(Number::valueOf( "0.1")->floor());
print_r(Number::valueOf("-0.1")->floor());

print_r(Number::valueOf( NAN)->floor());
print_r(Number::valueOf( INF)->floor());
print_r(Number::valueOf(-INF)->floor());

?>
--EXPECT--
Number::toDecimal
Number Object
(
    [value:protected] => 0
)
Number::toDecimal
Number Object
(
    [value:protected] => 0
)
Number::toDecimal
Number Object
(
    [value:protected] => 0
)
Number::toDecimal
Number Object
(
    [value:protected] => -1
)
Number::toDecimal
Number Object
(
    [value:protected] => NAN
)
Number::toDecimal
Number Object
(
    [value:protected] => INF
)
Number::toDecimal
Number Object
(
    [value:protected] => -INF
)