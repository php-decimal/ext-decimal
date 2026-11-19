--TEST--
Decimal serialize/unserialize
--FILE--
<?php
use Decimal\Decimal;

$decimal = Decimal::valueOf("1234.5678E+9", 42);

var_dump(serialize($decimal));
var_dump(unserialize(serialize($decimal)));

?>
--EXPECTF--
string(81) "O:15:"Decimal\Decimal":2:{s:5:"value";s:13:"1.2345678E+12";s:9:"precision";i:42;}"
object(Decimal\Decimal)#%d (2) {
  ["value"]=>
  string(13) "1234567800000"
  ["precision"]=>
  int(42)
}
