--TEST--
Rational serialize/unserialize
--FILE--
<?php
use Decimal\Rational;

$obj = Rational::valueOf("2.4");

var_dump(serialize($obj));
var_dump(unserialize(serialize($obj)));

/* */
var_dump(unserialize('C:16:"Decimal\Rational":26:{s:13:"abc0000000000";i:42;}'));

?>
--EXPECTF--
string(53) "C:16:"Decimal\Rational":24:{s:6:"1.2E+1";s:4:"5E+0";}"
object(Decimal\Rational)#4 (2) {
  ["num"]=>
  string(2) "12"
  ["den"]=>
  string(1) "5"
}

Fatal error: Uncaught RuntimeException: Failed to unserialize decimal number string in %s
Stack trace:
#0 %s: unserialize('C:16:"Decimal\\R...')
#1 {main}
  thrown in %s on line %d
