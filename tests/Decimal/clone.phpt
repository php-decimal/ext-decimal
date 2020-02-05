--TEST--
Decimal clone
--FILE--
<?php
use Decimal\Decimal;

$a = Decimal::valueOf("1.234", 16);
$b = clone $a;

var_dump($a);
var_dump($b);
?>
--EXPECTF--
object(Decimal\Decimal)#%d (2) {
  ["value"]=>
  string(5) "1.234"
  ["precision"]=>
  int(16)
}
object(Decimal\Decimal)#%d (2) {
  ["value"]=>
  string(5) "1.234"
  ["precision"]=>
  int(16)
}
