--TEST--
Decimal::toRational
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

print_r(Decimal::valueOf(0)->toRational());
print_r(Decimal::valueOf("1.234")->toRational());
print_r(Decimal::valueOf("1.000")->toRational());

?>
--EXPECT--
Decimal\Rational Object
(
    [num] => 0
    [den] => 1
)
Decimal\Rational Object
(
    [num] => 617
    [den] => 500
)
Decimal\Rational Object
(
    [num] => 1
    [den] => 1
)
