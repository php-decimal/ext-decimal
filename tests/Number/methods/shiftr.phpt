--TEST--
Number::shiftr
--FILE--
<?php
require __DIR__ . "/../helpers/Number.php";

print_r(Number::valueOf(4) >>  2);
print_r(Number::valueOf(4) >> -2);

print_r(Number::valueOf(4) >> "1");

print_r(Number::valueOf(4)->shiftr( 2));
print_r(Number::valueOf(4)->shiftr(-2));

print_r(Number::valueOf(4)->shiftr("1"));
?>
--EXPECT--
Number::shiftr
Number Object
(
    [value:protected] => 0.04
)
Number::shiftr
Number Object
(
    [value:protected] => 400
)
Number::shiftr
Number Object
(
    [value:protected] => 0.4
)
Number::shiftr
Number Object
(
    [value:protected] => 0.04
)
Number::shiftr
Number Object
(
    [value:protected] => 400
)
Number::shiftr
Number Object
(
    [value:protected] => 0.4
)