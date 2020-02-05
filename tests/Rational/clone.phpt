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
--EXPECTF--
object(Decimal\Rational)#%d (2) {
  ["num"]=>
  string(3) "617"
  ["den"]=>
  string(3) "500"
}
object(Decimal\Rational)#%d (2) {
  ["num"]=>
  string(3) "617"
  ["den"]=>
  string(3) "500"
}
