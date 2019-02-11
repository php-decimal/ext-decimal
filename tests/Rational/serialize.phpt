--TEST--
Rational serialize/unserialize
--FILE--
<?php
use Decimal\Rational;

$obj = Rational::valueOf("2.4");

var_dump(serialize($obj));
var_dump(unserialize(serialize($obj)));
?>
--EXPECT--
string(46) "C:16:"Decimal\Rational":17:{s:2:"12";s:1:"5";}"
object(Decimal\Rational)#2 (2) {
  ["num"]=>
  string(2) "12"
  ["den"]=>
  string(1) "5"
}
