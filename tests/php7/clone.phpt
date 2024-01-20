--TEST--
Decimal clone
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Decimal;

$a = new Decimal("1.234", 16);
$b = clone $a;

var_dump($a);
var_dump($b);

$a = new Decimal("-0");
$b = clone $a;

var_dump($a);
var_dump($b);
?>
--EXPECT--
object(Decimal\Decimal)#1 (2) {
  ["value"]=>
  string(5) "1.234"
  ["precision"]=>
  int(16)
}
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(5) "1.234"
  ["precision"]=>
  int(16)
}
object(Decimal\Decimal)#3 (2) {
  ["value"]=>
  string(1) "0"
  ["precision"]=>
  int(28)
}
object(Decimal\Decimal)#1 (2) {
  ["value"]=>
  string(1) "0"
  ["precision"]=>
  int(28)
}
