--TEST--
Rational clone
--FILE--
<?php
use Decimal\Rational;

$a = Rational::valueOf("1.234");
$b = clone $a;

var_dump($a);
var_dump($b);
?>
--EXPECT--
object(Decimal\Rational)#1 (2) {
  ["num"]=>
  string(3) "617"
  ["den"]=>
  string(3) "500"
}
object(Decimal\Rational)#2 (2) {
  ["num"]=>
  string(3) "617"
  ["den"]=>
  string(3) "500"
}
