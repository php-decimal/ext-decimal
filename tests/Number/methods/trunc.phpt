--TEST--
Number::trunc
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

print_r(Number::valueOf(0)->trunc());
print_r(Number::valueOf("0")->trunc());

print_r(Number::valueOf( "0.1")->trunc());
print_r(Number::valueOf("-0.1")->trunc());

print_r(Number::valueOf( NAN)->trunc());
print_r(Number::valueOf( INF)->trunc());
print_r(Number::valueOf(-INF)->trunc());

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