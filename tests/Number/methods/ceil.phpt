--TEST--
Number::ceil
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

print_r(Number::valueOf(0)->ceil());
print_r(Number::valueOf("0")->ceil());

print_r(Number::valueOf( "0.1")->ceil());
print_r(Number::valueOf("-0.1")->ceil());

print_r(Number::valueOf( NAN)->ceil());
print_r(Number::valueOf( INF)->ceil());
print_r(Number::valueOf(-INF)->ceil());

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
    [value:protected] => 1
)
Number::toDecimal
Number Object
(
    [value:protected] => -0
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