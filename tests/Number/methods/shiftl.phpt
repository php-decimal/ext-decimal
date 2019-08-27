--TEST--
Number::shiftl
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

print_r(Number::valueOf(4) <<  2);
print_r(Number::valueOf(4) << -2);

print_r(Number::valueOf(4) << "1");

print_r(Number::valueOf(4)->shiftl( 2));
print_r(Number::valueOf(4)->shiftl(-2));

print_r(Number::valueOf(4)->shiftl("1"));

?>
--EXPECT--
Number::shiftl
Number Object
(
    [value:protected] => 400
)
Number::shiftl
Number Object
(
    [value:protected] => 0.04
)
Number::shiftl
Number Object
(
    [value:protected] => 40
)
Number::shiftl
Number Object
(
    [value:protected] => 400
)
Number::shiftl
Number Object
(
    [value:protected] => 0.04
)
Number::shiftl
Number Object
(
    [value:protected] => 40
)