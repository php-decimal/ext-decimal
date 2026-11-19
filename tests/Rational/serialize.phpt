--TEST--
Rational serialize/unserialize
--FILE--
<?php
use Decimal\Rational;

$obj = Rational::valueOf("2.4");

var_dump(serialize($obj));
var_dump(unserialize(serialize($obj)));

?>
--EXPECTF--
string(72) "O:16:"Decimal\Rational":2:{s:3:"num";s:6:"1.2E+1";s:3:"den";s:4:"5E+0";}"
object(Decimal\Rational)#%d (2) {
  ["num"]=>
  string(2) "12"
  ["den"]=>
  string(1) "5"
}
