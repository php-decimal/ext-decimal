--TEST--
Decimal serialize/unserialize
--FILE--
<?php
use Decimal\Decimal;

$decimal = Decimal::valueOf("1234.5678E+9", 42);

var_dump(serialize($decimal));
var_dump(unserialize(serialize($decimal)));
var_dump(unserialize('C:15:"Decimal\Decimal":26:{s:13:"abc0000000000";i:42;}'));

?>
--EXPECTF--
string(54) "C:15:"Decimal\Decimal":26:{s:13:"1.2345678E+12";i:42;}"
object(Decimal\Decimal)#%d (2) {
  ["value"]=>
  string(13) "1234567800000"
  ["precision"]=>
  int(42)
}

Fatal error: Uncaught RuntimeException: Failed to unserialize decimal number string in %s
Stack trace:
#0 %s: unserialize('C:15:"Decimal\\D...')
#1 {main}
  thrown in %s on line %d
