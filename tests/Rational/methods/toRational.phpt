--TEST--
Rational::toRational
--FILE--
<?php
use Decimal\Rational;

print_r(Rational::valueOf(0)->toRational());
print_r(Rational::valueOf("1.234")->toRational());
print_r(Rational::valueOf("1.000")->toRational());

$a = Rational::valueOf("1.000");
$b = $a->toRational();

$a = $a + 1;

print_r($a);
print_r($b);

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
Decimal\Rational Object
(
    [num] => 2
    [den] => 1
)
Decimal\Rational Object
(
    [num] => 1
    [den] => 1
)
